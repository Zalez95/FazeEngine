#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <array>
#include <cassert>

namespace fe { namespace physics {

	class RigidBody;
	class ConstraintBounds;


	/**
	 * Class Constraint, TODO
	 *
	 * @note	the constraints can only restrict the movement between two
	 *			RigidBodies
	 */
	class Constraint
	{
	protected:	// Attributes
		/** A pointer to the bounds of the Constraint */
		const ConstraintBounds* mConstraintBounds;

		/** The two RigidBodies affected by the constraint */
		std::array<RigidBody*, 2> mRigidBodies;

	public:		// Functions
		/** Creates a new Constraint
		 *
		 * @param	constraintBounds a pointer to the data of the Constraint
		 * @param	rigidBodies the two rigidBodies affected by the
		 *			Constraint */
		Constraint(
			const ConstraintBounds* constraintBounds,
			const std::array<RigidBody*, 2>& rigidBodies
		) : mConstraintBounds(constraintBounds),
			mRigidBodies(rigidBodies) {};

		/** Class destructor */
		~Constraint() {};

		/** @return the a pointer to the ConstraintBounds of the Constraint */
		inline const ConstraintBounds* getConstraintBounds() const
		{ return mConstraintBounds; };

		/** Returns the specified RigidBody
		 *
		 * @param	rb the index of the RigidBody (0 or 1)
		 * @return	a pointer the the RigidBody */
		inline RigidBody* getRigidBody(size_t rb) const
		{
			assert(rb < 2 && "There are only two RigidBodies");
			return mRigidBodies[rb];
		};

		/** @return the value of the Bias of the constraint */
		virtual float getBias() const = 0;

		/** @return the Jacobian matrix of the constraint */
		virtual std::array<float, 12> getJacobianMatrix() const = 0;
	};

}}

#endif		// CONSTRAINT_H