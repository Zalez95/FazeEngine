#include <algorithm>
#include "se/graphics/3D/Layer3D.h"

namespace se::graphics {

	void Layer3D::addRenderable3D(const Renderable3D* renderable3D)
	{
		if (renderable3D) {
			mRenderable3Ds.push_back(renderable3D);
		}
	}


	void Layer3D::removeRenderable3D(const Renderable3D* renderable3D)
	{
		mRenderable3Ds.erase(
			std::remove(mRenderable3Ds.begin(), mRenderable3Ds.end(), renderable3D),
			mRenderable3Ds.end()
		);
	}


	void Layer3D::addPointLight(const PointLight* pointLight)
	{
		if (pointLight) {
			mPointLights.push_back(pointLight);
		}
	}


	void Layer3D::removePointLight(const PointLight* pointLight)
	{
		mPointLights.erase(
			std::remove(mPointLights.begin(), mPointLights.end(), pointLight),
			mPointLights.end()
		);
	}


	void Layer3D::render()
	{
		if (mSky) {
			mRenderer3D.renderSky(mCamera, *mSky);
		}

		for (const Renderable3D* renderable3D : mRenderable3Ds) {
			mRenderer3D.submit(renderable3D);
		}
		mRenderer3D.render(mCamera, mPointLights);
	}

}
