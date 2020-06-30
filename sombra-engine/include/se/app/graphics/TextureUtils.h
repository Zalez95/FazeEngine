#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H

#include <memory>
#include "../../graphics/core/Texture.h"

namespace se::app {

	/**
	 * Class TextureUtils, holds all the Texture transformation functions
	 */
	class TextureUtils
	{
	private:	// Nested types
		using TextureSPtr = std::shared_ptr<graphics::Texture>;

	public:		// Functions
		/** Creates a new CubeMap Texture from the given equirectangular one
		 *
		 * @param	source the source Texture to convert
		 * @param	size the width and height of each of the CubeMap images
		 * @return	the new CubeMap Texture */
		static TextureSPtr equirectangularToCubeMap(
			TextureSPtr source, std::size_t size
		);

		/** Creates a new CubeMap Texture from the given one by convoluting all
		 * its values
		 *
		 * @param	source the source Texture to convert
		 * @param	size the width and height of each of the CubeMap images
		 * @return	the new CubeMap Texture */
		static TextureSPtr convoluteCubeMap(
			TextureSPtr source, std::size_t size
		);

		/** Creates a new CubeMap Texture from the given one by prefiltering it
		 * with different roughness values, and storing the resulting it the
		 * different mipmap levels of the new CubeMap texture
		 *
		 * @param	source the source Texture to convert
		 * @param	size the width and height of each of the CubeMap images
		 * @return	the new CubeMap Texture */
		static TextureSPtr prefilterCubeMap(
			TextureSPtr source, std::size_t size
		);

		/** Creates a normal map Texture from the given height map one with the
		 * normal vectors stored in local space
		 *
		 * @param	source the source Texture to convert
		 * @param	width the width of the source and output textures
		 * @param	height the height of the source and output textures
		 * @return	the new normal map Texture */
		static TextureSPtr heightmapToNormalMapLocal(
			TextureSPtr source, std::size_t width, std::size_t height
		);
	};

}

#endif		// TEXTURE_UTILS_H
