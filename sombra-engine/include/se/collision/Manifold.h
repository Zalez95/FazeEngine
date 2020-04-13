#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <bitset>
#include "Contact.h"
#include "../utils/FixedVector.h"
#include "../utils/Repository.h"

namespace se::collision {

	class Collider;

	/**
	 * Struct Manifold, a contact manifold that holds all the Contacs between
	 * two Colliders. A Manifold can store up to 4 contacts.
	 */
	struct Manifold
	{
		using Repository = utils::Repository<Manifold, unsigned short>;

		/** The different states in which a Manifold can be */
		enum State : int
		{
			Intersecting = 0,	///< The Manifold Colliders are intersecting
			Updated,			///< Manifold updated by the CollisionWorld
			Count				///< The number of States
		};

		/** The maximum number of Contacts in the Manifold */
		static constexpr std::size_t kMaxContacts = 4;

		/** The current state of the Manifold */
		std::bitset<State::Count> state;

		/** The colliders of the Manifold */
		const Collider* colliders[2];

		/** All the Contacs the Contact Manifold can hold */
		utils::FixedVector<Contact, kMaxContacts> contacts;

		/** Creates a new Manifold
		 *
		 * @param	c1 a pointer to the first Collider of the Manifold
		 * @param	c2 a pointer to the second Collider of the Manifold */
		Manifold(const Collider* c1, const Collider* c2) :
			colliders{ c1, c2 } {};
	};

}

#endif		// MANIFOLD_H
