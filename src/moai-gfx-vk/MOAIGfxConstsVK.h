// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCONSTSVK_H
#define	MOAIGFXCONSTSVK_H

//================================================================//
// MOAIGfxConstsGL
//================================================================//
class MOAIGfxConstsVK {
public:

//	//----------------------------------------------------------------//
//	static ZGLEnum Remap ( MOAIBlendFactor::Type value ) {
//
//		switch ( value ) {
//			case MOAIBlendFactor::DST_ALPHA:					return ZGL_BLEND_FACTOR_DST_ALPHA;
//			case MOAIBlendFactor::DST_COLOR:					return ZGL_BLEND_FACTOR_DST_COLOR;
//			case MOAIBlendFactor::ONE:							return ZGL_BLEND_FACTOR_ONE;
//			case MOAIBlendFactor::ONE_MINUS_DST_ALPHA:			return ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
//			case MOAIBlendFactor::ONE_MINUS_DST_COLOR:			return ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
//			case MOAIBlendFactor::ONE_MINUS_SRC_ALPHA:			return ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//			case MOAIBlendFactor::ONE_MINUS_SRC_COLOR:			return ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
//			case MOAIBlendFactor::SRC_ALPHA:					return ZGL_BLEND_FACTOR_SRC_ALPHA;
//			case MOAIBlendFactor::SRC_ALPHA_SATURATE:			return ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE;
//			case MOAIBlendFactor::SRC_COLOR:					return ZGL_BLEND_FACTOR_SRC_COLOR;
//			case MOAIBlendFactor::ZERO:							return ZGL_BLEND_FACTOR_ZERO;
//		}
//		assert ( false );
//		return ZGL_NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZGLEnum Remap ( MOAIBlendFunc::Type value ) {
//
//		switch ( value ) {
//			case MOAIBlendFunc::ADD:								return ZGL_BLEND_MODE_ADD;
//			case MOAIBlendFunc::MAX:								return ZGL_BLEND_MODE_MAX;
//			case MOAIBlendFunc::MIN:								return ZGL_BLEND_MODE_MIN;
//			case MOAIBlendFunc::REVERSE_SUBTRACT:					return ZGL_BLEND_MODE_REVERSE_SUBTRACT;
//			case MOAIBlendFunc::SUBTRACT:							return ZGL_BLEND_MODE_SUBTRACT;
//		}
//		assert ( false );
//		return ZGL_NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZGLEnum Remap ( MOAICullFunc::Type value ) {
//
//		switch ( value ) {
//			case MOAICullFunc::NONE:							return ZGL_NONE;
//			case MOAICullFunc::ALL:					return ZGL_CULL_ALL;
//			case MOAICullFunc::BACK:					return ZGL_CULL_BACK;
//			case MOAICullFunc::FRONT:					return ZGL_CULL_FRONT;
//		}
//		assert ( false );
//		return ZGL_NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZGLEnum Remap ( MOAIDepthFunc::Type value ) {
//
//		switch ( value ) {
//			case MOAIDepthFunc::NONE:							return ZGL_NONE;
//			case MOAIDepthFunc::ALWAYS:				return ZGL_DEPTH_ALWAYS;
//			case MOAIDepthFunc::EQUAL:				return ZGL_DEPTH_EQUAL;
//			case MOAIDepthFunc::LESS:					return ZGL_DEPTH_LESS;
//			case MOAIDepthFunc::LEQUAL:				return ZGL_DEPTH_LEQUAL;
//			case MOAIDepthFunc::GEQUAL:				return ZGL_DEPTH_GEQUAL;
//			case MOAIDepthFunc::GREATER:				return ZGL_DEPTH_GREATER;
//			case MOAIDepthFunc::NEVER:				return ZGL_DEPTH_NEVER;
//			case MOAIDepthFunc::NOTEQUAL:				return ZGL_DEPTH_NOTEQUAL;
//		}
//		assert ( false );
//		return ZGL_NONE;
//	};
//
//
//	//----------------------------------------------------------------//
//	static ZGLEnum Remap ( MOAIGfxType::Type value ) {
//
//		switch ( value ) {
//			case MOAIGfxType::BYTE:								return ZGL_TYPE_BYTE;
//			case MOAIGfxType::DOUBLE:							return ZGL_TYPE_DOUBLE;
//			case MOAIGfxType::FLOAT:							return ZGL_TYPE_FLOAT;
//			case MOAIGfxType::INT:								return ZGL_TYPE_INT;
//			case MOAIGfxType::SHORT:							return ZGL_TYPE_SHORT;
//			case MOAIGfxType::UNSIGNED_BYTE:					return ZGL_TYPE_UNSIGNED_BYTE;
//			case MOAIGfxType::UNSIGNED_INT:						return ZGL_TYPE_UNSIGNED_INT;
//			case MOAIGfxType::UNSIGNED_SHORT:					return ZGL_TYPE_UNSIGNED_SHORT;
//		}
//		assert ( false );
//		return ZGL_NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZGLEnum Remap ( MOAITextureFilter::Type value ) {
//
//		switch ( value ) {
//			case MOAITextureFilter::LINEAR:						return ZGL_SAMPLE_LINEAR;
//			case MOAITextureFilter::LINEAR_MIPMAP_LINEAR:		return ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR;
//			case MOAITextureFilter::LINEAR_MIPMAP_NEAREST:		return ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST;
//			case MOAITextureFilter::NEAREST:					return ZGL_SAMPLE_NEAREST;
//			case MOAITextureFilter::NEAREST_MIPMAP_LINEAR:		return ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR;
//			case MOAITextureFilter::NEAREST_MIPMAP_NEAREST:		return ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST;
//		}
//		assert ( false );
//		return ZGL_NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZGLEnum Remap ( MOAITextureWrap::Type value ) {
//
//		switch ( value ) {
//			case MOAITextureWrap::CLAMP:						return ZGL_WRAP_MODE_CLAMP;
//			case MOAITextureWrap::REPEAT:						return ZGL_WRAP_MODE_REPEAT;
//		}
//		assert ( false );
//		return ZGL_NONE;
//	};

	//----------------------------------------------------------------//
	static VkPrimitiveTopology Remap ( MOAITopology::Type value ) {
	
		switch ( value ) {
			case MOAITopology::LINE_LIST:						return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case MOAITopology::LINE_LOOP:						assert ( false );
			case MOAITopology::LINE_STRIP:						return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case MOAITopology::POINT_LIST:						return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case MOAITopology::TRIANGLE_FAN:					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			case MOAITopology::TRIANGLE_LIST:					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case MOAITopology::TRIANGLE_STRIP:					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		}
		assert ( false );
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}
};

#endif
