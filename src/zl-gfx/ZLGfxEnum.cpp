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
 ZLGfxEnum::_ ZLGfxEnum::MapNativeToZL ( u32 value ) {

	switch ( value ) {
		case GL_DST_ALPHA:					return BLEND_FACTOR_DST_ALPHA;
		case GL_DST_COLOR:					return BLEND_FACTOR_DST_COLOR;
		case GL_ONE:						return BLEND_FACTOR_ONE;
		case GL_ONE_MINUS_DST_ALPHA:		return BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case GL_ONE_MINUS_DST_COLOR:		return BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case GL_ONE_MINUS_SRC_ALPHA:		return BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case GL_ONE_MINUS_SRC_COLOR:		return BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case GL_SRC_ALPHA:					return BLEND_FACTOR_SRC_ALPHA;
		case GL_SRC_ALPHA_SATURATE:			return BLEND_FACTOR_SRC_ALPHA_SATURATE;
		case GL_SRC_COLOR:					return BLEND_FACTOR_SRC_COLOR;
		case GL_ZERO:						return BLEND_FACTOR_ZERO;
	};
	assert ( false );
	return NONE;
}

//----------------------------------------------------------------//
u32  ZLGfxEnum::MapZLToNative ( _ value ) {

	switch ( value ) {

		case BLEND_FACTOR_DST_ALPHA:					return GL_DST_ALPHA;
		case BLEND_FACTOR_DST_COLOR:					return GL_DST_COLOR;
		case BLEND_FACTOR_ONE:							return GL_ONE;
		case BLEND_FACTOR_ONE_MINUS_DST_ALPHA:			return GL_ONE_MINUS_DST_ALPHA;
		case BLEND_FACTOR_ONE_MINUS_DST_COLOR:			return GL_ONE_MINUS_DST_COLOR;
		case BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:			return GL_ONE_MINUS_SRC_ALPHA;
		case BLEND_FACTOR_ONE_MINUS_SRC_COLOR:			return GL_ONE_MINUS_SRC_COLOR;
		case BLEND_FACTOR_SRC_ALPHA:					return GL_SRC_ALPHA;
		case BLEND_FACTOR_SRC_ALPHA_SATURATE:			return GL_SRC_ALPHA_SATURATE;
		case BLEND_FACTOR_SRC_COLOR:					return GL_SRC_COLOR;
		case BLEND_FACTOR_ZERO:							return GL_ZERO;

		case BLEND_MODE_ADD:							return GL_FUNC_ADD;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case BLEND_MODE_MAX:						return GL_MAX;
			case BLEND_MODE_MIN:						return GL_MIN;
		#endif

		case BLEND_MODE_REVERSE_SUBTRACT:				return GL_FUNC_REVERSE_SUBTRACT;
		case BLEND_MODE_SUBTRACT:						return GL_FUNC_SUBTRACT;

		case BUFFER_TARGET_ARRAY:						return GL_ARRAY_BUFFER;
		case BUFFER_TARGET_ELEMENT_ARRAY:				return GL_ELEMENT_ARRAY_BUFFER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case BUFFER_USAGE_DYNAMIC_COPY:				return GL_DYNAMIC_COPY;
		#endif

		case BUFFER_USAGE_DYNAMIC_DRAW:					return GL_DYNAMIC_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case BUFFER_USAGE_DYNAMIC_READ:				return GL_DYNAMIC_READ;
			case BUFFER_USAGE_STATIC_COPY:				return GL_STATIC_COPY;
		#endif

		case BUFFER_USAGE_STATIC_DRAW:					return GL_STATIC_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case BUFFER_USAGE_STATIC_READ:				return GL_STATIC_READ;
			case BUFFER_USAGE_STREAM_COPY:				return GL_STREAM_COPY;
		#endif

		case BUFFER_USAGE_STREAM_DRAW:					return GL_STREAM_DRAW;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case BUFFER_USAGE_STREAM_READ:				return GL_STREAM_READ;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_BLACKBERRY )
			case COMPOSE_MODULATE:						return GL_MODULATE;
		#endif

		case CULL_ALL:									return GL_FRONT_AND_BACK;
		case CULL_BACK:									return GL_BACK;
		case CULL_FRONT:								return GL_FRONT;

		case DEPTH_ALWAYS:								return GL_ALWAYS;
		case DEPTH_EQUAL:								return GL_EQUAL;
		case DEPTH_LESS:								return GL_LESS;
		case DEPTH_LEQUAL:								return GL_LEQUAL;
		case DEPTH_GEQUAL:								return GL_GEQUAL;
		case DEPTH_GREATER:								return GL_GREATER;
		case DEPTH_NEVER:								return GL_NEVER;
		case DEPTH_NOTEQUAL:							return GL_NOTEQUAL;

		case FRAMEBUFFER_ATTACHMENT_COLOR:				return GL_COLOR_ATTACHMENT0;
		case FRAMEBUFFER_ATTACHMENT_DEPTH:				return GL_DEPTH_ATTACHMENT;
		case FRAMEBUFFER_ATTACHMENT_STENCIL:			return GL_STENCIL_ATTACHMENT;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case FRAMEBUFFER_TARGET_DRAW:				return GL_DRAW_FRAMEBUFFER;
			case FRAMEBUFFER_TARGET_READ:				return GL_READ_FRAMEBUFFER;
		#endif

		case FRAMEBUFFER_TARGET_DRAW_READ:				return GL_FRAMEBUFFER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case MATRIX_COLOR:							return GL_COLOR;
			case MATRIX_MODELVIEW:						return GL_MODELVIEW;
			case MATRIX_PROJECTION:						return GL_PROJECTION;
		#endif

		case MATRIX_TEXTURE:							return GL_TEXTURE;

		case PIPELINE_BLEND:							return GL_BLEND;

		#if !defined ( MOAI_OS_NACL )
		  case PIPELINE_COLOR_ARRAY:					return GL_COLOR_ARRAY;
		#endif
		case PIPELINE_CULL:								return GL_CULL_FACE;
		case PIPELINE_DEPTH:							return GL_DEPTH_TEST;

		#if !defined ( MOAI_OS_NACL )
			case PIPELINE_NORMAL_ARRAY:					return GL_NORMAL_ARRAY;
		#endif

		case PIPELINE_SCISSOR:							return GL_SCISSOR_TEST;
		case PIPELINE_TEXTURE_2D:						return GL_TEXTURE_2D;

		#if !defined ( MOAI_OS_NACL )
			case PIPELINE_TEXTURE_COORD_ARRAY:			return GL_TEXTURE_COORD_ARRAY;
			case PIPELINE_VERTEX_ARRAY:					return GL_VERTEX_ARRAY;
		#endif

		#if !defined ( MOAI_OS_IPHONE )
			//case COLOR_INDEX:								return GL_COLOR_INDEX;
			//case STENCIL_INDEX:								return GL_STENCIL_INDEX;
		#endif
		
		case DEPTH_COMPONENT:							return GL_DEPTH_COMPONENT;

		case PIXEL_FORMAT_ALPHA:						return GL_ALPHA;
		case PIXEL_FORMAT_LUMINANCE:					return GL_LUMINANCE;
		case PIXEL_FORMAT_LUMINANCE_ALPHA:				return GL_LUMINANCE_ALPHA;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case PIXEL_FORMAT_RED:						return GL_RED;
			case PIXEL_FORMAT_RG:						return GL_RG;
		#endif

		case PIXEL_FORMAT_RGB:							return GL_RGB;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case PIXEL_FORMAT_RGB4:						return GL_RGB4;
		#endif

		case PIXEL_FORMAT_RGB5_A1:						return GL_RGB5_A1;

		#if defined ( MOAI_OS_ANDROID ) || defined ( MOAI_OS_HTML )
			case PIXEL_FORMAT_RGB565:					return GL_RGB565;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case PIXEL_FORMAT_RGB8:						return GL_RGB8;
			case PIXEL_FORMAT_BGR:						return GL_BGR;
		#endif

		case PIXEL_FORMAT_RGBA:							return GL_RGBA;
		case PIXEL_FORMAT_RGBA4:						return GL_RGBA4;

		#if !defined ( MOAI_OS_ANDROID )
			case PIXEL_FORMAT_RGBA8:					return GL_RGBA8;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case PIXEL_FORMAT_BGRA:						return GL_BGRA;
		#endif

		case PIXEL_TYPE_BYTE:							return GL_BYTE;

		#ifdef MOAI_OS_IPHONE
			case PIXEL_TYPE_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:	return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
			case PIXEL_TYPE_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:	return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
			case PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:	return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
			case PIXEL_TYPE_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:	return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		#endif

		case PIXEL_TYPE_FLOAT:							return GL_FLOAT;
		case PIXEL_TYPE_INT:							return GL_INT;
		case PIXEL_TYPE_SHORT:							return GL_SHORT;
		case PIXEL_TYPE_UNSIGNED_BYTE:					return GL_UNSIGNED_BYTE;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case PIXEL_TYPE_UNSIGNED_BYTE_2_3_3_REV:	return GL_UNSIGNED_BYTE_2_3_3_REV;
			case PIXEL_TYPE_UNSIGNED_BYTE_3_3_2:		return GL_UNSIGNED_BYTE_3_3_2;
		#endif

		case PIXEL_TYPE_UNSIGNED_INT:					return GL_UNSIGNED_INT;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case PIXEL_TYPE_UNSIGNED_INT_8_8_8_8:			return GL_UNSIGNED_INT_8_8_8_8;
			case PIXEL_TYPE_UNSIGNED_INT_8_8_8_8_REV:		return GL_UNSIGNED_INT_8_8_8_8_REV;
			case PIXEL_TYPE_UNSIGNED_INT_2_10_10_10_REV:	return GL_UNSIGNED_INT_2_10_10_10_REV;
			case PIXEL_TYPE_UNSIGNED_INT_10_10_10_2:		return GL_UNSIGNED_INT_10_10_10_2;
		#endif

		case PIXEL_TYPE_UNSIGNED_SHORT:					return GL_UNSIGNED_SHORT;
		case PIXEL_TYPE_UNSIGNED_SHORT_5_6_5:			return GL_UNSIGNED_SHORT_5_6_5;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case PIXEL_TYPE_UNSIGNED_SHORT_5_6_5_REV:	return GL_UNSIGNED_SHORT_5_6_5_REV;
		#endif

		case PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4:			return GL_UNSIGNED_SHORT_4_4_4_4;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case PIXEL_TYPE_UNSIGNED_SHORT_4_4_4_4_REV:		return GL_UNSIGNED_SHORT_4_4_4_4_REV;
			case PIXEL_TYPE_UNSIGNED_SHORT_1_5_5_5_REV:		return GL_UNSIGNED_SHORT_1_5_5_5_REV;
		#endif

		case PIXEL_TYPE_UNSIGNED_SHORT_5_5_5_1:		return GL_UNSIGNED_SHORT_5_5_5_1;

		case PRIM_LINE_LOOP:						return GL_LINE_LOOP;
		case PRIM_LINE_STRIP:						return GL_LINE_STRIP;
		case PRIM_LINES:							return GL_LINES;
		case PRIM_POINTS:							return GL_POINTS;
		case PRIM_TRIANGLE_FAN:						return GL_TRIANGLE_FAN;
		case PRIM_TRIANGLE_STRIP:					return GL_TRIANGLE_STRIP;
		case PRIM_TRIANGLES:						return GL_TRIANGLES;

		case PROGRAM_INFO_ACTIVE_ATTRIBUTES:			return GL_ACTIVE_ATTRIBUTES;
		case PROGRAM_INFO_ACTIVE_ATTRIBUTE_MAX_LENGTH:	return GL_ACTIVE_ATTRIBUTE_MAX_LENGTH;
		case PROGRAM_INFO_ACTIVE_UNIFORMS:				return GL_ACTIVE_UNIFORMS;
		case PROGRAM_INFO_ACTIVE_UNIFORM_MAX_LENGTH:	return GL_ACTIVE_UNIFORM_MAX_LENGTH;
		case PROGRAM_INFO_ATTACHED_SHADERS:				return GL_ATTACHED_SHADERS;
		case PROGRAM_INFO_DELETE_STATUS:				return GL_DELETE_STATUS;
		case PROGRAM_INFO_LOG_LENGTH:					return GL_INFO_LOG_LENGTH;
		case PROGRAM_INFO_LINK_STATUS:					return GL_LINK_STATUS;
		case PROGRAM_INFO_VALIDATE_STATUS:				return GL_VALIDATE_STATUS;

		case SAMPLE_LINEAR:							return GL_LINEAR;
		case SAMPLE_LINEAR_MIPMAP_LINEAR:			return GL_LINEAR_MIPMAP_LINEAR;
		case SAMPLE_LINEAR_MIPMAP_NEAREST:			return GL_LINEAR_MIPMAP_NEAREST;
		case SAMPLE_NEAREST:						return GL_NEAREST;
		case SAMPLE_NEAREST_MIPMAP_LINEAR:			return GL_NEAREST_MIPMAP_LINEAR;
		case SAMPLE_NEAREST_MIPMAP_NEAREST:			return GL_NEAREST_MIPMAP_NEAREST;

		case SHADER_INFO_COMPILE_STATUS:			return GL_COMPILE_STATUS;
		case SHADER_INFO_DELETE_STATUS:				return GL_DELETE_STATUS;
		case SHADER_INFO_LOG_LENGTH:				return GL_INFO_LOG_LENGTH;
		case SHADER_INFO_SOURCE_LENGTH:				return GL_SHADER_SOURCE_LENGTH;
		case SHADER_INFO_TYPE:						return GL_SHADER_TYPE;

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_LINUX ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case SHADER_TYPE_TESS_CONTROL:			return GL_TESS_CONTROL_SHADER;
			case SHADER_TYPE_TESS_EVALUATION:		return GL_TESS_EVALUATION_SHADER;
		#endif

		case SHADER_TYPE_FRAGMENT:					return GL_FRAGMENT_SHADER;

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case SHADER_TYPE_GEOMETRY:				return GL_GEOMETRY_SHADER;
		#endif

		case SHADER_TYPE_VERTEX:					return GL_VERTEX_SHADER;

		case STRING_VENDOR:							return GL_VENDOR;
		case STRING_VERSION:						return GL_VERSION;
		case STRING_RENDERER:						return GL_RENDERER;
		case STRING_SHADING_LANGUAGE_VERSION:		return GL_SHADING_LANGUAGE_VERSION;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case TEXTURE_BASE_LEVEL:				return GL_TEXTURE_BASE_LEVEL;
			case TEXTURE_COMPARE_FUNC:				return GL_TEXTURE_COMPARE_FUNC;
			case TEXTURE_COMPARE_MODE:				return GL_TEXTURE_COMPARE_MODE;
			//case TEXTURE_DEPTH_STENCIL_MODE:		return GL_DEPTH_STENCIL_TEXTURE_MODE;
			case TEXTURE_ENV_MODE:					return GL_TEXTURE_ENV_MODE;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case TEXTURE_LOD_BIAS:					return GL_TEXTURE_LOD_BIAS;
		#endif

  		case TEXTURE_MAG_FILTER:					return GL_TEXTURE_MAG_FILTER;
		case TEXTURE_MIN_FILTER:					return GL_TEXTURE_MIN_FILTER;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case TEXTURE_MAX_LEVEL:					return GL_TEXTURE_MAX_LEVEL;
			case TEXTURE_MAX_LOD:					return GL_TEXTURE_MAX_LOD;
			case TEXTURE_MIN_LOD:					return GL_TEXTURE_MIN_LOD;
		#endif

		#if !defined ( MOAI_OS_OSX ) && !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_LINUX ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case TEXTURE_SWIZZLE_A:					return GL_TEXTURE_SWIZZLE_A;
			case TEXTURE_SWIZZLE_B:					return GL_TEXTURE_SWIZZLE_B;
			case TEXTURE_SWIZZLE_G:					return GL_TEXTURE_SWIZZLE_G;
			case TEXTURE_SWIZZLE_R:					return GL_TEXTURE_SWIZZLE_R;
		#endif

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case TEXTURE_WRAP_R:					return GL_TEXTURE_WRAP_R;
		#endif

		case TEXTURE_WRAP_S:						return GL_TEXTURE_WRAP_S;
		case TEXTURE_WRAP_T:						return GL_TEXTURE_WRAP_T;

		case TYPE_BYTE:								return GL_BYTE;

		#if !defined ( MOAI_OS_NACL ) && !defined ( MOAI_OS_IPHONE ) && !defined ( MOAI_OS_BLACKBERRY ) && !defined ( MOAI_OS_ANDROID ) && !defined ( MOAI_OS_HTML )
			case TYPE_DOUBLE:						return GL_DOUBLE;
		#endif

		case TYPE_FLOAT:							return GL_FLOAT;
		case TYPE_INT:								return GL_INT;
		case TYPE_SHORT:							return GL_SHORT;
		case TYPE_UNSIGNED_BYTE:					return GL_UNSIGNED_BYTE;
		case TYPE_UNSIGNED_INT:						return GL_UNSIGNED_INT;
		case TYPE_UNSIGNED_SHORT:					return GL_UNSIGNED_SHORT;

		case WRAP_MODE_CLAMP:						return GL_CLAMP_TO_EDGE;
		case WRAP_MODE_REPEAT:						return GL_REPEAT;
	}

	assert ( false );
	return 0;
}
