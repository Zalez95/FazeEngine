#include "se/graphics/3D/RendererTerrain.h"
#include "se/graphics/3D/Camera.h"
#include "se/graphics/3D/RenderableTerrain.h"
#include "../core/GLWrapper.h"

namespace se::graphics {

	static const float kNormal[] = {
		 0.0f, 0.0f, 0.0f,-0.5f,-0.5f,-0.5f,-0.5f, 0.0f,-0.5f, 0.5f,
		 0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.5f,-0.5f, 0.0f,-0.5f
	};

	static const float kBottom[] = {
		 0.0f, 0.0f,-0.5f,-0.5f,-0.5f, 0.0f,-0.5f, 0.5f, 0.0f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.0f, 0.5f,-0.5f,-0.5f,-0.5f,
	};

	static const float kTop[] = {
		 0.0f, 0.0f,-0.5f,-0.5f,-0.5f, 0.0f,-0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.0f, 0.5f,-0.5f, 0.0f,-0.5f,-0.5f,-0.5f
	};

	static const float kLeft[] = {
		 0.0f, 0.0f,-0.5f,-0.5f,-0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.0f, 0.5f,-0.5f, 0.0f,-0.5f,-0.5f,-0.5f
	};

	static const float kRight[] = {
		 0.0f, 0.0f,-0.5f,-0.5f,-0.5f, 0.0f,-0.5f, 0.5f, 0.0f, 0.5f,
		 0.5f, 0.5f, 0.5f,-0.5f, 0.0f,-0.5f,-0.5f,-0.5f
	};

	static const float kBottomLeft[] = {
		 0.0f, 0.0f,-0.5f,-0.5f,-0.5f, 0.5f, 0.0f, 0.5f,
		 0.5f, 0.5f, 0.5f, 0.0f, 0.5f,-0.5f,-0.5f,-0.5f
	};

	static const float kBottomRight[] = {
		 0.0f, 0.0f,-0.5f,-0.5f,-0.5f, 0.0f,-0.5f, 0.5f,
		 0.0f, 0.5f, 0.5f, 0.5f, 0.5f,-0.5f,-0.5f,-0.5f
	};

	static const float kTopLeft[] = {
		 0.0f, 0.0f,-0.5f,-0.5f,-0.5f, 0.5f, 0.5f, 0.5f,
		 0.5f, 0.0f, 0.5f,-0.5f, 0.0f,-0.5f,-0.5f,-0.5f
	};

	static const float kTopRight[] = {
		 0.0f, 0.0f,-0.5f,-0.5f,-0.5f, 0.0f,-0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,-0.5f, 0.0f,-0.5f,-0.5f,-0.5f
	};


	RendererTerrain::Patch::Patch(const float* vertices, std::size_t numVertices) :
		mNumVertices(numVertices), mInstanceCount(0)
	{
		mVBOXZPositions.setData(vertices, numVertices);

		mVAO.bind();

		mVBOXZPositions.bind();
		mVAO.setVertexAttribute(0, TypeId::Float, false, 2, 0);

		mVBOXZLocations.bind();
		mVAO.setVertexAttribute(1, TypeId::Float, false, 2, 0);
		mVAO.setAttributeDivisor(1, 1);

		mVBOLods.bind();
		mVAO.setVertexAttribute(2, TypeId::Int, false, 1, 0);
		mVAO.setAttributeDivisor(2, 1);

		mVAO.unbind();
	}


	void RendererTerrain::Patch::submitInstance(const glm::vec2& nodeLocation, int lod)
	{
		mInstanceCount++;
		mXZLocations.push_back(nodeLocation);
		mLods.push_back(lod);
	}


	void RendererTerrain::Patch::drawInstances()
	{
		// Set submitted instances data
		mVBOXZLocations.setData(mXZLocations.data(), mXZLocations.size());
		mVBOLods.setData(mLods.data(), mLods.size());

		// Render instanced
		mVAO.bind();
		GL_WRAP( glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, mNumVertices, mInstanceCount) );

