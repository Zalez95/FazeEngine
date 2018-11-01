#include "fe/physics/constraints/NormalConstraint.h"
#include "fe/physics/RigidBody.h"

namespace fe { namespace physics {

	constexpr ConstraintBounds NormalConstraint::kConstraintBounds;


	NormalConstraint::NormalConstraint(const std::array<RigidBody*, 2>& rigidBodies, float beta) :
		Constraint(&kConstraintBounds, rigidBodies),
		mNormal(0.0f), mBeta(beta), mDeltaTime(0.0f), mK(0.0f) {}


	float NormalConstraint::getBias() const
	{
		glm::vec3 p1 = mRigidBodies[0]->position + mConstraintPoints[0];
		glm::vec3 p2 = mRigidBodies[1]->position + mConstraintPoints[1];
		float positionConstraint = glm::dot((p2 - p1), mNormal);

		return glm::pow(1 - mDeltaTime * mBeta, mK) * positionConstraint;
	}


	std::array<float, 12> NormalConstraint::getJacobianMatrix() const
	{
		glm::vec3 r1xn = glm::cross(mConstraintPoints[0], mNormal);
		glm::vec3 r2xn = glm::cross(mConstraintPoints[1], mNormal);

		return {
			-mNormal.x, -mNormal.y, -mNormal.z,
			-r1xn.x, -r1xn.y, -r1xn.z,
			mNormal.x, mNormal.y, mNormal.z,
			r2xn.x, r2xn.y, r2xn.z
		};
	}

}}
