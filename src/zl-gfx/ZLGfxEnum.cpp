// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/headers.h>
#include <zl-gfx/ZLGfx-gles.h>
#include <zl-gfx/ZLGfxEnum.h>

//================================================================//
// ZLGfxEnum
//================================================================//

//----------------------------------------------------------------//
ZGLEnum ZLGfxEnum::MapNativeToZL ( u32 value ) {

	switch ( value ) {
		case GL_DST_ALPHA:					return ZGL_BLEND_FACTOR_DST_ALPHA;
		case GL_DST_COLOR:					return ZGL_BLEND_FACTOR_DST_COLOR;
		case GL_ONE:						return ZGL_BLEND_FACTOR_ONE;
		case GL_ONE_MINUS_DST_ALPHA:		return ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case GL_ONE_MINUS_DST_COLOR:		return ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case GL_ONE_MINUS_SRC_ALPHA:		return ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case GL_ONE_MINUS_SRC_COLOR:		return ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case GL_SRC_ALPHA:					return ZGL_BLEND_FACTOR_SRC_ALPHA;
		case GL_SRC_ALPHA_SATURATE:			return ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		case GL_SRC_COLOR:					return ZGL_BLEND_FACTOR_SRC_COLOR;
		case GL_ZERO:						return ZGL_BLEND_FACTOR_ZERO;
	};
	assert ( false );
	return ZGL_NONE;
}

