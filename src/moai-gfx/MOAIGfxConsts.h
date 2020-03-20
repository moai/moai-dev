// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCONSTS_H
#define	MOAIGFXCONSTS_H

//================================================================//
// MOAIBlendFactorEnum
//================================================================//
struct MOAIBlendFactorEnum {
	enum _ {
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
// MOAIBlendFuncEnum
//================================================================//
struct MOAIBlendFuncEnum {
	enum _ {
		ADD,
		MAX,
		MIN,
		REVERSE_SUBTRACT,
		SUBTRACT,
		UNKNOWN,
	};
};

//================================================================//
// MOAIClearFlagsEnum
//================================================================//
struct MOAIClearFlagsEnum {
	enum {
		COLOR_BUFFER_BIT		= 0x01,
		DEPTH_BUFFER_BIT		= 0x02,
		STENCIL_BUFFER_BIT		= 0x04,
	};
};

//================================================================//
// MOAICullFuncEnum
//================================================================//
struct MOAICullFuncEnum {
	enum _ {
		NONE,
		ALL,
		BACK,
		FRONT,
		UNKNOWN,
	};
};

//================================================================//
// MOAIDepthFuncEnum
//================================================================//
struct MOAIDepthFuncEnum {
	enum _ {
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
// MOAIGfxTopologyEnum
//================================================================//
struct MOAIGfxTopologyEnum {
	enum _ {
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

//================================================================//
// MOAIGfxTypeEnum
//================================================================//
struct MOAIGfxTypeEnum {
	enum _ {
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
// MOAIRenderPassAttachmentEnum
//================================================================//
//struct MOAIRenderPassAttachmentEnum {
//	enum _ {
//		COLOR_ATTACHMENT,
//		DEPTH_ATTACHMENT,
//		STENCIL_ATTACHMENT,
//	};
//};

//================================================================//
// MOAITextureFilterEnum
//================================================================//
struct MOAITextureFilterEnum {
	enum _ {
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
// MOAITextureWrapEnum
//================================================================//
struct MOAITextureWrapEnum {
	enum _ {
		CLAMP,
		REPEAT,
		UNKNOWN,
	};
};

#endif
