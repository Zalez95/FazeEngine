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


	void Layer3D::addLight(const ILight* light)
	{
		if (light) {
			mLights.push_back(light);
		}
	}


	void Layer3D::removeLight(const ILight* light)
	{
		mLights.erase(
			std::remove(mLights.begin(), mLights.end(), light),
			mLights.end()
		);
	}


	void Layer3D::render()
	{
		if (mSky) {
			mRendererSky.render(mCamera, *mSky);
		}

		if (mTerrain) {
			mRendererTerrain.render(mCamera, mLights, *mTerrain);
		}

		for (const Renderable3D* renderable3D : mRenderable3Ds) {
			mRendererPBR.submit(renderable3D);
		}
		mRendererPBR.render(mCamera, mLights);
	}

}
