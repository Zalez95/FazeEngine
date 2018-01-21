#include "fe/collision/FineCollisionDetector.h"
#include <limits>
#include <cassert>
#include <algorithm>
#include "fe/collision/Contact.h"
#include "fe/collision/Polytope.h"
#include "fe/collision/ConvexCollider.h"

namespace fe { namespace collision {

// Static variables definition
	const float EPACollisionDetector::sMinFDifference = 0.001f;

// Public functions
	Contact EPACollisionDetector::calculate(
		const ConvexCollider& collider1, const ConvexCollider& collider2,
		Polytope& polytope
	) const
	{
		// 1. Calculate the closest face to the origin
		Triangle* closestF;
		float closestFDist;
		std::tie(closestF, closestFDist) = getClosestFaceToOrigin(collider1, collider2, polytope);

		// 2. Project the origin into the closest triangle and get its
		// barycentric coordinates
		glm::vec3 baryCoordinates = projectPointOnTriangle(
			glm::vec3(0.0f),
			{
				closestF->mAB.mP1->getCSOPosition(),
				closestF->mBC.mP1->getCSOPosition(),
				closestF->mCA.mP1->getCSOPosition()
			}
		);

		// 3. Calculate the normal, local and world coordinates of the contact
		// from the barycenter coordinates of the point
		glm::vec3 contactNormal = baryCoordinates.x * closestF->mAB.mP1->getCSOPosition()
								+ baryCoordinates.y * closestF->mBC.mP1->getCSOPosition()
								+ baryCoordinates.z * closestF->mCA.mP1->getCSOPosition();

		Contact ret(closestFDist, glm::normalize(contactNormal));
		for (unsigned int i = 0; i < 2; ++i) {
			for (unsigned int j = 0; j < 1; ++j) {
				ret.mWorldPos[i][j] = baryCoordinates.x * closestF->mAB.mP1->getWorldPosition(i)[j]
									+ baryCoordinates.y * closestF->mBC.mP1->getWorldPosition(i)[j]
									+ baryCoordinates.z * closestF->mCA.mP1->getWorldPosition(i)[j];
				ret.mLocalPos[i][j] = baryCoordinates.x * closestF->mAB.mP1->getLocalPosition(i)[j]
									+ baryCoordinates.y * closestF->mBC.mP1->getLocalPosition(i)[j]
									+ baryCoordinates.z * closestF->mCA.mP1->getLocalPosition(i)[j];
			}
		}

		return ret;
	}

// Private functions
	std::tuple<Triangle*, float> EPACollisionDetector::getClosestFaceToOrigin(
		const ConvexCollider& collider1, const ConvexCollider& collider2,
		Polytope& polytope
	) const
	{
		Triangle* closestF = nullptr;
		float closestFDist = std::numeric_limits<float>::max();
		while (true) {
			// 1. Calculate the closest face to the origin of the polytope
			std::vector<Triangle>::iterator closestF2 = polytope.mFaces.begin();
			float closestFDist2 = std::numeric_limits<float>::max();
			for (auto it = polytope.mFaces.begin(); it != polytope.mFaces.end(); ++it) {
				float fDist = it->getDistanceToOrigin();
				if (closestFDist2 > fDist) {
					closestF2		= it;
					closestFDist2	= fDist;
				}
			}

			// 2. If the difference of distance to the origin between the
			// current closest face and last one is smaller than
			// sMinFDifference we have found the closest face
			if (closestFDist - closestFDist2 <= sMinFDifference) {
				closestF		= &(*closestF2);
				closestFDist	= closestFDist2;
				break;
			}

			// 3. Add a new support point along the face normal
			polytope.mVertices.emplace_back(collider1, collider2, closestF2->mNormal);
			SupportPoint* sp = &polytope.mVertices.back();

			// 4. Remove the current closest face from the polytope
			polytope.mFaces.erase(closestF2);

			// 5. Delete the faces that can be seen from the new point and get
			// the edges of the created hole
			std::vector<Edge> holeEdges;
			for (auto it = polytope.mFaces.begin(); it != polytope.mFaces.end();) {
				if (glm::dot(it->mNormal, sp->getCSOPosition()) > 0) {
					auto itE1 = std::find(holeEdges.begin(), holeEdges.end(), it->mAB);
					if (itE1 == holeEdges.end()) holeEdges.push_back(it->mAB); else holeEdges.erase(itE1);

					auto itE2 = std::find(holeEdges.begin(), holeEdges.end(), it->mBC);
					if (itE2 == holeEdges.end()) holeEdges.push_back(it->mBC); else holeEdges.erase(itE2);

					auto itE3 = std::find(holeEdges.begin(), holeEdges.end(), it->mCA);
					if (itE3 == holeEdges.end()) holeEdges.push_back(it->mCA); else holeEdges.erase(itE3);

					it = polytope.mFaces.erase(it);
				}
				else {
					++it;
				}
			}

			// 6. Add new faces connecting the edges of the hole to the support point
			for (Edge e : holeEdges) {
				polytope.mFaces.emplace_back(sp, e.mP1, e.mP2);
			}

			closestFDist = closestFDist2;
		}

		return std::make_tuple(closestF, closestFDist);
	}


	glm::vec3 EPACollisionDetector::projectPointOnTriangle(
		const glm::vec3& point,
		const std::array<glm::vec3, 3>& triangle
	) const
	{
		glm::vec3 u = triangle[1] - triangle[0], v = triangle[2] - triangle[0],
			w = point - triangle[0], n = glm::cross(u, v);

		float gamma	= glm::dot(glm::cross(u, w), n) / glm::dot(n, n);
		float beta	= glm::dot(glm::cross(w, v), n) / glm::dot(n, n);
		float alpha	= 1 - gamma - beta;

		assert(0.0f <= gamma && gamma <= 1.0f
			&& 0.0f <= beta  && beta  <= 1.0f
			&& 0.0f <= alpha && alpha <= 1.0f
			&& "The given point can't be projected on the triangle"
		);

		return glm::vec3(gamma, beta, alpha);
	}

}}
