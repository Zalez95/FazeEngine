#include <algorithm>
#include "se/collision/CollisionWorld.h"

namespace se::collision {

	CollisionWorld::CollisionWorld() :
		mFineCollisionDetector(kMinFDifference, kContactPrecision, kContactSeparation) {}


	void CollisionWorld::addCollider(const Collider* collider)
	{
		if (!collider) { return; }

		mColliders.push_back(collider);
	}


	void CollisionWorld::removeCollider(const Collider* collider)
	{
		mColliders.erase(
			std::remove(mColliders.begin(), mColliders.end(), collider),
			mColliders.end()
		);
	}


	void CollisionWorld::update()
	{
		// Clean old non intersecting Manifolds
		for (Manifold* manifold : mManifolds) {
			if (manifold->state == ManifoldState::Disjoint) {
				auto itPair = mMapCollidersManifolds.find(std::make_pair(manifold->colliders[0], manifold->colliders[1]));
				if (itPair != mMapCollidersManifolds.end()) {
					mMapCollidersManifolds.erase(itPair);
				}
			}
		}

		// Broad collision phase
		for (const Collider* collider : mColliders) {
			mCoarseCollisionDetector.submit(collider);
		}
		auto intersectingColliders = mCoarseCollisionDetector.getIntersectingColliders();

		// Narrow collision phase
		mManifolds.clear();
		for (ColliderPair& pair : intersectingColliders) {
			// Find a Manifold between the given colliders
			auto itPairManifold = mMapCollidersManifolds.find(std::make_pair(pair.first, pair.second));
			if (itPairManifold == mMapCollidersManifolds.end()) {
				itPairManifold = mMapCollidersManifolds.find(std::make_pair(pair.second, pair.first));
			}

			if (itPairManifold != mMapCollidersManifolds.end()) {
				// Update the Manifold data
				mFineCollisionDetector.collide(itPairManifold->second);
				mManifolds.push_back(&itPairManifold->second);
			}
			else {
				// Create a new Manifold
				Manifold manifold(pair.first, pair.second);
				if (mFineCollisionDetector.collide(manifold)) {
					itPairManifold = mMapCollidersManifolds.emplace(
						std::piecewise_construct,
						std::forward_as_tuple(pair.first, pair.second),
						std::forward_as_tuple(manifold)
					).first;
					mManifolds.push_back(&itPairManifold->second);
				}
			}
		}
	}

}