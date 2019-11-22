#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace se::utils {

	/** Calculates the pow with the given numbers
	 *
	 * @param	base the base of the pow function
	 * @param	exponent the exponent of the pow function
	 * @return	the value of the base raised to the exponent power */
	template <typename T>
	constexpr T ipow(T base, unsigned int exponent)
	{ return (exponent == 0)? 1 : base * ipow(base, exponent - 1); }


	/** Calculates the closest point to p in the given edge
	 *
	 * @param	p the point
	 * @param	e1 the first point of the edge
	 * @param	e2 the second point of the edge
	 * @return	the closest point in the plane to the given point p */
	glm::vec3 getClosestPointInEdge(
		const glm::vec3& p,
		const glm::vec3& e1, const glm::vec3& e2
	);


	/** Calculates the closest point to p in the given plane
	 *
	 * @param	p the point
	 * @param	planePoints three of the points of the plane
	 * @return	the closest point in the plane to the given point p */
	glm::vec3 getClosestPointInPlane(
		const glm::vec3& p,
		const std::array<glm::vec3, 3>& planePoints
	);


	/** Calculates the intersection between the given ray and the given plane
	 *
	 * @param	origin the origin of the ray
	 * @param	direction the direction of the ray
	 * @param	planePoint a point in the plane
	 * @param	planeNormal the normal vector of the plane
	 * @param	intersectionPrecision the precision of the intersection between
	 *			the ray and the plane
	 * @return	a pair with a flag that tells if the point could be projected
	 *			and the 3D coordinates of the projected point */
	std::pair<bool, glm::vec3> rayPlaneIntersection(
		const glm::vec3& origin, const glm::vec3& direction,
		const glm::vec3& planePoint, const glm::vec3& planeNormal,
		float intersectionPrecision
	);


	/** Projects the given point onto the the given 3D triangle
	 *
	 * @param	point the 3D coordinates of the point in world space
	 * @param	triangle an array with the 3 points of the triangle in
	 *			world space
	 * @param	projectionPrecision the precision of the projected point onto
	 *			the triangle
	 * @param	projectedPoint a reference to the vector where we are going
	 *			to store the coordinates of the projected point in
	 *			barycentric coordinates
	 * @return	true if the point could be projected onto the triangle,
	 *			false otherwise */
	bool projectPointOnTriangle(
		const glm::vec3& point, const std::array<glm::vec3, 3>& triangle,
		float projectionPrecision, glm::vec3& projectedPoint
	);


	/** Calculates the area of the given triangle
	 *
	 * @param	triangle an array with the 3 points of the triangle in 3D
	 * @return	the area of the triangle */
	float calculateTriangleArea(const std::array<glm::vec3, 3>& triangle);


	/** Decomposes the given transforms matrix into a translation vector, a
	 * quaternion orientation and a scale vector
	 *
	 * @param	transforms the original transformations matrix
	 * @param	translation the vector where the translation will be stored
	 * @param	rotation the quaternion where the rotation will be stored
	 * @param	scale the vector where the scale will be stored */
	void decompose(
		const glm::mat4& transforms,
		glm::vec3& translation, glm::quat& rotation, glm::vec3& scale
	);

}

#endif		// MATH_UTILS_H
