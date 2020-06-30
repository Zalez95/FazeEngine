#include <GL/glew.h>
#include "se/utils/Log.h"
#include "se/graphics/core/Constants.h"

#define GL_WRAP(x)					\
	se::graphics::glClearError();	\
	x;								\
	se::graphics::glLogError(#x, __FUNCTION__, __LINE__);


namespace se::graphics {

	static void glClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}


	static void glLogError(const char* glFunction, const char* function, int line)
	{
		GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::string errorTag = "";
			switch (error) {
				case GL_INVALID_ENUM:					errorTag = "INVALID_ENUM";					break;
				case GL_INVALID_VALUE:					errorTag = "INVALID_VALUE";					break;
				case GL_INVALID_OPERATION:				errorTag = "INVALID_OPERATION";				break;
				case GL_STACK_OVERFLOW:					errorTag = "STACK_OVERFLOW";				break;
				case GL_STACK_UNDERFLOW:				errorTag = "STACK_UNDERFLOW";				break;
				case GL_OUT_OF_MEMORY:					errorTag = "OUT_OF_MEMORY";					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:	errorTag = "INVALID_FRAMEBUFFER_OPERATION";	break;
			}

			const char* errorString = reinterpret_cast<const char*>( glGetString(error) );

			utils::Log::getInstance()(utils::LogLevel::Error) << FORMAT_LOCATION(function, line)
				<< "OpenGL function \"" << glFunction << "\" returned error code " << error
				<< " (" << errorTag << "): \"" << (errorString? errorString : "") << "\"";
		}
	}


	constexpr GLenum toGLType(TypeId type)
	{
		switch (type) {
			case TypeId::Byte:			return GL_BYTE;
			case TypeId::UnsignedByte:	return GL_UNSIGNED_BYTE;
			case TypeId::Short:			return GL_SHORT;
			case TypeId::UnsignedShort:	return GL_UNSIGNED_SHORT;
			case TypeId::Int:			return GL_INT;
			case TypeId::UnsignedInt:	return GL_UNSIGNED_INT;
			case TypeId::Float:			return GL_FLOAT;
			case TypeId::HalfFloat:		return GL_HALF_FLOAT;
			case TypeId::Double:		return GL_DOUBLE;
			default:					return GL_NONE;
		}
	}


	constexpr GLenum toGLPrimitive(PrimitiveType primitive)
	{
		switch (primitive) {
			case PrimitiveType::Point:			return GL_POINT;
			case PrimitiveType::Line:			return GL_LINE;
			case PrimitiveType::Triangle:		return GL_TRIANGLES;
			case PrimitiveType::TriangleFan:	return GL_TRIANGLE_FAN;
			case PrimitiveType::TriangleStrip:	return GL_TRIANGLE_STRIP;
			default:							return GL_NONE;
		}
	}


	constexpr GLenum toGLShader(ShaderType type)
	{
		switch (type) {
			case ShaderType::Vertex:	return GL_VERTEX_SHADER;
			case ShaderType::Geometry:	return GL_GEOMETRY_SHADER;
			case ShaderType::Fragment:	return GL_FRAGMENT_SHADER;
			default:					return GL_NONE;
		}
	}


	constexpr GLenum toGLFrameBufferTarget(FrameBufferTarget target)
	{
		switch (target) {
			case FrameBufferTarget::Read:	return GL_READ_FRAMEBUFFER;	break;
			case FrameBufferTarget::Write:	return GL_DRAW_FRAMEBUFFER;	break;
			case FrameBufferTarget::Both:	return GL_FRAMEBUFFER;		break;
			default:						return GL_NONE;
		}
	}


	constexpr int toGLTextureTarget(TextureTarget target)
	{
		switch (target) {
			case TextureTarget::Texture1D:	return GL_TEXTURE_1D;
			case TextureTarget::Texture2D:	return GL_TEXTURE_2D;
			case TextureTarget::Texture3D:	return GL_TEXTURE_3D;
			case TextureTarget::CubeMap:	return GL_TEXTURE_CUBE_MAP;
			default:						return GL_NONE;
		}
	}


	constexpr GLenum toGLColorFormat(ColorFormat format)
	{
		switch (format) {
			case ColorFormat::Red:			return GL_RED;
			case ColorFormat::RG:			return GL_RG;
			case ColorFormat::RGB:			return GL_RGB;
			case ColorFormat::RGBA:			return GL_RGBA;
			case ColorFormat::Depth:		return GL_DEPTH_COMPONENT;
			case ColorFormat::Depth16:		return GL_DEPTH_COMPONENT16;
			case ColorFormat::Depth24:		return GL_DEPTH_COMPONENT24;
			case ColorFormat::Depth32:		return GL_DEPTH_COMPONENT32F;
			case ColorFormat::DepthStencil:	return GL_DEPTH_STENCIL;
			case ColorFormat::RedInteger:	return GL_RED_INTEGER;
			case ColorFormat::RGInteger:	return GL_RG_INTEGER;
			case ColorFormat::RGBInteger:	return GL_RGB_INTEGER;
			case ColorFormat::RGBAInteger:	return GL_RGBA_INTEGER;
			case ColorFormat::Red32ui:		return GL_R32UI;
			case ColorFormat::RG32ui:		return GL_RG32UI;
			case ColorFormat::RGB16ui:		return GL_RGB16UI;
			case ColorFormat::RGB16f:		return GL_RGB16F;
			case ColorFormat::RGB32ui:		return GL_RGB32UI;
			case ColorFormat::RGB32f:		return GL_RGB32F;
			case ColorFormat::RGBA8:		return GL_RGBA8;
			case ColorFormat::RGBA16ui:		return GL_RGBA16UI;
			case ColorFormat::RGBA16f:		return GL_RGBA16F;
			case ColorFormat::RGBA32ui:		return GL_RGBA32UI;
			case ColorFormat::RGBA32f:		return GL_RGBA32F;
			default:						return GL_NONE;
		}
	}


	constexpr int toGLFilter(TextureFilter filter)
	{
		switch (filter) {
			case TextureFilter::Nearest:				return GL_NEAREST;
			case TextureFilter::Linear:					return GL_LINEAR;
			case TextureFilter::NearestMipMapNearest:	return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilter::LinearMipMapNearest:	return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilter::NearestMipMapLinear:	return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilter::LinearMipMapLinear:		return GL_LINEAR_MIPMAP_LINEAR;
			default:									return GL_NONE;
		}
	}


	constexpr int toGLWrap(TextureWrap wrap)
	{
		switch (wrap) {
			case TextureWrap::Repeat:			return GL_REPEAT;
			case TextureWrap::MirroredRepeat:	return GL_MIRRORED_REPEAT;
			case TextureWrap::ClampToEdge:		return GL_CLAMP_TO_EDGE;
			case TextureWrap::ClampToBorder:	return GL_CLAMP_TO_BORDER;
			default:							return GL_NONE;
		}
	}

}
