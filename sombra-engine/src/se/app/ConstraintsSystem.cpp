#include <glm/gtx/string_cast.hpp>
#include "se/utils/Log.h"
#include "se/physics/RigidBody.h"
#include "se/app/ConstraintsSystem.h"
#include "se/app/EntityDatabase.h"
#include "se/app/TransformsComponent.h"
#include "se/app/events/CollisionEvent2.h"

namespace se::app {

	ConstraintsSystem::ConstraintsSystem(
		EntityDatabase& entityDatabase, physics::PhysicsEngine& physicsEngine,
		EventManager& eventManager
	) : ISystem(entityDatabase), mPhysicsEngine(physicsEngine), mEventManager(eventManager)
	{
		// TODO: reserve max contact constraints
		mEventManager.subscribe(this, Topic::Collision);
	}


	ConstraintsSystem::~ConstraintsSystem()
	{
		mEventManager.unsubscribe(this, Topic::Collision);
	}


	void ConstraintsSystem::notify(const IEvent& event)
	{
		tryCall(&ConstraintsSystem::onCollisionEvent, event);
	}


	void ConstraintsSystem::update()
	{
		SOMBRA_INFO_LOG << "Start";

		mPhysicsEngine.resetRigidBodiesState();

		SOMBRA_DEBUG_LOG << "Updating the RigidBodies";
		mEntityDatabase.iterateComponents<TransformsComponent, physics::RigidBody>(
			[this](Entity, TransformsComponent* transforms, physics::RigidBody* rigidBody) {
				// Skip the Entity physics change in the doDynamics step
				auto updatedWithoutPhysics = transforms->updated;
				updatedWithoutPhysics.reset( static_cast<int>(TransformsComponent::Update::Physics) );
				if (updatedWithoutPhysics.any()) {
					rigidBody->getData().position		= transforms->position;
					rigidBody->getData().linearVelocity	= transforms->velocity;
					rigidBody->getData().orientation	= transforms->orientation;
					rigidBody->synchWithData();
				}
			}
		);

		SOMBRA_DEBUG_LOG << "Updating the NormalConstraints time";
		for (auto& pair : mManifoldConstraintsMap) {
			for (ContactConstraints& contactConstraints : pair.second) {
				contactConstraints.normalConstraint.setDeltaTime(mDeltaTime);
			}
		}

		SOMBRA_DEBUG_LOG << "Solving the Constraints";
		mPhysicsEngine.solveConstraints(mDeltaTime);

		SOMBRA_DEBUG_LOG << "Updating the Entities";
		mEntityDatabase.iterateComponents<TransformsComponent, physics::RigidBody>(
			[this](Entity, TransformsComponent* transforms, physics::RigidBody* rigidBody) {
				if (rigidBody->checkState(physics::RigidBodyState::ConstraintsSolved)) {
					transforms->position	= rigidBody->getData().position;
					transforms->velocity	= rigidBody->getData().linearVelocity;
					transforms->orientation	= rigidBody->getData().orientation;
					transforms->updated.set( static_cast<int>(TransformsComponent::Update::Physics) );
				}
			}
		);

		SOMBRA_DEBUG_LOG << "Putting the RigidBodies to sleep";
		mPhysicsEngine.checkSleepyRigidBodies(mDeltaTime);

		SOMBRA_INFO_LOG << "End";
	}

// Private functions
	void ConstraintsSystem::onCollisionEvent(const CollisionEvent& event)
	{
		SOMBRA_TRACE_LOG << "Received CollisionEvent: " << event;

		Entity entity1 = event.getEntity(0);
		Entity entity2 = event.getEntity(1);
		const collision::Manifold* manifold = event.getManifold();

		if ((entity1 != kNullEntity) && (entity2 != kNullEntity) && manifold) {
			auto [rb1] = mEntityDatabase.getComponents<physics::RigidBody>(entity1);
			auto [rb2] = mEntityDatabase.getComponents<physics::RigidBody>(entity2);
			if (rb1 && rb2) {
				if ((rb1->getConfig().invertedMass > 0.0f) || (rb2->getConfig().invertedMass > 0.0f)) {
					SOMBRA_DEBUG_LOG << "Handling CollisionEvent between "
						<< rb1 << " (p=" << glm::to_string(rb1->getData().position) << ", o=" << glm::to_string(rb1->getData().orientation) << ") and "
						<< rb2 << " (p=" << glm::to_string(rb2->getData().position) << ", o=" << glm::to_string(rb2->getData().orientation) << ")";

					if (manifold->state[collision::Manifold::State::Intersecting]) {
						handleIntersectingManifold(rb1, rb2, manifold);
					}
					else {
						handleDisjointManifold(manifold);
					}
				}
				else {
					SOMBRA_TRACE_LOG << "Skipping CollisionEvent between infinite mass RigidBodies " << rb1 << " and " << rb2;
				}
			}
			else {
				SOMBRA_WARN_LOG << "CollisionEvent Entities hasn't been added: " << event;
			}
		}
		else {
			SOMBRA_ERROR_LOG << "Wrong CollisionEvent data: " << event;
		}
	}


