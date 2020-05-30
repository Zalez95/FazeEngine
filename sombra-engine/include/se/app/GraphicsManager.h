#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <map>
#include "events/EventManager.h"
#include "../utils/Repository.h"
#include "../graphics/GraphicsEngine.h"
#include "../graphics/core/UniformBlock.h"
#include "../graphics/core/UniformBuffer.h"
#include "../graphics/core/UniformVariable.h"
#include "../graphics/3D/RenderableMesh.h"
#include "../graphics/3D/RenderableTerrain.h"
#include "Entity.h"
#include "graphics/Skin.h"
#include "graphics/Camera.h"
#include "graphics/Lights.h"

namespace se::graphics {
	struct Font;
	class Texture;
	class Program;
	class Pass;
	class Technique;
}

namespace se::app {

	class ResizeEvent;


	/**
	 * Class GraphicsManager, it's a Manager used for storing, updating and
	 * rendering the Entities' graphics data
	 */
	class GraphicsManager : public IEventListener
	{
	private:	// Nested types
		friend class TechniqueLoader;

		using FontRepository =
			utils::Repository<std::string, graphics::Font>;
		using TextureRepository =
			utils::Repository<std::string, graphics::Texture>;
		using ProgramRepository =
			utils::Repository<std::string, graphics::Program>;
		using TechniqueRepository =
			utils::Repository<std::string, graphics::Technique>;

		using CameraUPtr = std::unique_ptr<Camera>;
		using LightUPtr = std::unique_ptr<ILight>;
		using SkinSPtr = std::shared_ptr<Skin>;
		using PassSPtr = std::shared_ptr<graphics::Pass>;
		using ProgramSPtr = std::shared_ptr<graphics::Program>;
		using RenderableMeshUPtr =
			std::unique_ptr<graphics::RenderableMesh>;
		using RenderableTerrainUPtr =
			std::unique_ptr<graphics::RenderableTerrain>;

		struct Impl;

	private:	// Attributes
		/** The maximum number of joints in the program */
		static constexpr unsigned int kMaxJoints = 64;

		/** The maximum number of lights in the program */
		static constexpr unsigned int kMaxLights = 4;

		/** The GraphicsEngine used for rendering the data of the Entities */
		graphics::GraphicsEngine& mGraphicsEngine;

		/** The EventManager that will notify the events */
		EventManager& mEventManager;

		/** The repository that holds all the Fonts of the
		 * GraphicsManager */
		FontRepository mFontRepository;

		/** The repository that holds all the Textures of the
		 * GraphicsManager */
		TextureRepository mTextureRepository;

		/** The repository that holds all the Programs of the
		 * GraphicsManager */
		ProgramRepository mProgramRepository;

		/** The repository that holds all the Techniques of the
		 * GraphicsManager */
		TechniqueRepository mTechniqueRepository;

		/** Holds all the implementation details of the GraphicsManager */
		std::unique_ptr<Impl> mImpl;

	public:		// Functions
		/** Creates a new GraphicsManager
		 *
		 * @param	graphicsEngine a reference to the GraphicsEngine used by
		 * 			the GraphicsManager to render the entities
		 * @param	eventManager a reference to the EventManager that the
		 *			GraphicsManager will be subscribed to */
		GraphicsManager(
			graphics::GraphicsEngine& graphicsEngine,
			EventManager& eventManager
		);

		/** Class destructor */
		~GraphicsManager();

		/** @return	the GraphicsEngine of the GraphicsManager */
		graphics::GraphicsEngine& getGraphicsEngine()
		{ return mGraphicsEngine; };

		/** @return	the FontRepository of the GraphicsManager */
		FontRepository& getFontRepository()
		{ return mFontRepository; };

		/** @return	the TextureRepository of the GraphicsManager */
		TextureRepository& getTextureRepository()
		{ return mTextureRepository; };

		/** @return	the ProgramRepository of the GraphicsManager */
		ProgramRepository& getProgramRepository()
		{ return mProgramRepository; };

		/** @return	the TechniqueRepository of the GraphicsManager */
		TechniqueRepository& getTechniqueRepository()
		{ return mTechniqueRepository; };

		/** Notifies the GraphicsManager of the given event
		 *
		 * @param	event the IEvent to notify */
		virtual void notify(const IEvent& event) override;

		/** Adds the given Entity and its Camera data to the GraphicsManager
		 *
		 * @param	entity a pointer to the Entity to add to the GraphicsManager
		 * @param	camera a pointer to the Camera to add to the GraphicsManager
		 * @note	The Camera initial data is overridden by the Entity one */
		void addCameraEntity(Entity* entity, CameraUPtr camera);

		/** Adds the given Entity and its ILight data to the GraphicsManager
		 *
		 * @param	entity a pointer to the Entity to add to the GraphicsManager
		 * @param	light a pointer to the ILight to add to the GraphicsManager
		 * @note	The PointLight initial data is overridden by the Entity
		 *			one */
		void addLightEntity(Entity* entity, LightUPtr light);

		/** Creates a new Pass and adds the uniform variables for the cameras
		 *
		 * @param	program a pointer to the program of the new Pass
		 * @return	the new Pass */
		PassSPtr createPass2D(ProgramSPtr program);

		/** Creates a new Pass and adds the uniform variables for the cameras
		 * and lightning
		 *
		 * @param	program a pointer to the program of the new Pass
		 * @param	addLights if we want the uniform variables for lightning or
		 *			not
		 * @return	the new Pass */
		PassSPtr createPass3D(ProgramSPtr program, bool addLights = true);

		/** Adds the given Entity and its Mesh (and skin) data to the
		 * GraphicsManager
		 *
		 * @param	entity a pointer to the Entity to add to the GraphicsManager
		 * @param	renderable a pointer to the RenderableMesh to add to the
		 *			GraphicsManager
		 * @param	skin a pointer to the Skin needed for the skeletal animation
		 *			of the RenderableMesh (optional)
		 * @note	The Mesh initial data is overridden by the Entity
		 *			one. Also the Techniques of the Renderable must has been
		 *			with the TechniqueLoader */
		void addMeshEntity(
			Entity* entity, RenderableMeshUPtr renderable,
			SkinSPtr skin = nullptr
		);

		/** Adds the given Entity and its RenderableTerrain data to the
		 * GraphicsManager
		 *
		 * @param	entity a pointer to the Entity to add to the GraphicsManager
		 * @param	renderable a pointer to the RenderableTerrain to add to the
		 *			GraphicsManager
		 * @note	The Terrain initial data is overridden by the Entity
		 *			one. Also the Techniques of the Renderable must has been
		 *			with the TechniqueLoader. The model matrix of the terrain
		 *			wont be updated with the scale of the Entity. */
		void addTerrainEntity(Entity* entity, RenderableTerrainUPtr renderable);

		/** Removes the given Entity from the GraphicsManager so it won't
		 * longer be updated
		 *
		 * @param	entity a pointer to the Entity to remove from the
		 *			GraphicsManager */
		void removeEntity(Entity* entity);

		/** Updates the graphics data with the Entities
		 *
		 * @note	this function must be called from the thread with the
		 *			Graphics API context (probably thread 0) */
		void update();

		/** Renders the graphics data of the Entities
		 *
		 * @note	this function must be called from the thread with the
		 *			Graphics API context (probably thread 0) */
		void render();
	private:
		/** Handles the given ResizeEvent by notifying the GraphicsEngine of
		 * the window resize
		 *
		 * @param	event the ResizeEvent to handle */
		void onResizeEvent(const ResizeEvent& event);
	};

}

#endif		// GRAPHICS_MANAGER_H
