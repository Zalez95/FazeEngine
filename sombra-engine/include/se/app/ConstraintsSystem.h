#ifndef CONSTRAINTS_SYSTEM_H
#define CONSTRAINTS_SYSTEM_H

#include <map>
#include <memory>
#include "../physics/PhysicsEngine.h"
#include "../physics/constraints/NormalConstraint.h"
#include "../physics/constraints/FrictionConstraint.h"
#include "../collision/Manifold.h"
#include "../utils/FixedVector.h"
#include "events/EventManager.h"
#include "ISystem.h"

namespace se::app {

	class CollisionEvent;


	/**
	 * Class ConstraintsSystem, it's a System used for updating the Entities'
	 * physics data with the constraints changes
	 */
	class ConstraintsSystem : public ISystem
	{
	private:	// Nested types
		/** Holds all the Constraints per Contact */
		struct ContactConstraints
		{
			physics::NormalConstraint normalConstraint;
			physics::FrictionConstraint frictionConstraints[2];
		};

		using ManifoldConstraints = utils::FixedVector<
			ContactConstraints, collision::Manifold::kMaxContacts
		>;

	private:	// Attributes
		/** The velocity of the constraint resolution process of the
		 * NormalConstraints */
		static constexpr float kCollisionBeta = 0.1f;

		/** The restitution factor of all the NormalConstraints */
		static constexpr float kCollisionRestitutionFactor = 0.2f;

		/** The slop penetration value of all the NormalConstraints */
		static constexpr float kCollisionSlopPenetration = 0.005f;

		/** The slop restitution value of all the NormalConstraints */
		static constexpr float kCollisionSlopRestitution = 0.5f;

		/** The gravity acceleration value of all the FrictionConstraints */
		static constexpr float kFrictionGravityAcceleration = 9.8f;

		/** The Engine used for updating the data of the PhysicsEntities */
		physics::PhysicsEngine& mPhysicsEngine;

		/** The EventManager that will notify the events */
		EventManager& mEventManager;

		/** The NormalConstraints generated as a consecuence of the
		 * PhysicsEntities collisions */
		std::map<const collision::Manifold*, ManifoldConstraints>
			mManifoldConstraintsMap;

	public:		// Functions
		/** Creates a new ConstraintsSystem
		 *
		 * @param	entityDatabase the EntityDatabase that holds all the
		 *			Entities
		 * @param	physicsEngine a reference to the PhysicsEngine used
		 *			by the ConstraintsSystem
		 * @param	eventManager a reference to the EventManager that the
		 *			ConstraintsSystem will be subscribed to */
		ConstraintsSystem(
			EntityDatabase& entityDatabase,
			physics::PhysicsEngine& physicsEngine,
			EventManager& eventManager
		);

		/** Class destructor */
		~ConstraintsSystem();

		/** Notifies the ConstraintsSystem of the given event
		 *
		 * @param	event the IEvent to notify */
		virtual void notify(const IEvent& event) override;

		/** Solves the Constraints between the RigidBodies of the entities
		 *
		 * @param	delta the elapsed time since the last update in seconds */
		virtual void update() override;
	private:
		/** Handles the given CollisionEvent by creating ContactConstraints
		 * between the colliding Entities
		 *
		 * @param	event the CollisionEvent to handle */
		void onCollisionEvent(const CollisionEvent& event);

		/** Adds contact constraints to the ConstraintsSystem for resolving the
		 * collision detected in the given contact Manifold
		 *
		 * @param	rb1 a pointer to the first RigidBody of the Constraints
		 * @param	rb1 a pointer to the second RigidBody of the Constraints
		 * @param	manifold a pointer to the contact Manifold */
		void handleIntersectingManifold(
			physics::RigidBody* rb1, physics::RigidBody* rb2,
			const collision::Manifold* manifold
		);

		/** Removes all the Manifold contact constraints from the ConstraintsSystem
		 *
		 * @param	manifold a pointer to the contact Manifold */
		void handleDisjointManifold(const collision::Manifold* manifold);
	};

}

#endif	// CONSTRAINTS_SYSTEM_H
