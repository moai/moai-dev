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
//	static ZLGfxEnum::_ Remap ( MOAIBlendFactorEnum::_ value ) {
//
//		switch ( value ) {
//			case MOAIBlendFactorEnum::DST_ALPHA:					return ZLGfxEnum::BLEND_FACTOR_DST_ALPHA;
//			case MOAIBlendFactorEnum::DST_COLOR:					return ZLGfxEnum::BLEND_FACTOR_DST_COLOR;
//			case MOAIBlendFactorEnum::ONE:							return ZLGfxEnum::BLEND_FACTOR_ONE;
//			case MOAIBlendFactorEnum::ONE_MINUS_DST_ALPHA:			return ZLGfxEnum::BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
//			case MOAIBlendFactorEnum::ONE_MINUS_DST_COLOR:			return ZLGfxEnum::BLEND_FACTOR_ONE_MINUS_DST_COLOR;
//			case MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA:			return ZLGfxEnum::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//			case MOAIBlendFactorEnum::ONE_MINUS_SRC_COLOR:			return ZLGfxEnum::BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
//			case MOAIBlendFactorEnum::SRC_ALPHA:					return ZLGfxEnum::BLEND_FACTOR_SRC_ALPHA;
//			case MOAIBlendFactorEnum::SRC_ALPHA_SATURATE:			return ZLGfxEnum::BLEND_FACTOR_SRC_ALPHA_SATURATE;
//			case MOAIBlendFactorEnum::SRC_COLOR:					return ZLGfxEnum::BLEND_FACTOR_SRC_COLOR;
//			case MOAIBlendFactorEnum::ZERO:							return ZLGfxEnum::BLEND_FACTOR_ZERO;
//		}
//		assert ( false );
//		return ZLGfxEnum::NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZLGfxEnum::_ Remap ( MOAIBlendFuncEnum::_ value ) {
//
//		switch ( value ) {
//			case MOAIBlendFuncEnum::ADD:								return ZLGfxEnum::BLEND_MODE_ADD;
//			case MOAIBlendFuncEnum::MAX:								return ZLGfxEnum::BLEND_MODE_MAX;
//			case MOAIBlendFuncEnum::MIN:								return ZLGfxEnum::BLEND_MODE_MIN;
//			case MOAIBlendFuncEnum::REVERSE_SUBTRACT:					return ZLGfxEnum::BLEND_MODE_REVERSE_SUBTRACT;
//			case MOAIBlendFuncEnum::SUBTRACT:							return ZLGfxEnum::BLEND_MODE_SUBTRACT;
//		}
//		assert ( false );
//		return ZLGfxEnum::NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZLGfxEnum::_ Remap ( MOAICullFuncEnum::_ value ) {
//
//		switch ( value ) {
//			case MOAICullFuncEnum::NONE:							return ZLGfxEnum::NONE;
//			case MOAICullFuncEnum::ALL:					return ZLGfxEnum::CULL_ALL;
//			case MOAICullFuncEnum::BACK:					return ZLGfxEnum::CULL_BACK;
//			case MOAICullFuncEnum::FRONT:					return ZLGfxEnum::CULL_FRONT;
//		}
//		assert ( false );
//		return ZLGfxEnum::NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZLGfxEnum::_ Remap ( MOAIDepthFuncEnum::_ value ) {
//
//		switch ( value ) {
//			case MOAIDepthFuncEnum::NONE:							return ZLGfxEnum::NONE;
//			case MOAIDepthFuncEnum::ALWAYS:				return ZLGfxEnum::DEPTH_ALWAYS;
//			case MOAIDepthFuncEnum::EQUAL:				return ZLGfxEnum::DEPTH_EQUAL;
//			case MOAIDepthFuncEnum::LESS:					return ZLGfxEnum::DEPTH_LESS;
//			case MOAIDepthFuncEnum::LEQUAL:				return ZLGfxEnum::DEPTH_LEQUAL;
//			case MOAIDepthFuncEnum::GEQUAL:				return ZLGfxEnum::DEPTH_GEQUAL;
//			case MOAIDepthFuncEnum::GREATER:				return ZLGfxEnum::DEPTH_GREATER;
//			case MOAIDepthFuncEnum::NEVER:				return ZLGfxEnum::DEPTH_NEVER;
//			case MOAIDepthFuncEnum::NOTEQUAL:				return ZLGfxEnum::DEPTH_NOTEQUAL;
//		}
//		assert ( false );
//		return ZLGfxEnum::NONE;
//	};

	//----------------------------------------------------------------//
	static VkPrimitiveTopology Remap ( MOAIGfxTopologyEnum::_ value ) {

		switch ( value ) {
			case MOAIGfxTopologyEnum::LINE_LIST:						return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case MOAIGfxTopologyEnum::LINE_LOOP:						assert ( false );
			case MOAIGfxTopologyEnum::LINE_STRIP:						return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case MOAIGfxTopologyEnum::POINT_LIST:						return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case MOAIGfxTopologyEnum::TRIANGLE_FAN:						return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			case MOAIGfxTopologyEnum::TRIANGLE_LIST:					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case MOAIGfxTopologyEnum::TRIANGLE_STRIP:					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		}
		assert ( false );
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}
		
