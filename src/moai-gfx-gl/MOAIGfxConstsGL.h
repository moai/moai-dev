// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCONSTSGL_H
#define	MOAIGFXCONSTSGL_H

//================================================================//
// MOAIGfxConstsGL
//================================================================//
class MOAIGfxConstsGL {
public:

	//----------------------------------------------------------------//
	static ZLGfxEnum::Type Remap ( MOAIBlendFactor::Type value ) {
	
		switch ( value ) {
			case MOAIBlendFactor::DST_ALPHA:					return ZLGfxEnum::BLEND_FACTOR_DST_ALPHA;
			case MOAIBlendFactor::DST_COLOR:					return ZLGfxEnum::BLEND_FACTOR_DST_COLOR;
			case MOAIBlendFactor::ONE:							return ZLGfxEnum::BLEND_FACTOR_ONE;
			case MOAIBlendFactor::ONE_MINUS_DST_ALPHA:			return ZLGfxEnum::BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case MOAIBlendFactor::ONE_MINUS_DST_COLOR:			return ZLGfxEnum::BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case MOAIBlendFactor::ONE_MINUS_SRC_ALPHA:			return ZLGfxEnum::BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case MOAIBlendFactor::ONE_MINUS_SRC_COLOR:			return ZLGfxEnum::BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case MOAIBlendFactor::SRC_ALPHA:					return ZLGfxEnum::BLEND_FACTOR_SRC_ALPHA;
			case MOAIBlendFactor::SRC_ALPHA_SATURATE:			return ZLGfxEnum::BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case MOAIBlendFactor::SRC_COLOR:					return ZLGfxEnum::BLEND_FACTOR_SRC_COLOR;
			case MOAIBlendFactor::ZERO:							return ZLGfxEnum::BLEND_FACTOR_ZERO;
		}
		assert ( false );
		return ZLGfxEnum::NONE;
	};

	//----------------------------------------------------------------//
	static ZLGfxEnum::Type Remap ( MOAIBlendFunc::Type value ) {
	
		switch ( value ) {
			case MOAIBlendFunc::ADD:								return ZLGfxEnum::BLEND_MODE_ADD;
			case MOAIBlendFunc::MAX:								return ZLGfxEnum::BLEND_MODE_MAX;
			case MOAIBlendFunc::MIN:								return ZLGfxEnum::BLEND_MODE_MIN;
			case MOAIBlendFunc::REVERSE_SUBTRACT:					return ZLGfxEnum::BLEND_MODE_REVERSE_SUBTRACT;
			case MOAIBlendFunc::SUBTRACT:							return ZLGfxEnum::BLEND_MODE_SUBTRACT;
		}
		assert ( false );
		return ZLGfxEnum::NONE;
	};

	//----------------------------------------------------------------//
	static ZLGfxEnum::Type Remap ( MOAICullFunc::Type value ) {
	
		switch ( value ) {
			case MOAICullFunc::NONE:							return ZLGfxEnum::NONE;
			case MOAICullFunc::ALL:								return ZLGfxEnum::CULL_ALL;
			case MOAICullFunc::BACK:							return ZLGfxEnum::CULL_BACK;
			case MOAICullFunc::FRONT:							return ZLGfxEnum::CULL_FRONT;
		}
		assert ( false );
		return ZLGfxEnum::NONE;
	};

	//----------------------------------------------------------------//
	static ZLGfxEnum::Type Remap ( MOAIDepthFunc::Type value ) {
	
		switch ( value ) {
			case MOAIDepthFunc::NONE:							return ZLGfxEnum::NONE;
			case MOAIDepthFunc::ALWAYS:							return ZLGfxEnum::DEPTH_ALWAYS;
			case MOAIDepthFunc::EQUAL:							return ZLGfxEnum::DEPTH_EQUAL;
			case MOAIDepthFunc::LESS:							return ZLGfxEnum::DEPTH_LESS;
			case MOAIDepthFunc::LEQUAL:							return ZLGfxEnum::DEPTH_LEQUAL;
			case MOAIDepthFunc::GEQUAL:							return ZLGfxEnum::DEPTH_GEQUAL;
			case MOAIDepthFunc::GREATER:						return ZLGfxEnum::DEPTH_GREATER;
			case MOAIDepthFunc::NEVER:							return ZLGfxEnum::DEPTH_NEVER;
			case MOAIDepthFunc::NOTEQUAL:						return ZLGfxEnum::DEPTH_NOTEQUAL;
		}
		assert ( false );
		return ZLGfxEnum::NONE;
	};

