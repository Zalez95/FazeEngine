#include "se/collision/CoarseCollisionDetector.h"
#include "se/collision/Collider.h"

namespace se::collision {

	void CoarseCollisionDetector::submit(const Collider* collider)
	{
		mColliders.push_back(collider);
		mAABBs.push_back(collider->getAABB());
	}


	std::set<CoarseCollisionDetector::ColliderPair> CoarseCollisionDetector::getIntersectingColliders()
	{
		std::set<ColliderPair> ret;

		while (!mAABBs.empty()) {
			for (std::size_t i = 1; i < mAABBs.size(); ++i) {
				if ( overlaps(mAABBs.front(), mAABBs[i]) ) {
					ret.insert(std::make_pair(mColliders.front(), mColliders[i]));
				}
			}

			mAABBs.pop_front();
			mColliders.pop_front();
		}

		return ret;
	}

}