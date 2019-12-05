// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCONSTS_H
#define	MOAIGFXCONSTS_H

//================================================================//
// MOAIBlendFactor
//================================================================//
namespace MOAIBlendFactor {
	enum Type {
		DST_ALPHA,
		DST_COLOR,
		ONE,
		ONE_MINUS_DST_ALPHA,
		ONE_MINUS_DST_COLOR,
		ONE_MINUS_SRC_ALPHA,
		ONE_MINUS_SRC_COLOR,
		SRC_ALPHA,
		SRC_ALPHA_SATURATE,
		SRC_COLOR,
		ZERO,
		UNKNOWN,
	};
};

//================================================================//
// MOAIBlendFunc
//================================================================//
namespace MOAIBlendFunc {
	enum Type {
		ADD,
		MAX,
		MIN,
		REVERSE_SUBTRACT,
		SUBTRACT,
		UNKNOWN,
	};
};

//================================================================//
// MOAIClearFlags
//================================================================//
namespace MOAIClearFlags {
	enum {
		COLOR_BUFFER_BIT		= 0x01,
		DEPTH_BUFFER_BIT		= 0x02,
		STENCIL_BUFFER_BIT		= 0x04,
	};
};

//================================================================//
// MOAICullFunc
//================================================================//
namespace MOAICullFunc {
	enum Type {
		NONE,
		ALL,
		BACK,
		FRONT,
		UNKNOWN,
	};
};

//================================================================//
// MOAIDepthFunc
//================================================================//
namespace MOAIDepthFunc {
	enum Type {
		NONE,
		ALWAYS,
		EQUAL,
		LESS,
		LEQUAL,
		GEQUAL,
		GREATER,
		NEVER,
		NOTEQUAL,
	};
};

//================================================================//
// MOAIGfxType
//================================================================//
namespace MOAIGfxType {
	enum Type {
		BYTE,
		DOUBLE,
		FLOAT,
		INT,
		SHORT,
		UNSIGNED_BYTE,
		UNSIGNED_INT,
		UNSIGNED_SHORT,
	};
};

//================================================================//
// MOAITextureFilter
//================================================================//
namespace MOAITextureFilter {
	enum Type {
		LINEAR,
		LINEAR_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST,
		NEAREST,
		NEAREST_MIPMAP_LINEAR,
		NEAREST_MIPMAP_NEAREST,
		UNKNOWN,
	};
};

//================================================================//
// MOAITextureWrap
//================================================================//
namespace MOAITextureWrap {
	enum Type {
		CLAMP,
		REPEAT,
		UNKNOWN,
	};
};

//================================================================//
// MOAITopology
//================================================================//
namespace MOAITopology {
	enum Type {
		LINE_LIST,
		LINE_LOOP,
		LINE_STRIP,
		POINT_LIST,
		TRIANGLE_FAN,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		UNKNOWN,
	};
};

#endif
