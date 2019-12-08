#ifndef RENDERABLE_TERRAIN_H
#define RENDERABLE_TERRAIN_H

#include <memory>
#include "QuadTree.h"

namespace se::graphics {

	class Camera;
	class Texture;
	struct Material;


	/**
	 * Class RenderableTerrain, its a Renderable that holds all the terrain data
	 */
	class RenderableTerrain
	{
	public:		// Nested types
		using TextureSPtr = std::shared_ptr<Texture>;
		using MaterialSPtr = std::shared_ptr<Material>;

	private:	// Attributes
		/** The size of the terrain in the XZ plane */
		float mSize;

		/** The maximum height/depression the terrain can have in the Y axis */
		float mMaxHeight;

		/** The height map texture of the Terrain */
		TextureSPtr mHeightMap;

		/** The terrain QuadTree */
		QuadTree mQuadTree;

		/** The matrix that transforms the coordinates of the Terrain from
		 * Local space to World space */
		glm::mat4 mModelMatrix;

		/** The Material of the Terrain */
		MaterialSPtr mMaterial;

	public:		// Functions
		/** Creates a new RenderableTerrain
		 *
		 * @param	size the size of the terrain in the XZ plane
		 * @param	maxHeight the maximum height/depression the terrain can
		 *			have in the Y axis
		 * @param	heightMap the texture to use as height map in the terrain
		 * @param	lodDistances the minimum distance to the camera at each
		 *			level of detail
		 * @param	material the material of the Terrain */
		RenderableTerrain(
			float size, float maxHeight, TextureSPtr heightMap,
			const std::vector<float>& lodDistances, MaterialSPtr material
		) : mSize(size), mMaxHeight(maxHeight), mHeightMap(heightMap),
			mQuadTree(size, lodDistances),
			mModelMatrix(1.0f), mMaterial(material) {};

		/** @return	the size of the Terrain in the XZ plane */
		float getSize() const { return mSize; };

		/** @return	the maximum height/depression the terrain can have in the
		 *			Y axis */
		float getMaxHeight() const { return mMaxHeight; };

		/** @return	the height map texture of the Terrain */
		TextureSPtr getHeightMap() const { return mHeightMap; };

		/** @return	a pointer to the Material of the Terrain */
		const MaterialSPtr getMaterial() const { return mMaterial; };

		/** @return	the model matrix of the Terrain */
		glm::mat4 getModelMatrix() const { return mModelMatrix; };

		/** @return	the QuadTree of the Terrain */
		const QuadTree& getQuadTree() const { return mQuadTree; };

		/** Sets the Model Matrix of the Terrain
		 *
		 * @param	modelMatrix the new model matrix of the Terrain */
		void setModelMatrix(const glm::mat4& modelMatrix)
		{ mModelMatrix = modelMatrix; };

		/** Updates the terrain QuadTree according to the given camera and the
		 * configured level of details
		 *
		 * @param	camera the camera used for updating the terrain */
		void update(const Camera& camera);
	};

}

#endif		// RENDERABLE_TERRAIN_H
