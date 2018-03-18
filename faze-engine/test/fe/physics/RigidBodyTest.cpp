#include <gtest/gtest.h>
#include <fe/physics/RigidBody.h>

#define TOLERANCE 0.000000001


TEST(RigidBody, linearVelocityIntegration)
{
	fe::physics::RigidBody rb(3.5f, 1.0f, glm::mat3(2.0f / 5.0f * 3.5f * pow(2.0f, 2)), 1.0f);
	rb.setPosition(glm::vec3(-3.0f, 2.0f, -5.0f));
	rb.setLinearVelocity(glm::vec3(2.5f, -2.0f, -0.5f));
	rb.updateData();

	rb.integrate(0.016f);

	const glm::vec3 expectedPosition(-2.96f, 1.968f, -5.008f);
	for (size_t i = 0; i < 3; ++i) {
		EXPECT_LE(abs(rb.getPosition()[i] - expectedPosition[i]), TOLERANCE);
	}
}


TEST(RigidBody, angularVelocityIntegration)
{
	fe::physics::RigidBody rb(3.5f, 1.0f, glm::mat3(2.0f / 5.0f * 3.5f * pow(2.0f, 2)), 1.0f);
	rb.setOrientation(glm::quat(0.020926f, 0.841695f, 0.296882f, -0.450525f));
	rb.setAngularVelocity(glm::vec3(-0.13f, -3.6f, 10.125f));
	rb.updateData();

	rb.integrate(0.016f);

	const glm::quat expectedOrientation(0.02045356904f, 0.8415004242f, 0.2980528874f, -0.4500772163f);
	for (size_t i = 0; i < 4; ++i) {
		EXPECT_LE(abs(rb.getOrientation()[i] - expectedOrientation[i]), TOLERANCE);
	}
}


