// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGCOMMAND_H
#define	MOAIDRAWINGCOMMAND_H

#include <moai-gfx/MOAIGfxConsts.h>

class MOAIAbstractDrawingAPICallback;
class MOAIGfxMgr;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIDrawingCmdEnum
//================================================================//
struct MOAIDrawingCmdEnum {
	enum _ {
		NONE,
		
		CLEAR_SURFACE,
		
		DRAW_AXIS_2D,
		DRAW_LINE,
		DRAW_POINT,
		DRAW_TRIANGLE,
		
		POP_GFX_STATE,
		PUSH_GFX_STATE,
		
		SET_BLEND_MODE,
		SET_CULL_FUNC,
		SET_DEPTH_FUNC,
		SET_DEPTH_MASK,
		SET_FRAME_BUFFER,
		SET_INDEX_BUFFER,
		SET_MATRIX,
		SET_PEN_COLOR,
		SET_PEN_WIDTH,
		SET_SCISSOR_RECT,
		SET_SHADER,
		SET_TEXTURE,
		SET_UNIFORM,
		SET_VERTEX_ARRAY,
		SET_VERTEX_BUFFER,
		SET_VERTEX_FORMAT,
		SET_VIEW_RECT,
		
		// keep these last
		CALL, // keep this first of call commands
		CALL_FROM_SHADER,
	};
};

//================================================================//
// MOAIDrawingParam
//================================================================//
namespace MOAIDrawingParam {

	//----------------------------------------------------------------//
	struct DrawAxis2D {
		ZLVec2D 			mV0;
		ZLVec2D 			mD;
	};
	
	//----------------------------------------------------------------//
	struct DrawAxisGrid2D {
		ZLVec2D 			mV0;
		ZLVec2D 			mD;
		float				mSize;
	};

	//----------------------------------------------------------------//
	struct DrawLine {
		ZLVec3D 			mV0;
		ZLVec3D 			mV1;
	};

	//----------------------------------------------------------------//
	struct DrawTriangle {
		ZLVec3D 			mV0;
		ZLVec3D 			mV1;
		ZLVec3D 			mV2;
	};

	//----------------------------------------------------------------//
	struct SetMatrix {
		u32 				mMatrixID;
		ZLMatrix4x4 		mMatrix;
	};

	//----------------------------------------------------------------//
	struct SetPenColor {
		u32 				mColor;
	};

	//----------------------------------------------------------------//
	struct SetPenWidth {
		float 				mWidth;
	};
	
	//----------------------------------------------------------------//
	struct SetTexture {
		MOAITexture* 		mTexture;
		ZLIndex 			mTextureUnit;
	};
	
	//----------------------------------------------------------------//
	struct SetUniform {
		ZLIndex				mPipelineGlobalID;
		ZLIndex				mTargetUniformID;
		ZLIndex				mTargetUniformIndex;
	};
};

//================================================================//
// MOAIDrawingCommand
//================================================================//
struct MOAIDrawingCommand {

	MOAIDrawingCmdEnum::_ 		mType;
	ZLSize 						mParamSize;

	//----------------------------------------------------------------//
	static void		Execute					( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam );
	static void 	ExecuteDrawAxis2D 		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawAxis2D& param );
	static void 	ExecuteDrawAxisGrid2D	( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawAxisGrid2D& param );
	static void 	ExecuteDrawLine 		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawLine& param );
	static void 	ExecuteDrawPoint 		( MOAIGfxMgr& gfxMgr, const ZLVec3D& param );
	static void 	ExecuteDrawTriangle		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawTriangle& param );
	static void		ExecuteSetUniform		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::SetUniform& param );
};

#endif