//	//----------------------------------------------------------------//
//	static ZLGfxEnum::_ Remap ( MOAIGfxTypeEnum::_ value ) {
//
//		switch ( value ) {
//			case MOAIGfxTypeEnum::BYTE:								return ZLGfxEnum::TYPE_BYTE;
//			case MOAIGfxTypeEnum::DOUBLE:							return ZLGfxEnum::TYPE_DOUBLE;
//			case MOAIGfxTypeEnum::FLOAT:							return ZLGfxEnum::TYPE_FLOAT;
//			case MOAIGfxTypeEnum::INT:								return ZLGfxEnum::TYPE_INT;
//			case MOAIGfxTypeEnum::SHORT:							return ZLGfxEnum::TYPE_SHORT;
//			case MOAIGfxTypeEnum::UNSIGNED_BYTE:					return ZLGfxEnum::TYPE_UNSIGNED_BYTE;
//			case MOAIGfxTypeEnum::UNSIGNED_INT:						return ZLGfxEnum::TYPE_UNSIGNED_INT;
//			case MOAIGfxTypeEnum::UNSIGNED_SHORT:					return ZLGfxEnum::TYPE_UNSIGNED_SHORT;
//		}
//		assert ( false );
//		return ZLGfxEnum::NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZLGfxEnum::_ Remap ( MOAITextureFilterEnum::_ value ) {
//
//		switch ( value ) {
//			case MOAITextureFilterEnum::LINEAR:						return ZLGfxEnum::SAMPLE_LINEAR;
//			case MOAITextureFilterEnum::LINEAR_MIPMAP_LINEAR:		return ZLGfxEnum::SAMPLE_LINEAR_MIPMAP_LINEAR;
//			case MOAITextureFilterEnum::LINEAR_MIPMAP_NEAREST:		return ZLGfxEnum::SAMPLE_LINEAR_MIPMAP_NEAREST;
//			case MOAITextureFilterEnum::NEAREST:					return ZLGfxEnum::SAMPLE_NEAREST;
//			case MOAITextureFilterEnum::NEAREST_MIPMAP_LINEAR:		return ZLGfxEnum::SAMPLE_NEAREST_MIPMAP_LINEAR;
//			case MOAITextureFilterEnum::NEAREST_MIPMAP_NEAREST:		return ZLGfxEnum::SAMPLE_NEAREST_MIPMAP_NEAREST;
//		}
//		assert ( false );
//		return ZLGfxEnum::NONE;
//	};
//
//	//----------------------------------------------------------------//
//	static ZLGfxEnum::_ Remap ( MOAITextureWrapEnum::_ value ) {
//
//		switch ( value ) {
//			case MOAITextureWrapEnum::CLAMP:						return ZLGfxEnum::WRAP_MODE_CLAMP;
//			case MOAITextureWrapEnum::REPEAT:						return ZLGfxEnum::WRAP_MODE_REPEAT;
//		}
//		assert ( false );
//		return ZLGfxEnum::NONE;
//	};

};

#endif
