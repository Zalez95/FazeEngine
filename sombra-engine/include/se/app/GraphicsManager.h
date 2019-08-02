#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <map>
#include "../graphics/GraphicsSystem.h"
#include "../graphics/3D/Layer3D.h"
#include "../graphics/3D/Camera.h"
#include "../graphics/3D/Lights.h"
#include "../graphics/3D/Renderable3D.h"

namespace se::app {

	struct Entity;


	/**
	 * Class GraphicsManager, it's a Manager used for storing, updating and
	 * rendering the Entities' graphics data
	 */
	class GraphicsManager
	{
	private:	// Nested types
		using CameraUPtr = std::unique_ptr<graphics::Camera>;
		using PointLightUPtr = std::unique_ptr<graphics::PointLight>;
		using Renderable3DUPtr = std::unique_ptr<graphics::Renderable3D>;

	private:	// Attributes
		/** The System used for rendering the data of the Entities */
		graphics::GraphicsSystem& mGraphicsSystem;

		/** The Layer3D used by the GraphicsSystem */
		graphics::Layer3D mLayer3D;

		std::map<Entity*, CameraUPtr> mCameraEntities;
		std::map<Entity*, PointLightUPtr> mPointLightEntities;
		std::multimap<Entity*, Renderable3DUPtr> mRenderable3DEntities;

	public:		// Functions
		/** Creates a new GraphicsManager
		 *
		 * @param	graphicsSystem a reference to the GraphicsSystem used by
		 * 			the GraphicsManager to render the entities */
		GraphicsManager(graphics::GraphicsSystem& graphicsSystem);

		/** Adds the given Entity and its Camera data to the GraphicsManager
		 *
		 * @param	entity a pointer to the Entity to add to the
		 *			GraphicsManager
		 * @param	camera a pointer to the camera to add to the
		 *			GraphicsManager
		 * @note	The Camera initial data is overridden by the Entity one */
		void addEntity(Entity* entity, CameraUPtr camera);

		/** Adds the given Entity and its Renderable3D data to the
		 * GraphicsManager
		 *
		 * @param	entity a pointer to the Entity to add to the
		 *			GraphicsManager
		 * @param	renderable3D a pointer to the Renderable3D to add to the
		 *			GraphicsManager
		 * @note	The Renderable3D initial data is overridden by the Entity
		 *			one */
		void addEntity(Entity* entity, Renderable3DUPtr renderable3D);

		/** Adds the given Entity and its PointLight data to the
		 * GraphicsManager
		 *
		 * @param	entity a pointer to the Entity to add to the
		 *			GraphicsManager
		 * @param	pointLight a pointer to the PointLight to add to the
		 *			GraphicsManager
		 * @note	The PointLight initial data is overridden by the Entity
		 *			one */
		void addEntity(Entity* entity, PointLightUPtr pointLight);

		/** Removes the given Entity from the GraphicsManager so it won't
		 * longer be updated
		 *
		 * @param	entity a pointer to the Entity to remove from the
		 *			GraphicsManager */
		void removeEntity(Entity* entity);

		/** Updates the graphics data with the Entities */
		void update();

		/** Renders the graphics data of the Entities */
		void render();
	};

}

#endif		// GRAPHICS_MANAGER_H
