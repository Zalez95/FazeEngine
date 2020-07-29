#include "se/utils/Log.h"
#include "se/audio/Source.h"
#include "se/app/AudioSystem.h"
#include "se/app/EntityDatabase.h"
#include "se/app/TransformsComponent.h"

namespace se::app {

	void AudioSystem::onNewEntity(Entity entity)
	{
		auto [transforms, source] = mEntityDatabase.getComponents<TransformsComponent, audio::Source>(entity);
		if (!transforms || !source) {
			SOMBRA_WARN_LOG << "Entity " << entity << " couldn't be added as Source";
			return;
		}

		// The Source initial data is overridden by the entity one
		source->setPosition(transforms->position);
		source->setOrientation(glm::vec3(0.0f, 0.0f, 1.0f) * transforms->orientation, glm::vec3(0.0f, 1.0f, 0.0));
		source->setVelocity(transforms->velocity);
		SOMBRA_INFO_LOG << "Entity " << entity << " with Source " << source << " added successfully";
	}


	void AudioSystem::onRemoveEntity(Entity entity)
	{
		if (mListener == entity) {
			mListener = kNullEntity;
			SOMBRA_INFO_LOG << "Listener Entity " << entity << " removed successfully";
		}
	}


	void AudioSystem::setListener(Entity entity)
	{
		auto [transforms] = mEntityDatabase.getComponents<TransformsComponent>(entity);
		if (!transforms) {
			SOMBRA_WARN_LOG << "Entity " << entity << " couldn't be setted as Listener";
			return;
		}

		// The Listener initial data is overridden by the entity one
		mListener = entity;
		mAudioEngine.setListenerPosition(transforms->position);
		mAudioEngine.setListenerOrientation(glm::vec3(0.0f, 0.0f, 1.0f) * transforms->orientation, glm::vec3(0.0f, 1.0f, 0.0));
		mAudioEngine.setListenerVelocity(transforms->velocity);
		SOMBRA_INFO_LOG << "Entity " << entity << " was setted as Listener";
	}


	void AudioSystem::update()
	{
		SOMBRA_INFO_LOG << "Updating the AudioSystem";

		SOMBRA_DEBUG_LOG << "Updating the Listener";
		if (mListener != kNullEntity) {
			auto [transforms] = mEntityDatabase.getComponents<TransformsComponent>(mListener);
			if (transforms && transforms->updated.any()) {
				mAudioEngine.setListenerPosition(transforms->position);
				mAudioEngine.setListenerOrientation(
					glm::vec3(0.0f, 0.0f, 1.0f) * transforms->orientation,
					glm::vec3(0.0f, 1.0f, 0.0)
				);
				mAudioEngine.setListenerVelocity(transforms->velocity);
			}
		}

		SOMBRA_DEBUG_LOG << "Updating the Sources";
		mEntityDatabase.iterateComponents<TransformsComponent, audio::Source>(
			[this](Entity, TransformsComponent* transforms, audio::Source* source) {
				if (transforms->updated.any()) {
					source->setPosition(transforms->position);
					source->setOrientation(
						glm::vec3(0.0f, 0.0f, 1.0f) * transforms->orientation,
						glm::vec3(0.0f, 1.0f, 0.0)
					);
					source->setVelocity(transforms->velocity);
				}
			}
		);

		SOMBRA_INFO_LOG << "AudioSystem updated";
	}

}
