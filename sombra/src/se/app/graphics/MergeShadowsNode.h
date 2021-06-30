#ifndef MERGE_SHADOWS_NODE_H
#define MERGE_SHADOWS_NODE_H

#include <array>
#include "se/app/Repository.h"
#include "se/graphics/BindableRenderNode.h"
#include "se/graphics/core/Program.h"
#include "se/graphics/core/UniformVariable.h"
#include "se/graphics/3D/Mesh.h"

namespace se::app {

	/**
	 * Class MergeShadowsNode, it's the BindableRenderNode used for combining
	 * all the Shadow maps and render them to a single framebuffer from the
	 * camera perspective. It has an "attach" input and output, a "target"
	 * framebuffer input and output, a "depthTexture" input texture for the
	 * depth buffer rendered from the camera perspective, and @see kMaxShadows
	 * shadowTextureX input textures for the depth buffers rendered from each
	 * shadow perspective
	 */
	class MergeShadowsNode : public graphics::BindableRenderNode
	{
	private:	// Nested types
		/** Holds all the Uniforms of a shadow */
		struct ShadowUniforms
		{
			std::shared_ptr<graphics::UniformVariableValue<int>> active;
			std::shared_ptr<graphics::UniformVariableValue<glm::mat4>>
				viewProjectionMatrix;
		};

	public:		// Attributes
		/** The maximum number of shadows that can be rendered at a single
		 * time */
		static constexpr unsigned int kMaxShadows = 15;

		/** The Texture unit where the Depth Buffer texture must be attached */
		static constexpr unsigned int kDepthTextureUnit = kMaxShadows;
	private:
		/** The program used by the DeferredLightRenderer */
		Repository::ResourceRef<graphics::Program> mProgram;

		/** The plane used for rendering */
		Repository::ResourceRef<graphics::Mesh> mPlane;

		/** Camera view projection matrix uniform variable */
		std::shared_ptr<graphics::UniformVariableValue<glm::mat4>>
			mCameraViewProjectionMatrix;

		/** The Shadows uniforms */
		std::array<ShadowUniforms, kMaxShadows> mShadows;

	public:		// Functions
		/** Creates a new MergeShadowsNode
		 *
		 * @param	name the name of the new MergeShadowsNode
		 * @param	repository the Repository that holds the Resources */
		MergeShadowsNode(const std::string& name, Repository& repository);

		/** Sets the view projection matrix of the camera used for rendering
		 * the scene
		 *
		 * @param	viewProjectionMatrix the new camera view projection
		 *			matrix */
		void setCameraVPMatrix(const glm::mat4& viewProjectionMatrix)
		{ mCameraViewProjectionMatrix->setValue(viewProjectionMatrix); };

		/** Enables or disables a Shadow
		 *
		 * @param	i the index of the shadow to update
		 * @param	active if the shadow should be enabled or disabled */
		void enableShadow(std::size_t i, bool active = true)
		{ mShadows[i].active->setValue(static_cast<int>(active)); };

		/** Changes the Shadow view projection matrix
		 *
		 * @param	i the index of the shadow to update
		 * @param	viewProjectionMatrix the new shadow view projection
		 *			matrix */
		void setShadowVPMatrix(
			std::size_t i, const glm::mat4& viewProjectionMatrix
		) { mShadows[i].viewProjectionMatrix->setValue(viewProjectionMatrix); };

		/** @copydoc RenderNode::execute() */
		virtual void execute() override;
	};

}

#endif		// MERGE_SHADOWS_NODE_H