	//----------------------------------------------------------------//
	static ZLGfxEnum::Type Remap ( MOAIGfxType::Type value ) {
	
		switch ( value ) {
			case MOAIGfxType::BYTE:								return ZLGfxEnum::TYPE_BYTE;
			case MOAIGfxType::DOUBLE:							return ZLGfxEnum::TYPE_DOUBLE;
			case MOAIGfxType::FLOAT:							return ZLGfxEnum::TYPE_FLOAT;
			case MOAIGfxType::INT:								return ZLGfxEnum::TYPE_INT;
			case MOAIGfxType::SHORT:							return ZLGfxEnum::TYPE_SHORT;
			case MOAIGfxType::UNSIGNED_BYTE:					return ZLGfxEnum::TYPE_UNSIGNED_BYTE;
			case MOAIGfxType::UNSIGNED_INT:						return ZLGfxEnum::TYPE_UNSIGNED_INT;
			case MOAIGfxType::UNSIGNED_SHORT:					return ZLGfxEnum::TYPE_UNSIGNED_SHORT;
		}
		assert ( false );
		return ZLGfxEnum::NONE;
	};

	//----------------------------------------------------------------//
	static ZLGfxEnum::Type Remap ( MOAITextureFilter::Type value ) {
	
		switch ( value ) {
			case MOAITextureFilter::LINEAR:						return ZLGfxEnum::SAMPLE_LINEAR;
			case MOAITextureFilter::LINEAR_MIPMAP_LINEAR:		return ZLGfxEnum::SAMPLE_LINEAR_MIPMAP_LINEAR;
			case MOAITextureFilter::LINEAR_MIPMAP_NEAREST:		return ZLGfxEnum::SAMPLE_LINEAR_MIPMAP_NEAREST;
			case MOAITextureFilter::NEAREST:					return ZLGfxEnum::SAMPLE_NEAREST;
			case MOAITextureFilter::NEAREST_MIPMAP_LINEAR:		return ZLGfxEnum::SAMPLE_NEAREST_MIPMAP_LINEAR;
			case MOAITextureFilter::NEAREST_MIPMAP_NEAREST:		return ZLGfxEnum::SAMPLE_NEAREST_MIPMAP_NEAREST;
		}
		assert ( false );
		return ZLGfxEnum::NONE;
	};

	//----------------------------------------------------------------//
	static ZLGfxEnum::Type Remap ( MOAITextureWrap::Type value ) {
	
		switch ( value ) {
			case MOAITextureWrap::CLAMP:						return ZLGfxEnum::WRAP_MODE_CLAMP;
			case MOAITextureWrap::REPEAT:						return ZLGfxEnum::WRAP_MODE_REPEAT;
		}
		assert ( false );
		return ZLGfxEnum::NONE;
	};

	//----------------------------------------------------------------//
	static ZLGfxEnum::Type Remap ( MOAITopology::Type value ) {
	
		switch ( value ) {
			case MOAITopology::LINE_LIST:						return ZLGfxEnum::PRIM_LINES;
			case MOAITopology::LINE_LOOP:						return ZLGfxEnum::PRIM_LINE_LOOP;
			case MOAITopology::LINE_STRIP:						return ZLGfxEnum::PRIM_LINE_STRIP;
			case MOAITopology::POINT_LIST:						return ZLGfxEnum::PRIM_POINTS;
			case MOAITopology::TRIANGLE_FAN:					return ZLGfxEnum::PRIM_TRIANGLE_FAN;
			case MOAITopology::TRIANGLE_LIST:					return ZLGfxEnum::PRIM_TRIANGLES;
			case MOAITopology::TRIANGLE_STRIP:					return ZLGfxEnum::PRIM_TRIANGLE_STRIP;
		}
		assert ( false );
		return ZLGfxEnum::NONE;
	}
};

#endif
