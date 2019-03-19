#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstddef>
#include "Constants.h"

namespace se::graphics {

	/**
	 * Texture Class
	 */
	class Texture
	{
	private:	// Attributes
		/** The id of the texture object */
		unsigned int mTextureId;

	public:		// Functions
		/** Creates a new Texture */
		Texture();
		Texture(const Texture& other) = delete;
		Texture(Texture&& other);

		/** Class destructor */
		~Texture();

		/** Assignment operator */
		Texture& operator=(const Texture& other) = delete;
		Texture& operator=(Texture&& other);

		/** @return the id of the Texture */
		inline unsigned int getTextureId() const { return mTextureId; };

		/** Sets the filtering method used by the texture
		 *
		 * @param	minification the method used in the minification process
		 * @param	magnification the method used in the magnification
		 *			process */
		void setFiltering(
			TextureFilter minification, TextureFilter magnification
		) const;

		/** Sets the behavior of the texture in each axis when a requested
		 * position falls out of the [0.0, 1.0] range.
		 *
		 * @param	wrapS the TextureWrap in the x axis
		 * @param	wrapT the TextureWrap in the y axis */
		void setWrapping(TextureWrap wrapS, TextureWrap wrapT) const;

		/** Sets the image data of the Texture
		 *
		 * @param	pixels the pixel data of the new Texture
		 * @param	type the type of the pixel data of the new Texture
		 * @param	format the ColorFormat of the new Texture
		 * @param	width the width of the new Texture
		 * @param	height the height of the new Texture */
		void setImage(
			const void* pixels, TypeId type, ColorFormat format,
			std::size_t width, std::size_t height
		) const;

		/** Generate mipmaps for the current texture */
		void generateMipMap() const;

		/** Binds the Texture
		 *
		 * @param	slot the texture unit where we want to bind the texture */
		void bind(unsigned int slot = 0) const;

		/** Unbinds the Texture */
		void unbind() const;
	};

}

#endif		// TEXTURE_H
