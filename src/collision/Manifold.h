#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <vector>
#include "Contact.h"

namespace collision {

	class Collider;


	/**
	 * Class Manifold, a contact manifold holds all the Contacs between two
	 * Colliders
	 */
	class Manifold
	{
	private:	// Attributes
		friend class FineCollisionDetector;

		/** All the Contacs the the Contact Manifold can hold */
		std::vector<Contact> mContacts;

		/** The first collider of the Manifold */
		const Collider* mFirstCollider;

		/** The second collider of the Manifold */
		const Collider* mSecondCollider;

	public:		// Functions
		/** Creates a new Manifold
		 *
		 * @param	c1 a pointer to the first Collider of the Manifold
		 * @param	c2 a pointer to the second Collider of the Manifold */
		Manifold(const Collider* c1, const Collider* c2) :
			mFirstCollider(c1), mSecondCollider(c2) {};
		
		/** Class destructor */
		~Manifold() {};

		/** @return	all the Contacs that the Manifold holds */
		inline std::vector<Contact> getContacts() const
		{ return mContacts; };

		/** @return a pointer to the first Collider of the Manifold */
		inline const Collider* getFirstCollider() const
		{ return mFirstCollider; };

		/** @return a pointer to the second Collider of the Manifold */
		inline const Collider* getSecondCollider() const
		{ return mSecondCollider; };
	};

}

#endif		// MANIFOLD_H
