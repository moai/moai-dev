// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPI_H
#define	MOAIDRAWINGAPI_H

#include <moai-gfx/MOAIGfxConsts.h>

class MOAIAbstractChildTransform;
class MOAIAbstractGfxScriptCallback;
class MOAICamera;
class MOAIGfxMgr;
class MOAIShader;
class MOAITexture;
class MOAIViewport;

//================================================================//
// MOAIDrawingAPIEnum
//================================================================//
struct MOAIDrawingAPIEnum {

	typedef int _; // is this too gross?

	enum {
	
		NONE						= ( _ )0,
		
		BIND_VECTOR_PRESETS,
		CLEAR_SURFACE,
		
		DRAW_ANIM_CURVE,
		DRAW_AXIS_2D,
		DRAW_LINE,
		DRAW_POINT,
		DRAW_RAY,
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
		SET_MATRIX_FROM_TRANSFORM,
		SET_PEN_COLOR,
		SET_PEN_WIDTH,
		SET_SCISSOR_RECT,
		SET_SHADER,
		SET_TEXTURE,
		SET_VERTEX_ARRAY,
		SET_VERTEX_BUFFER,
		SET_VERTEX_FORMAT,
		SET_VIEW_PROJ,
		SET_VIEW_RECT,
		SET_VIEW_RECT_FROM_VIEWPORT,
		
		TOTAL_BASE_COMMANDS,
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
	struct DrawAnimCurve {
		MOAIAnimCurve*		mAnimCurve;
		u32					mResolution;
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
	struct DrawRay {
		ZLVec2D 			mLoc;
		ZLVec2D 			mVec;
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
	struct SetMatrixFromTransform {
		u32 							mMatrixID;
		MOAIAbstractChildTransform*		mTransform;
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
	struct SetViewProj {
		MOAICamera*			mCamera;
		MOAIViewport*		mViewport;
	};
};

//================================================================//
// MOAIDrawingAPI
//================================================================//
struct MOAIDrawingAPI {

	MOAIDrawingAPIEnum::_ 		mType;
	ZLSize 						mParamSize;

	//----------------------------------------------------------------//
	static void		Execute					( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam );
	static void		ExecuteDrawAnimCurve	( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawAnimCurve& param );
	static void 	ExecuteDrawAxis2D 		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawAxis2D& param );
	static void 	ExecuteDrawAxisGrid2D	( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawAxisGrid2D& param );
	static void 	ExecuteDrawLine 		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawLine& param );
	static void 	ExecuteDrawPoint 		( MOAIGfxMgr& gfxMgr, const ZLVec3D& param );
	static void 	ExecuteDrawRay 			( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawRay& param );
	static void 	ExecuteDrawTriangle		( MOAIGfxMgr& gfxMgr, const MOAIDrawingParam::DrawTriangle& param );
};

#endif