TEST(RigidBody, addForce)
{
	fe::physics::RigidBody rb(4.512f, 1.0f, glm::mat3(2.0f / 5.0f * 4.512f * pow(2.0f, 2)), 1.0f);
	rb.setPosition(glm::vec3(-3.146f, 2.95f, -5.2f));
	rb.setLinearVelocity(glm::vec3(-0.13f, -3.6f, 10.125f));
	rb.updateData();
	rb.addForce(glm::vec3(8.11f, -10.31f, -6.8124f));

	rb.integrate(0.016f);

	const glm::vec3 expectedPosition(-3.147619724f, 2.891815185f, -5.038386344f);
	const glm::vec3 expectedLinearVelocity(-0.101241126f, -3.636560201f, 10.100842475f);
	const glm::vec3 expectedLinearAcceleration(1.797428965f, -2.285017728f, -1.509840369f);
	for (size_t i = 0; i < 3; ++i) {
		EXPECT_LE(abs(rb.getPosition()[i] - expectedPosition[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getLinearVelocity()[i] - expectedLinearVelocity[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getLinearAcceleration()[i] - expectedLinearAcceleration[i]), TOLERANCE);
	}
}


TEST(RigidBody, addForceAtPoint)
{
	fe::physics::RigidBody rb(3.953f, 1.0f, glm::mat3(2.0f / 5.0f * 3.953f * pow(2.0f, 2)), 1.0f);
	rb.setPosition(glm::vec3(5.373533248f, -5.649199485f, 3.746687889f));
	rb.setOrientation(glm::quat(0.812893509f, 0.441731840f, -0.347656339f, 0.152355521f));
	rb.setLinearVelocity(glm::vec3(-0.13f, -3.6f, 10.125f));
	rb.setAngularVelocity(glm::vec3(0.965f, -2.0154f, -7.849f));
	rb.updateData();
	rb.addForceAtPoint( glm::vec3(6.541f, -12.451f, 1.568f), glm::vec3(5.94522f, -4.89395f, 3.14315f) );

	rb.integrate(0.016f);

	const glm::vec3 expectedPosition(5.371876716f, -5.707605838f, 3.908789396f);
	const glm::quat expectedOrientation(0.811595022f, 0.423024982f, -0.389173269f, 0.104439236f);
	const glm::vec3 expectedLinearVelocity(-0.103524908f, -3.650396108f, 10.131346702f);
	const glm::vec3 expectedAngularVelocity(1.044589281f, -1.986972808f, -7.955280303f);
	const glm::vec3 expectedLinearAcceleration(1.654692649f, -3.149759769f, 0.396660745f);
	const glm::vec3 expectedAngularAcceleration(4.974330902f, 1.776695132f, -6.642519950f);
	for (size_t i = 0; i < 3; ++i) {
		EXPECT_LE(abs(rb.getPosition()[i] - expectedPosition[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getLinearVelocity()[i] - expectedLinearVelocity[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getAngularVelocity()[i] - expectedAngularVelocity[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getLinearAcceleration()[i] - expectedLinearAcceleration[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getAngularAcceleration()[i] - expectedAngularAcceleration[i]), TOLERANCE);
	}
	for (size_t i = 0; i < 4; ++i) {
		EXPECT_LE(abs(rb.getOrientation()[i] - expectedOrientation[i]), TOLERANCE);
	}
}


TEST(RigidBody, addForceAtLocalPoint)
{
	fe::physics::RigidBody rb(6.1781f, 1.0f, glm::mat3(6.1781f * pow(3.21f, 2) / 6.0f), 1.0f);
	rb.setPosition(glm::vec3(-4.547531127f, 2.949749708f, -3.636348962f));
	rb.setOrientation(glm::quat(-0.074506878f, -0.676165580f, -0.448467493f, -0.579763472f));
	rb.setLinearVelocity(glm::vec3(0.1237f, 4.12248f, -5.9655f));
	rb.setAngularVelocity(glm::vec3(-1.23759f, -5.3746f, 0.4093f));
	rb.updateData();
	rb.addForceAtLocalPoint( glm::vec3(7.341f, 4.6405f, -3.19709f), glm::vec3(-0.884065f, 1.0f, 0.278097f) );

	rb.integrate(1.56f);

	const glm::vec3 expectedPosition(-1.462884187f, 11.208745956f, -14.201886177f);
	const glm::quat expectedOrientation(0.004820846f, -0.720890223f, -0.487551778f, -0.492531448f);
	const glm::vec3 expectedLinearVelocity(1.977337837f, 5.294228553f, -6.772780418f);
	const glm::vec3 expectedAngularVelocity(0.434943199f, -11.159923553f, -4.147571086f);
	const glm::vec3 expectedLinearAcceleration(1.188229441f, 0.751120924f, -0.517487585f);
	const glm::vec3 expectedAngularAcceleration(1.072136640f, -3.708540678f, -2.921071290f);
	const glm::mat4 expectedTransformsMatrix(
		glm::vec4(0.039412070f, 0.698193788f, 0.714823007f, 0.0f),
		glm::vec4(0.707691431f, -0.524539887f, 0.473318576f, 0.0f),
		glm::vec4(0.705421388f, 0.487219780f, -0.514778912f, 0.0f),
		glm::vec4(-1.462884187f, 11.208745956f, -14.201886177f, 1.0f)
	);

	for (size_t i = 0; i < 3; ++i) {
		EXPECT_LE(abs(rb.getPosition()[i] - expectedPosition[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getLinearVelocity()[i] - expectedLinearVelocity[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getAngularVelocity()[i] - expectedAngularVelocity[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getLinearAcceleration()[i] - expectedLinearAcceleration[i]), TOLERANCE);
		EXPECT_LE(abs(rb.getAngularAcceleration()[i] - expectedAngularAcceleration[i]), TOLERANCE);
	}
	for (size_t i = 0; i < 4; ++i) {
		EXPECT_LE(abs(rb.getOrientation()[i] - expectedOrientation[i]), TOLERANCE);
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_LE(abs(rb.getTransformsMatrix()[i][j] - expectedTransformsMatrix[i][j]), TOLERANCE);
		}
	}

}