//----------------------------------------------------------------//
u32 ZLGfxEnum::MapZLToNative ( ZGLEnum value ) {

	switch ( value ) {

		case ZGL_BLEND_FACTOR_DST_ALPHA:					return GL_DST_ALPHA;
		case ZGL_BLEND_FACTOR_DST_COLOR:					return GL_DST_COLOR;
		case ZGL_BLEND_FACTOR_ONE:							return GL_ONE;
		case ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:			return GL_ONE_MINUS_DST_ALPHA;
		case ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR:			return GL_ONE_MINUS_DST_COLOR;
		case ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:			return GL_ONE_MINUS_SRC_ALPHA;
		case ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:			return GL_ONE_MINUS_SRC_COLOR;
		case ZGL_BLEND_FACTOR_SRC_ALPHA:					return GL_SRC_ALPHA;
		case ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE:			return GL_SRC_ALPHA_SATURATE;
		case ZGL_BLEND_FACTOR_SRC_COLOR:					return GL_SRC_COLOR;
		case ZGL_BLEND_FACTOR_ZERO:							return GL_ZERO;

		case ZGL_BLEND_MODE_ADD:							return GL_FUNC_ADD;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BLEND_MODE_MAX:						return GL_MAX;
			case ZGL_BLEND_MODE_MIN:						return GL_MIN;
		#endif

		case ZGL_BLEND_MODE_REVERSE_SUBTRACT:				return GL_FUNC_REVERSE_SUBTRACT;
		case ZGL_BLEND_MODE_SUBTRACT:						return GL_FUNC_SUBTRACT;

		case ZGL_BUFFER_TARGET_ARRAY:						return GL_ARRAY_BUFFER;
		case ZGL_BUFFER_TARGET_ELEMENT_ARRAY:				return GL_ELEMENT_ARRAY_BUFFER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BUFFER_USAGE_DYNAMIC_COPY:				return GL_DYNAMIC_COPY;
		#endif

		case ZGL_BUFFER_USAGE_DYNAMIC_DRAW:					return GL_DYNAMIC_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BUFFER_USAGE_DYNAMIC_READ:				return GL_DYNAMIC_READ;
			case ZGL_BUFFER_USAGE_STATIC_COPY:				return GL_STATIC_COPY;
		#endif

		case ZGL_BUFFER_USAGE_STATIC_DRAW:					return GL_STATIC_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BUFFER_USAGE_STATIC_READ:				return GL_STATIC_READ;
			case ZGL_BUFFER_USAGE_STREAM_COPY:				return GL_STREAM_COPY;
		#endif

		case ZGL_BUFFER_USAGE_STREAM_DRAW:					return GL_STREAM_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_BUFFER_USAGE_STREAM_READ:				return GL_STREAM_READ;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_BLACKBERRY )
			case ZGL_COMPOSE_MODULATE:						return GL_MODULATE;
		#endif

		case ZGL_CULL_ALL:									return GL_FRONT_AND_BACK;
		case ZGL_CULL_BACK:									return GL_BACK;
		case ZGL_CULL_FRONT:								return GL_FRONT;

		case ZGL_DEPTH_ALWAYS:								return GL_ALWAYS;
		case ZGL_DEPTH_EQUAL:								return GL_EQUAL;
		case ZGL_DEPTH_LESS:								return GL_LESS;
		case ZGL_DEPTH_LEQUAL:								return GL_LEQUAL;
		case ZGL_DEPTH_GEQUAL:								return GL_GEQUAL;
		case ZGL_DEPTH_GREATER:								return GL_GREATER;
		case ZGL_DEPTH_NEVER:								return GL_NEVER;
		case ZGL_DEPTH_NOTEQUAL:							return GL_NOTEQUAL;

		case ZGL_FRAMEBUFFER_ATTACHMENT_COLOR:				return GL_COLOR_ATTACHMENT0;
		case ZGL_FRAMEBUFFER_ATTACHMENT_DEPTH:				return GL_DEPTH_ATTACHMENT;
		case ZGL_FRAMEBUFFER_ATTACHMENT_STENCIL:			return GL_STENCIL_ATTACHMENT;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_FRAMEBUFFER_TARGET_DRAW:				return GL_DRAW_FRAMEBUFFER;
			case ZGL_FRAMEBUFFER_TARGET_READ:				return GL_READ_FRAMEBUFFER;
		#endif

		case ZGL_FRAMEBUFFER_TARGET_DRAW_READ:				return GL_FRAMEBUFFER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_MATRIX_COLOR:							return GL_COLOR;
			case ZGL_MATRIX_MODELVIEW:						return GL_MODELVIEW;
			case ZGL_MATRIX_PROJECTION:						return GL_PROJECTION;
		#endif

		case ZGL_MATRIX_TEXTURE:							return GL_TEXTURE;

		case ZGL_PIPELINE_BLEND:							return GL_BLEND;

		#if !defined ( MOAI_OS_NACL )
		  case ZGL_PIPELINE_COLOR_ARRAY:					return GL_COLOR_ARRAY;
		#endif
		case ZGL_PIPELINE_CULL:								return GL_CULL_FACE;
		case ZGL_PIPELINE_DEPTH:							return GL_DEPTH_TEST;

		#if !defined ( MOAI_OS_NACL )
			case ZGL_PIPELINE_NORMAL_ARRAY:					return GL_NORMAL_ARRAY;
		#endif

		case ZGL_PIPELINE_SCISSOR:							return GL_SCISSOR_TEST;
		case ZGL_PIPELINE_TEXTURE_2D:						return GL_TEXTURE_2D;

		#if !defined ( MOAI_OS_NACL )
			case ZGL_PIPELINE_TEXTURE_COORD_ARRAY:			return GL_TEXTURE_COORD_ARRAY;
			case ZGL_PIPELINE_VERTEX_ARRAY:					return GL_VERTEX_ARRAY;
		#endif

		#if !defined ( MOAI_OS_IPHONE )
			//case ZGL_COLOR_INDEX:								return GL_COLOR_INDEX;
			//case ZGL_STENCIL_INDEX:								return GL_STENCIL_INDEX;
		#endif
		
		case ZGL_DEPTH_COMPONENT:							return GL_DEPTH_COMPONENT;

		case ZGL_PIXEL_FORMAT_ALPHA:						return GL_ALPHA;
		case ZGL_PIXEL_FORMAT_LUMINANCE:					return GL_LUMINANCE;
		case ZGL_PIXEL_FORMAT_LUMINANCE_ALPHA:				return GL_LUMINANCE_ALPHA;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_RED:						return GL_RED;
			case ZGL_PIXEL_FORMAT_RG:						return GL_RG;
		#endif

		case ZGL_PIXEL_FORMAT_RGB:							return GL_RGB;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_RGB4:						return GL_RGB4;
		#endif

		case ZGL_PIXEL_FORMAT_RGB5_A1:						return GL_RGB5_A1;

		#if defined ( MOAI_OS_ANDROID ) || defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_RGB565:					return GL_RGB565;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_RGB8:						return GL_RGB8;
			case ZGL_PIXEL_FORMAT_BGR:						return GL_BGR;
		#endif

		case ZGL_PIXEL_FORMAT_RGBA:							return GL_RGBA;
		case ZGL_PIXEL_FORMAT_RGBA4:						return GL_RGBA4;

		#if !defined ( MOAI_OS_ANDROID )
			case ZGL_PIXEL_FORMAT_RGBA8:					return GL_RGBA8;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_FORMAT_BGRA:						return GL_BGRA;
		#endif

		case ZGL_PIXEL_TYPE_BYTE:							return GL_BYTE;

		#ifdef MOAI_OS_IPHONE
			case ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:	return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
			case ZGL_PIXEL_TYPE_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:	return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
			case ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:	return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
			case ZGL_PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:	return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		#endif

		case ZGL_PIXEL_TYPE_FLOAT:							return GL_FLOAT;
		case ZGL_PIXEL_TYPE_INT:							return GL_INT;
		case ZGL_PIXEL_TYPE_SHORT:							return GL_SHORT;
		case ZGL_PIXEL_TYPE_UNSIGNED_BYTE:					return GL_UNSIGNED_BYTE;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_TYPE_UNSIGNED_BYTE_2_3_3_REV:	return GL_UNSIGNED_BYTE_2_3_3_REV;
			case ZGL_PIXEL_TYPE_UNSIGNED_BYTE_3_3_2:		return GL_UNSIGNED_BYTE_3_3_2;
		#endif

		case ZGL_PIXEL_TYPE_UNSIGNED_INT:					return GL_UNSIGNED_INT;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_TYPE_UNSIGNED_INT_8_8_8_8:			return GL_UNSIGNED_INT_8_8_8_8;
			case ZGL_PIXEL_TYPE_UNSIGNED_INT_8_8_8_8_REV:		return GL_UNSIGNED_INT_8_8_8_8_REV;
			case ZGL_PIXEL_TYPE_UNSIGNED_INT_2_10_10_10_REV:	return GL_UNSIGNED_INT_2_10_10_10_REV;
			case ZGL_PIXEL_TYPE_UNSIGNED_INT_10_10_10_2:		return GL_UNSIGNED_INT_10_10_10_2;
		#endif

		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT:					return GL_UNSIGNED_SHORT;
		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5:			return GL_UNSIGNED_SHORT_5_6_5;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_6_5_REV:	return GL_UNSIGNED_SHORT_5_6_5_REV;
		#endif

		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4:			return GL_UNSIGNED_SHORT_4_4_4_4;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4_REV:		return GL_UNSIGNED_SHORT_4_4_4_4_REV;
			case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_1_5_5_5_REV:		return GL_UNSIGNED_SHORT_1_5_5_5_REV;
		#endif

		case ZGL_PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1:		return GL_UNSIGNED_SHORT_5_5_5_1;

		case ZGL_PRIM_LINE_LOOP:						return GL_LINE_LOOP;
		case ZGL_PRIM_LINE_STRIP:						return GL_LINE_STRIP;
		case ZGL_PRIM_LINES:							return GL_LINES;
		case ZGL_PRIM_POINTS:							return GL_POINTS;
		case ZGL_PRIM_TRIANGLE_FAN:						return GL_TRIANGLE_FAN;
		case ZGL_PRIM_TRIANGLE_STRIP:					return GL_TRIANGLE_STRIP;
		case ZGL_PRIM_TRIANGLES:						return GL_TRIANGLES;

		case ZGL_PROGRAM_INFO_ACTIVE_ATTRIBUTES:			return GL_ACTIVE_ATTRIBUTES;
		case ZGL_PROGRAM_INFO_ACTIVE_ATTRIBUTE_MAX_LENGTH:	return GL_ACTIVE_ATTRIBUTE_MAX_LENGTH;
		case ZGL_PROGRAM_INFO_ACTIVE_UNIFORMS:				return GL_ACTIVE_UNIFORMS;
		case ZGL_PROGRAM_INFO_ACTIVE_UNIFORM_MAX_LENGTH:	return GL_ACTIVE_UNIFORM_MAX_LENGTH;
		case ZGL_PROGRAM_INFO_ATTACHED_SHADERS:				return GL_ATTACHED_SHADERS;
		case ZGL_PROGRAM_INFO_DELETE_STATUS:				return GL_DELETE_STATUS;
		case ZGL_PROGRAM_INFO_LOG_LENGTH:					return GL_INFO_LOG_LENGTH;
		case ZGL_PROGRAM_INFO_LINK_STATUS:					return GL_LINK_STATUS;
		case ZGL_PROGRAM_INFO_VALIDATE_STATUS:				return GL_VALIDATE_STATUS;

		case ZGL_SAMPLE_LINEAR:							return GL_LINEAR;
		case ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR:			return GL_LINEAR_MIPMAP_LINEAR;
		case ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST:			return GL_LINEAR_MIPMAP_NEAREST;
		case ZGL_SAMPLE_NEAREST:						return GL_NEAREST;
		case ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR:			return GL_NEAREST_MIPMAP_LINEAR;
		case ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST:			return GL_NEAREST_MIPMAP_NEAREST;

		case ZGL_SHADER_INFO_COMPILE_STATUS:			return GL_COMPILE_STATUS;
		case ZGL_SHADER_INFO_DELETE_STATUS:				return GL_DELETE_STATUS;
		case ZGL_SHADER_INFO_LOG_LENGTH:				return GL_INFO_LOG_LENGTH;
		case ZGL_SHADER_INFO_SOURCE_LENGTH:				return GL_SHADER_SOURCE_LENGTH;
		case ZGL_SHADER_INFO_TYPE:						return GL_SHADER_TYPE;

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_LINUX ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_SHADER_TYPE_TESS_CONTROL:			return GL_TESS_CONTROL_SHADER;
			case ZGL_SHADER_TYPE_TESS_EVALUATION:		return GL_TESS_EVALUATION_SHADER;
		#endif

		case ZGL_SHADER_TYPE_FRAGMENT:					return GL_FRAGMENT_SHADER;

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_SHADER_TYPE_GEOMETRY:				return GL_GEOMETRY_SHADER;
		#endif

		case ZGL_SHADER_TYPE_VERTEX:					return GL_VERTEX_SHADER;

		case ZGL_STRING_VENDOR:							return GL_VENDOR;
		case ZGL_STRING_VERSION:						return GL_VERSION;
		case ZGL_STRING_RENDERER:						return GL_RENDERER;
		case ZGL_STRING_SHADING_LANGUAGE_VERSION:		return GL_SHADING_LANGUAGE_VERSION;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_BASE_LEVEL:				return GL_TEXTURE_BASE_LEVEL;
			case ZGL_TEXTURE_COMPARE_FUNC:				return GL_TEXTURE_COMPARE_FUNC;
			case ZGL_TEXTURE_COMPARE_MODE:				return GL_TEXTURE_COMPARE_MODE;
			//case ZGL_TEXTURE_DEPTH_STENCIL_MODE:		return GL_DEPTH_STENCIL_TEXTURE_MODE;
			case ZGL_TEXTURE_ENV_MODE:					return GL_TEXTURE_ENV_MODE;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_LOD_BIAS:					return GL_TEXTURE_LOD_BIAS;
		#endif

  		case ZGL_TEXTURE_MAG_FILTER:					return GL_TEXTURE_MAG_FILTER;
		case ZGL_TEXTURE_MIN_FILTER:					return GL_TEXTURE_MIN_FILTER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_MAX_LEVEL:					return GL_TEXTURE_MAX_LEVEL;
			case ZGL_TEXTURE_MAX_LOD:					return GL_TEXTURE_MAX_LOD;
			case ZGL_TEXTURE_MIN_LOD:					return GL_TEXTURE_MIN_LOD;
		#endif

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_LINUX ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_SWIZZLE_A:					return GL_TEXTURE_SWIZZLE_A;
			case ZGL_TEXTURE_SWIZZLE_B:					return GL_TEXTURE_SWIZZLE_B;
			case ZGL_TEXTURE_SWIZZLE_G:					return GL_TEXTURE_SWIZZLE_G;
			case ZGL_TEXTURE_SWIZZLE_R:					return GL_TEXTURE_SWIZZLE_R;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TEXTURE_WRAP_R:					return GL_TEXTURE_WRAP_R;
		#endif

		case ZGL_TEXTURE_WRAP_S:						return GL_TEXTURE_WRAP_S;
		case ZGL_TEXTURE_WRAP_T:						return GL_TEXTURE_WRAP_T;

		case ZGL_TYPE_BYTE:								return GL_BYTE;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case ZGL_TYPE_DOUBLE:						return GL_DOUBLE;
		#endif

		case ZGL_TYPE_FLOAT:							return GL_FLOAT;
		case ZGL_TYPE_INT:								return GL_INT;
		case ZGL_TYPE_SHORT:							return GL_SHORT;
		case ZGL_TYPE_UNSIGNED_BYTE:					return GL_UNSIGNED_BYTE;
		case ZGL_TYPE_UNSIGNED_INT:						return GL_UNSIGNED_INT;
		case ZGL_TYPE_UNSIGNED_SHORT:					return GL_UNSIGNED_SHORT;

		case ZGL_WRAP_MODE_CLAMP:						return GL_CLAMP_TO_EDGE;
		case ZGL_WRAP_MODE_REPEAT:						return GL_REPEAT;
	}

	assert ( false );
	return 0;
}
