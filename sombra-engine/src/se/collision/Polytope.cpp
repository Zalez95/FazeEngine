#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "se/collision/HalfEdgeMeshExt.h"
#include "Polytope.h"
#include "Geometry.h"

namespace se::collision {

	Polytope::Polytope(const std::array<SupportPoint, 4>& simplex, float precision) :
		mPrecision(precision)
	{
		// Add the HEVertices
		std::vector<int> vertexIndices;
		vertexIndices.push_back( addVertex(simplex[0]) );
		vertexIndices.push_back( addVertex(simplex[1]) );
		vertexIndices.push_back( addVertex(simplex[2]) );
		vertexIndices.push_back( addVertex(simplex[3]) );

		const glm::vec3 p0CSO = mMesh.vertices[vertexIndices[0]].location;
		const glm::vec3 p1CSO = mMesh.vertices[vertexIndices[1]].location;
		const glm::vec3 p2CSO = mMesh.vertices[vertexIndices[2]].location;
		const glm::vec3 p3CSO = mMesh.vertices[vertexIndices[3]].location;

		// Add the HEFaces with the correct normal winding order
		const glm::vec3 tNormal = glm::cross(p1CSO - p0CSO, p2CSO - p0CSO);
		if (glm::dot(p3CSO - p0CSO, tNormal) <= 0.0f) {
			addFace({ vertexIndices[0], vertexIndices[1], vertexIndices[2] });
			addFace({ vertexIndices[0], vertexIndices[3], vertexIndices[1] });
			addFace({ vertexIndices[0], vertexIndices[2], vertexIndices[3] });
			addFace({ vertexIndices[1], vertexIndices[3], vertexIndices[2] });
		}
		else {
			addFace({ vertexIndices[0], vertexIndices[2], vertexIndices[1] });
			addFace({ vertexIndices[0], vertexIndices[1], vertexIndices[3] });
			addFace({ vertexIndices[0], vertexIndices[3], vertexIndices[2] });
			addFace({ vertexIndices[1], vertexIndices[2], vertexIndices[3] });
		}
	}


	int Polytope::addVertex(const SupportPoint& sp)
	{
		int iVertex = se::collision::addVertex(mMesh, sp.getCSOPosition());
		mVertexSupportPoints.emplace(iVertex, sp);

		return iVertex;
	}


	int Polytope::addFace(const std::array<int, 3>& faceIndices)
	{
		int iFace = se::collision::addFace(mMesh, { faceIndices[0], faceIndices[1], faceIndices[2] });

		// Add the normal of the HEFace to mFaceNormals
		mFaceNormals[iFace] = calculateFaceNormal(mMesh, iFace);

		// Add the distance data of the HEFace to mFaceDistances
		const glm::vec3 p0CSO = mMesh.vertices[faceIndices[0]].location;
		const glm::vec3 p1CSO = mMesh.vertices[faceIndices[1]].location;
		const glm::vec3 p2CSO = mMesh.vertices[faceIndices[2]].location;
		glm::vec3 closestPoint = getClosestPointInPlane(glm::vec3(0.0f), { p0CSO, p1CSO, p2CSO });
		float distance = glm::length(closestPoint);
		glm::vec3 closestPointBarycentricCoords;
		bool inside = projectPointOnTriangle(
			closestPoint, { p0CSO, p1CSO, p2CSO },
			mPrecision, closestPointBarycentricCoords
		);
		mFaceDistances[iFace] = { closestPoint, distance, inside, closestPointBarycentricCoords };

		return iFace;
	}


	void Polytope::removeFace(int iFace)
	{
		se::collision::removeFace(mMesh, iFace);
		mFaceNormals.erase(iFace);
		mFaceDistances.erase(iFace);
	}

}