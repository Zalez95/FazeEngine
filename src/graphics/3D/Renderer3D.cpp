#include "Renderer3D.h"
#include "../Texture.h"
#include "Renderable3D.h"
#include "Mesh.h"
#include "Camera.h"

namespace graphics {

	void Renderer3D::submit(const Renderable3D* renderable3D)
	{
		if (renderable3D) {
			mRenderable3Ds.push(renderable3D);
		}
	}


	void Renderer3D::render(const Camera* camera, const std::vector<const PointLight*>& pointLights)
	{
		glm::mat4 viewMatrix = (camera)? camera->getViewMatrix() : glm::mat4();

		mProgram.enable();
		mProgram.setViewMatrix(viewMatrix);
		mProgram.setProjectionMatrix(mProjectionMatrix);
		mProgram.setLights(pointLights);

		while (!mRenderable3Ds.empty()) {
			const Renderable3D* renderable3D = mRenderable3Ds.front();
			mRenderable3Ds.pop();

			auto mesh				= renderable3D->getMesh();
			auto material			= renderable3D->getMaterial();
			auto texture			= renderable3D->getTexture();
			glm::mat4 modelMatrix	= renderable3D->getModelMatrix();
			RenderFlags flags		= renderable3D->getRenderFlags();

			if (!mesh) { continue; }

			// Set the mesh transparency
			if (flags & RenderFlags::DISABLE_DEPTH_TEST) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_DEPTH_TEST);
			}

			// Bind the program data
			mProgram.setModelMatrix(modelMatrix);
			mProgram.setColorTexture(0);
			if (material) { mProgram.setMaterial(material.get()); }
			if (texture) { texture->bind(0); }

			// Draw
			GLenum renderMode = (flags & RenderFlags::WIREFRAME)? GL_LINES : GL_TRIANGLES;

			mesh->bind();
			glDrawElements(renderMode, mesh->getIndexCount(), GL_UNSIGNED_SHORT, nullptr);
			mesh->unbind();

			// Unbind the program data
			if (texture) { texture->unbind(); }

			// Unset the mesh transparency
			if (flags & RenderFlags::DISABLE_DEPTH_TEST) {
				glEnable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);
			}
		}

		mProgram.disable();
	}

}