#include <algorithm>
#include "se/utils/Log.h"
#include "se/app/InputManager.h"
#include "se/app/Entity.h"

namespace se::app {

	InputManager::InputManager(window::WindowSystem& windowSystem) :
		mWindowSystem(windowSystem)
	{
		resetMousePosition();
	}


	void InputManager::addEntity(Entity* entity)
	{
		if (!entity) {
			SOMBRA_WARN_LOG << "Entity " << entity << " couldn't be added";
			return;
		}

		mEntities.push_back(entity);
		SOMBRA_INFO_LOG << "Entity " << entity << " added successfully";
	}


	void InputManager::removeEntity(Entity* entity)
	{
		auto itEntity = std::find(mEntities.begin(), mEntities.end(), entity);
		if (itEntity != mEntities.end()) {
			mEntities.erase(itEntity);
			SOMBRA_INFO_LOG << "Entity " << entity << " removed successfully";
		}
		else {
			SOMBRA_WARN_LOG << "Entity " << entity << " wasn't removed";
		}
	}


	void InputManager::update()
	{
		SOMBRA_INFO_LOG << "Updating the InputManager";

		// Update the entities
		const window::InputData& inputData = mWindowSystem.getInputData();
		for (Entity* entity : mEntities) {
			doMouseInput(entity, inputData);
			doKeyboardInput(entity, inputData);
		}

		resetMousePosition();

		SOMBRA_INFO_LOG << "InputManager updated";
	}

// Private functions definition
	void InputManager::doMouseInput(Entity* entity, const window::InputData& inputData) const
	{
		const window::WindowData& data = mWindowSystem.getWindowData();

		// Get the mouse movement from the center of the screen in the range [-1, 1]
		glm::vec2 mouseDelta(
			2 * inputData.mouseX / data.width - 1.0f,
			2 * inputData.mouseY / data.height - 1.0f		// note that the Y position is upsidedown
		);

		// Calculate the rotation around the Entity's y-axis
		float yaw			= kMouseSpeed * mouseDelta.x;
		glm::quat qYaw		= glm::angleAxis(yaw, glm::vec3(0, 1, 0));

		// Calculate the rotation around the Entity's x-axis
		float pitch			= kMouseSpeed * mouseDelta.y;
		glm::quat qPitch	= glm::angleAxis(pitch, glm::vec3(1, 0, 0));

		// Apply the change in orientation
		entity->orientation = glm::normalize((qPitch * qYaw) * entity->orientation);
	}


	void InputManager::doKeyboardInput(Entity* entity, const window::InputData& inputData) const
	{
		glm::vec3 forward	= glm::vec3(0, 0,-1) * entity->orientation;
		glm::vec3 up		= glm::vec3(0, 1, 0);
		glm::vec3 right		= glm::cross(forward, up);

		// Get the direction from the input in the XZ plane
		glm::vec3 direction(0.0f);
		if (inputData.keys[SE_KEY_W]) { direction += forward; }
		if (inputData.keys[SE_KEY_S]) { direction -= forward; }
		if (inputData.keys[SE_KEY_D]) { direction += right; }
		if (inputData.keys[SE_KEY_A]) { direction -= right; }

		// Normalize the direction
		float length = glm::length(direction);
		if (length > 0.0f) { direction /= length; }

		// Transform the direction to velocity
		float velocityDiff = kRunSpeed - glm::length(entity->velocity);
		if (velocityDiff > 0.0f) {
			entity->velocity += velocityDiff * direction;
		}

		// Add the jump velocity
		if (inputData.keys[SE_KEY_SPACE]) { entity->velocity += kJumpSpeed * up; }
		if (inputData.keys[SE_KEY_LEFT_CONTROL]) { entity->velocity -= kJumpSpeed * up; }
	}


	void InputManager::resetMousePosition() const
	{
		const window::WindowData& data = mWindowSystem.getWindowData();
		mWindowSystem.setMousePosition(data.width / 2.0, data.height / 2.0);
	}

}