		// Clear submitted instances
		mInstanceCount = 0;
		mXZLocations.clear();
		mLods.clear();
	}


	RendererTerrain::RendererTerrain() :
		mNormal(kNormal, 20),
		mBottom(kBottom, 18), mTop(kTop, 18), mLeft(kLeft, 18), mRight(kRight, 18),
		mBottomLeft(kBottomLeft, 16), mBottomRight(kBottomRight, 16), mTopLeft(kTopLeft, 16), mTopRight(kTopRight, 16)
	{
		if (!mProgram.init()) {
			SOMBRA_ERROR_LOG << "Failed to create the Program";
		}
	}


	RendererTerrain::~RendererTerrain()
	{
		mProgram.end();
	}


	void RendererTerrain::render(
		const Camera* camera, const std::vector<const ILight*>& lights,
		const RenderableTerrain& terrain
	) {
		glm::mat4 viewMatrix(1.0f), projectionMatrix(1.0f);
		if (camera) {
			viewMatrix = camera->getViewMatrix();
			projectionMatrix = camera->getProjectionMatrix();
		}

		auto material = terrain.getMaterial();

		// Bind uniforms
		mProgram.enable();
		mProgram.setViewMatrix(viewMatrix);
		mProgram.setProjectionMatrix(projectionMatrix);
		mProgram.setModelMatrix(terrain.getModelMatrix());
		mProgram.setXZSize(terrain.getSize());
		mProgram.setMaxHeight(terrain.getMaxHeight());
		mProgram.setHeightMap(*terrain.getHeightMap());
		mProgram.setLights(lights);

		if (material) { mProgram.setMaterial(*material); }

		// Submit the nodes to their respective patch
		submitNode(terrain.getQuadTree().getRootNode(), glm::vec2(0.0f));

		// Draw the patches
		mNormal.drawInstances();
		mBottom.drawInstances();
		mTop.drawInstances();
		mLeft.drawInstances();
		mRight.drawInstances();
		mBottomLeft.drawInstances();
		mBottomRight.drawInstances();
		mTopLeft.drawInstances();
		mTopRight.drawInstances();

		if (material) { mProgram.unsetMaterial(*material); }
	}

// Private functions
	void RendererTerrain::submitNode(const QuadTree::Node& node, const glm::vec2& parentLocation)
	{
		glm::vec2 nodeLocation(parentLocation + node.xzSeparation);

		if (node.isLeaf) {
			if (node.neighboursLods[static_cast<int>(QuadTree::Direction::Bottom)] < node.lod) {
				if (node.neighboursLods[static_cast<int>(QuadTree::Direction::Left)] < node.lod) {
					mBottomLeft.submitInstance(nodeLocation, node.lod);
				}
				else if (node.neighboursLods[static_cast<int>(QuadTree::Direction::Right)] < node.lod) {
					mBottomRight.submitInstance(nodeLocation, node.lod);
				}
				else {
					mBottom.submitInstance(nodeLocation, node.lod);
				}
			}
			else if (node.neighboursLods[static_cast<int>(QuadTree::Direction::Top)] < node.lod) {
				if (node.neighboursLods[static_cast<int>(QuadTree::Direction::Left)] < node.lod) {
					mTopLeft.submitInstance(nodeLocation, node.lod);
				}
				else if (node.neighboursLods[static_cast<int>(QuadTree::Direction::Right)] < node.lod) {
					mTopRight.submitInstance(nodeLocation, node.lod);
				}
				else {
					mTop.submitInstance(nodeLocation, node.lod);
				}
			}
			else if (node.neighboursLods[static_cast<int>(QuadTree::Direction::Left)] < node.lod) {
				mLeft.submitInstance(nodeLocation, node.lod);
			}
			else if (node.neighboursLods[static_cast<int>(QuadTree::Direction::Right)] < node.lod) {
				mRight.submitInstance(nodeLocation, node.lod);
			}
			else {
				mNormal.submitInstance(nodeLocation, node.lod);
			}
		}
		else {
			for (auto& child : node.children) {
				submitNode(*child, nodeLocation);
			}
		}
	}

}
