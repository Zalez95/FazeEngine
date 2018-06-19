#include <gtest/gtest.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fe/collision/ConvexPolyhedron.h>
/*
#define TOLERANCE 0.000001f

TEST(ConvexPolyhedron, getAABB)
{
	const std::vector<glm::vec3> vertices = {
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.5f, 0.5f, 1.6f }
	};
	const glm::vec3 expectedMinimum(0.0f, 0.0f, 0.0f);
	const glm::vec3 expectedMaximum(1.0, 1.0, 1.6f);

	fe::collision::ConvexPolyhedron cp1(vertices);
	fe::collision::AABB aabb1 = cp1.getAABB();
	for (int i = 0; i < 3; ++i) {
		EXPECT_NEAR(aabb1.minimum[i], expectedMinimum[i], TOLERANCE);
		EXPECT_NEAR(aabb1.maximum[i], expectedMaximum[i], TOLERANCE);
	}
}


TEST(ConvexPolyhedron, getAABBTransforms)
{
	const glm::vec3 translation(5.0f, -1.0f, -10.0f);
	const glm::quat rotation = glm::angleAxis(glm::pi<float>()/3, glm::vec3(2/3.0f, -2/3.0f, 1/3.0f));
	const std::vector<glm::vec3> vertices = {
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.5f, 0.5f, 1.6f }
	};
	const glm::vec3 expectedMinimum(4.35967969f, -1.70720076f, -10.0f);
	const glm::vec3 expectedMaximum(5.72222232f, -0.21132487f, -8.53376102f);

	fe::collision::ConvexPolyhedron cp1(vertices);
	glm::mat4 r = glm::mat4_cast(rotation);
	glm::mat4 t = glm::translate(glm::mat4(1.0f), translation);
	cp1.setTransforms(t * r);

	fe::collision::AABB aabb1 = cp1.getAABB();
	for (int i = 0; i < 3; ++i) {
		EXPECT_NEAR(aabb1.minimum[i], expectedMinimum[i], TOLERANCE);
		EXPECT_NEAR(aabb1.maximum[i], expectedMaximum[i], TOLERANCE);
	}
}


TEST(ConvexPolyhedron, getFurthestPointInDirection)
{
	const glm::vec3 translation(5.0f, -1.0f, -10.0f);
	const glm::quat rotation = glm::angleAxis(glm::pi<float>()/3, glm::vec3(2/3.0f, -2/3.0f, 1/3.0f));
	const std::vector<glm::vec3> vertices = {
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.5f, 0.5f, 1.6f }
	};
	const glm::vec3 expectedPWorld(4.48910236f, -0.27777779f, -9.53376102f);
	const glm::vec3 expectedPLocal(0.0, 1.0f, 0.0f);

	fe::collision::ConvexPolyhedron cp1(vertices);
	glm::mat4 r = glm::mat4_cast(rotation);
	glm::mat4 t = glm::translate(glm::mat4(1.0f), translation);
	cp1.setTransforms(t * r);

	glm::vec3 direction(-0.565685425f, 0.707106781f, 0.424264069f), pointWorld, pointLocal;
	cp1.getFurthestPointInDirection(direction, pointWorld, pointLocal);

	for (int i = 0; i < 3; ++i) {
		EXPECT_NEAR(pointWorld[i], expectedPWorld[i], TOLERANCE);
		EXPECT_NEAR(pointLocal[i], expectedPLocal[i], TOLERANCE);
	}
}
*/