	void ConstraintsSystem::handleIntersectingManifold(
		physics::RigidBody* rb1, physics::RigidBody* rb2,
		const collision::Manifold* manifold
	) {
		ManifoldConstraints& manifoldConstraints = mManifoldConstraintsMap[manifold];

		bool updateFrictionMasses = true;
		if (manifold->contacts.size() > manifoldConstraints.size()) {
			float mu1 = rb1->getConfig().frictionCoefficient, mu2 = rb2->getConfig().frictionCoefficient;
			float mu = std::sqrt((mu1 * mu1 + mu2 * mu2) / 2.0f);
			SOMBRA_DEBUG_LOG << "Using frictionCoefficient=" << mu;

			// Increase the number of constraints up to the number of contacts
			for (std::size_t i = manifoldConstraints.size(); i < manifold->contacts.size(); ++i) {
				auto& constraints = manifoldConstraints.emplace_back(ContactConstraints{
					physics::NormalConstraint(
						{ rb1, rb2 }, kCollisionBeta, kCollisionRestitutionFactor,
						kCollisionSlopPenetration, kCollisionSlopRestitution
					),
					{
						physics::FrictionConstraint({ rb1, rb2 }, kFrictionGravityAcceleration, mu),
						physics::FrictionConstraint({ rb1, rb2 }, kFrictionGravityAcceleration, mu)
					}
				});

				mPhysicsEngine.getConstraintManager().addConstraint(&constraints.normalConstraint);
				mPhysicsEngine.getConstraintManager().addConstraint(&constraints.frictionConstraints[0]);
				mPhysicsEngine.getConstraintManager().addConstraint(&constraints.frictionConstraints[1]);

				SOMBRA_DEBUG_LOG << "Added ContactConstraint[" << i << "]";
			}
		}
		else if (manifold->contacts.size() < manifoldConstraints.size()) {
			// Decrease the number of constraints down to the number of contacts
			for (std::size_t i = manifoldConstraints.size(); i > manifold->contacts.size(); --i) {
				ContactConstraints& lastConstraints = manifoldConstraints.back();
				mPhysicsEngine.getConstraintManager().removeConstraint(&lastConstraints.normalConstraint);
				mPhysicsEngine.getConstraintManager().removeConstraint(&lastConstraints.frictionConstraints[0]);
				mPhysicsEngine.getConstraintManager().removeConstraint(&lastConstraints.frictionConstraints[1]);
				manifoldConstraints.pop_back();

				SOMBRA_DEBUG_LOG << "Removed ContactConstraint[" << i-1 << "]";
			}
		}
		else {
			updateFrictionMasses = false;
		}

		if (updateFrictionMasses) {
			// Update the friction constraint masses
			float averageMass = 2.0f / (rb1->getConfig().invertedMass + rb2->getConfig().invertedMass);
			float perContactMass = averageMass / manifoldConstraints.size();

			for (ContactConstraints& contactConstraints : manifoldConstraints) {
				contactConstraints.frictionConstraints[0].calculateConstraintBounds(perContactMass);
				contactConstraints.frictionConstraints[1].calculateConstraintBounds(perContactMass);
			}

			SOMBRA_DEBUG_LOG << "Updated FrictionConstraint masses to " << perContactMass;
		}

		// Update the constraints data
		for (std::size_t i = 0; i < manifold->contacts.size(); ++i) {
			const collision::Contact& contact = manifold->contacts[i];

			// Calculate the vectors that points from the RigidBodies center of
			// mass to their contact points
			glm::vec3 r1 = contact.worldPosition[0] - rb1->getData().position;
			glm::vec3 r2 = contact.worldPosition[1] - rb2->getData().position;

			// Calculate two tangent vectors to the Contact normal
			glm::vec3 vAxis(0.0f);
			auto absCompare = [](float f1, float f2) { return std::abs(f1) < std::abs(f2); };
			std::size_t iAxis = std::distance(&contact.normal.x, std::min_element(&contact.normal.x, &contact.normal.x + 3, absCompare));
			vAxis[iAxis] = 1.0f;

			glm::vec3 tangent1 = glm::normalize(glm::cross(contact.normal, vAxis));
			glm::vec3 tangent2 = glm::normalize(glm::cross(contact.normal, tangent1));

			manifoldConstraints[i].normalConstraint.setNormal(contact.normal);
			manifoldConstraints[i].normalConstraint.setConstraintVectors({ r1, r2 });
			manifoldConstraints[i].frictionConstraints[0].setTangent(tangent1);
			manifoldConstraints[i].frictionConstraints[0].setConstraintVectors({ r1, r2 });
			manifoldConstraints[i].frictionConstraints[1].setTangent(tangent2);
			manifoldConstraints[i].frictionConstraints[1].setConstraintVectors({ r1, r2 });

			SOMBRA_DEBUG_LOG << "Updated ContactConstraints[" << i << "]: "
				<< "r1=" << glm::to_string(r1) << ", " << "r2=" << glm::to_string(r2) << ", "
				<< "normal=" << glm::to_string(contact.normal) << ", "
				<< "tangent1=" << glm::to_string(tangent1) << " and tangent2=" << glm::to_string(tangent2);
		}
	}


	void ConstraintsSystem::handleDisjointManifold(const collision::Manifold* manifold)
	{
		auto itPair = mManifoldConstraintsMap.find(manifold);
		if (itPair != mManifoldConstraintsMap.end()) {
			for (ContactConstraints& constraints : itPair->second) {
				mPhysicsEngine.getConstraintManager().removeConstraint(&constraints.normalConstraint);
				mPhysicsEngine.getConstraintManager().removeConstraint(&constraints.frictionConstraints[0]);
				mPhysicsEngine.getConstraintManager().removeConstraint(&constraints.frictionConstraints[1]);
			}

			SOMBRA_DEBUG_LOG << "Removed all the ContactConstraints (" << itPair->second.size() << ")";
			mManifoldConstraintsMap.erase(itPair);
		}
		else {
			SOMBRA_WARN_LOG << "Doesn't exists any ContactConstraints";
		}
	}

}
