// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSHADERGLOBALSCACHE_H
#define	MOAIGFXSHADERGLOBALSCACHE_H

#include <moai-sim/MOAIGfxStateCache.h>

class MOAIShaderUniformBuffer;

#define ID_TO_FLAG(id) ( 1 << id )

//================================================================//
// MOAIGfxGlobalsCache
//================================================================//
class MOAIGfxGlobalsCache :
	public MOAIGfxStateCache {
public:

	// MUST NOT EXCEED 32 GLOBALS FOR NOW
	
	static const u32 MAX_GLOBALS = 32; // used to trip an assert

	// GLOBAL IDs
	enum {
	
		// NOTE: GROUP ALL MATRICES TOGETHER
		
		// settable matrices
		
		CLIP_TO_WINDOW_MTX,						// WINDOW_MTX
		MODEL_TO_UV_MTX,						// UV_MTX
		MODEL_TO_WORLD_MTX,						// WORLD_MTX
		VIEW_TO_CLIP_MTX,						// PROJ_MTX
		WORLD_TO_DEBUG_MTX,
		WORLD_TO_VIEW_MTX,						// VIEW_MTX
		
		// derived matrices
		
		CLIP_TO_DISPLAY_MTX,
		CLIP_TO_MODEL_MTX,						// INVERSE_WORLD_VIEW_PROJ_MTX
		CLIP_TO_VIEW_MTX,						// INVERSE_PROJ_MTX
		CLIP_TO_WORLD_MTX,						// INVERSE_VIEW_PROJ_MTX
		
		MODEL_TO_CLIP_MTX,						// WORLD_VIEW_PROJ_MTX
		MODEL_TO_DISPLAY_MTX,
		
		MODEL_TO_VIEW_MTX,						// WORLD_VIEW_MTX
		
		NORMALIZED_MODEL_TO_CLIP_MTX,			// WORLD_VIEW_PROJ_NORMAL_MTX
		NORMALIZED_MODEL_TO_VIEW_MTX,			// WORLD_VIEW_NORMAL_MTX
		NORMALIZED_MODEL_TO_WORLD_MTX,			// WORLD_NORMAL_MTX
		
		UV_TO_MODEL_MTX,						// INVERSE_UV_MTX
		
		VIEW_TO_DISPLAY_MTX,
		VIEW_TO_MODEL_MTX,						// INVERSE_WORLD_VIEW_MTX
		VIEW_TO_WORLD_MTX,						// INVERSE_VIEW_MTX
		
		WINDOW_TO_CLIP_MTX,						// INVERSE_WINDOW_MTX
		
		WORLD_TO_CLIP_MTX,						// VIEW_PROJ_MTX
		
		WORLD_TO_DISPLAY_MTX,
		WORLD_TO_MODEL_MTX,						// INVERSE_WORLD_MTX
		
		
		// NOTE: WHATEVER FOLLOWS THIS *MUST* BE ALIASED AS TOTAL_MATRICES (BELOW)
		
		PEN_COLOR,
		VIEW_VOLUME,
		
		VIEW_HALF_HEIGHT,
		VIEW_HALF_WIDTH,
		VIEW_HEIGHT,
		VIEW_WIDTH,
		
		TOTAL_GLOBALS,
	};
	
	static const u32 TOTAL_MATRICES = PEN_COLOR;

	// NOTE: we don't need a dirty mask for the view dimensions; the state cache will flush if the view rect changes

	enum {
	
		// masks for all the non-settable (derived) matrices
	
		CLIP_TO_DISPLAY_MTX_MASK				= ID_TO_FLAG ( CLIP_TO_DISPLAY_MTX ),
		CLIP_TO_MODEL_MTX_MASK					= ID_TO_FLAG ( CLIP_TO_MODEL_MTX ),
		CLIP_TO_VIEW_MTX_MASK					= ID_TO_FLAG ( CLIP_TO_VIEW_MTX ),
		CLIP_TO_WORLD_MTX_MASK					= ID_TO_FLAG ( CLIP_TO_WORLD_MTX ),
		MODEL_TO_CLIP_MTX_MASK					= ID_TO_FLAG ( MODEL_TO_CLIP_MTX ),
		MODEL_TO_DISPLAY_MTX_MASK				= ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX ),
		MODEL_TO_VIEW_MTX_MASK					= ID_TO_FLAG ( MODEL_TO_VIEW_MTX ),
		NORMALIZED_MODEL_TO_CLIP_MTX_MASK		= ID_TO_FLAG ( NORMALIZED_MODEL_TO_CLIP_MTX ),
		NORMALIZED_MODEL_TO_VIEW_MTX_MASK		= ID_TO_FLAG ( NORMALIZED_MODEL_TO_VIEW_MTX ),
		NORMALIZED_MODEL_TO_WORLD_MTX_MASK		= ID_TO_FLAG ( NORMALIZED_MODEL_TO_WORLD_MTX ),
		PEN_COLOR_DIRTY_MASK					= ID_TO_FLAG ( PEN_COLOR ),
		UV_TO_MODEL_MTX_MASK					= ID_TO_FLAG ( UV_TO_MODEL_MTX ),
		VIEW_TO_MODEL_MTX_MASK					= ID_TO_FLAG ( VIEW_TO_MODEL_MTX ),
		VIEW_TO_DISPLAY_MTX_MASK				= ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX ),
		VIEW_TO_WORLD_MTX_MASK					= ID_TO_FLAG ( VIEW_TO_WORLD_MTX ),
		VIEW_VOLUME_MASK						= ID_TO_FLAG ( VIEW_VOLUME ),
		WINDOW_TO_CLIP_MTX_MASK					= ID_TO_FLAG ( WINDOW_TO_CLIP_MTX ),
		WORLD_TO_CLIP_MTX_MASK					= ID_TO_FLAG ( WORLD_TO_CLIP_MTX ),
		WORLD_TO_DISPLAY_MTX_MASK				= ID_TO_FLAG ( WORLD_TO_DISPLAY_MTX ),
		WORLD_TO_MODEL_MTX_MASK					= ID_TO_FLAG ( WORLD_TO_MODEL_MTX ),
		
		// for each settable matrix, here are the masks they will dirty
		
		CLIP_TO_WINDOW_MTX_DIRTY_MASK			= ID_TO_FLAG ( CLIP_TO_WINDOW_MTX )
												| ID_TO_FLAG ( WINDOW_TO_CLIP_MTX ),
		
		MODEL_TO_UV_MTX_DIRTY_MASK				= ID_TO_FLAG ( MODEL_TO_UV_MTX )
												| ID_TO_FLAG ( UV_TO_MODEL_MTX ),
		
		MODEL_TO_WORLD_MTX_DIRTY_MASK			= ID_TO_FLAG ( CLIP_TO_MODEL_MTX )
												| ID_TO_FLAG ( MODEL_TO_CLIP_MTX )
												| ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX )
												| ID_TO_FLAG ( MODEL_TO_VIEW_MTX )
												| ID_TO_FLAG ( MODEL_TO_WORLD_MTX )
												| ID_TO_FLAG ( NORMALIZED_MODEL_TO_CLIP_MTX )
												| ID_TO_FLAG ( NORMALIZED_MODEL_TO_VIEW_MTX )
												| ID_TO_FLAG ( NORMALIZED_MODEL_TO_WORLD_MTX )
												| ID_TO_FLAG ( VIEW_TO_MODEL_MTX )
												| ID_TO_FLAG ( WORLD_TO_MODEL_MTX ),
		
		VIEW_TO_CLIP_MTX_DIRTY_MASK				= ID_TO_FLAG ( CLIP_TO_MODEL_MTX )
												| ID_TO_FLAG ( CLIP_TO_VIEW_MTX )
												| ID_TO_FLAG ( CLIP_TO_WORLD_MTX )
												| ID_TO_FLAG ( MODEL_TO_CLIP_MTX )
												| ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX )
												| ID_TO_FLAG ( NORMALIZED_MODEL_TO_CLIP_MTX )
												| ID_TO_FLAG ( VIEW_TO_CLIP_MTX )
												| ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX )
												| ID_TO_FLAG ( VIEW_VOLUME )
												| ID_TO_FLAG ( WORLD_TO_CLIP_MTX )
												| ID_TO_FLAG ( WORLD_TO_DISPLAY_MTX ),
		
		WORLD_TO_DEBUG_MTX_DIRTY_MASK			= ID_TO_FLAG ( CLIP_TO_DISPLAY_MTX )
												| ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX )
												| ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX )
												| ID_TO_FLAG ( WORLD_TO_DISPLAY_MTX ),
		
		WORLD_TO_VIEW_MTX_DIRTY_MASK			= ID_TO_FLAG ( CLIP_TO_MODEL_MTX )
												| ID_TO_FLAG ( CLIP_TO_WORLD_MTX )
												| ID_TO_FLAG ( MODEL_TO_CLIP_MTX )
												| ID_TO_FLAG ( MODEL_TO_VIEW_MTX )
												| ID_TO_FLAG ( NORMALIZED_MODEL_TO_CLIP_MTX )
												| ID_TO_FLAG ( NORMALIZED_MODEL_TO_VIEW_MTX )
												| ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX )
												| ID_TO_FLAG ( VIEW_TO_MODEL_MTX )
												| ID_TO_FLAG ( VIEW_TO_WORLD_MTX )
												| ID_TO_FLAG ( VIEW_VOLUME )
												| ID_TO_FLAG ( WORLD_TO_CLIP_MTX )
												| ID_TO_FLAG ( WORLD_TO_DISPLAY_MTX )
												| ID_TO_FLAG ( WORLD_TO_VIEW_MTX ),

		// user to clear dirty flags for the base attributes (only used to trigger shader updates)
		
		BASE_ATTRS_MASK							= ID_TO_FLAG ( MODEL_TO_UV_MTX )
												| ID_TO_FLAG ( MODEL_TO_WORLD_MTX )
												| ID_TO_FLAG ( CLIP_TO_WINDOW_MTX )
												| ID_TO_FLAG ( PEN_COLOR )
												| ID_TO_FLAG ( VIEW_TO_CLIP_MTX )
												| ID_TO_FLAG ( WORLD_TO_DEBUG_MTX )
												| ID_TO_FLAG ( WORLD_TO_VIEW_MTX ),
	};

protected:
	
	ZLMatrix4x4				mMatrices [ TOTAL_MATRICES ]; // composition of VIEW and PROJ matrices via CPU
	u32						mDirtyFlags;
	u32						mShaderFlags;
	
	ZLFrustum				mViewVolume;
	
	ZLColorVec				mAmbientColor;
	ZLColorVec				mPenColor;
	ZLColorVec				mFinalColor;
	u32						mFinalColor32;
	
	u32						mClearFlags;
	ZLColorVec				mClearColor;
	double					mClearDepth;
	
	bool					mUseDebugMtx;
	
	//----------------------------------------------------------------//
	void					SetDirtyFlags				( u32 dirtyFlags );
	void					UpdateFinalColor			();
	
public:

	GET ( const ZLColorVec&, AmbientColor, mAmbientColor )
	GET ( const ZLColorVec&, PenColor, mPenColor )
	GET ( const ZLColorVec&, FinalColor, mFinalColor )
	GET ( u32, FinalColor32, mFinalColor32 )

	GET_SET ( u32, ShaderFlags, mShaderFlags )
	
	GET_SET ( u32, ClearFlags, mClearFlags )
	GET_SET ( ZLColorVec&, ClearColor, mClearColor )
	GET_SET ( double, ClearDepth, mClearDepth )

	//----------------------------------------------------------------//
	
	ZLMatrix4x4				GetNormToWndMtx				();
	ZLMatrix4x4				GetNormToWndMtx				( const ZLRect& wndRect );
	
	const ZLMatrix4x4&		GetMtx						( u32 transformID );
	
	void					GetVertexMtxMode			( u32& input, u32& output );
	const ZLMatrix4x4&		GetVertexTransform			( u32 id );
	
	ZLMatrix4x4				GetWorldToWndMtx			();
	ZLMatrix4x4				GetWorldToWndMtx			( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWndToNormMtx				();
	ZLMatrix4x4				GetWndToNormMtx				( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWndToWorldMtx			();
	ZLMatrix4x4				GetWndToWorldMtx			( const ZLRect& wndRect );
	
	const ZLFrustum&		GetViewVolume				();
	
	bool					IsInputMtx					( u32 transformID );
	
							MOAIGfxGlobalsCache			();
							~MOAIGfxGlobalsCache		();

	void					SetAmbientColor				( u32 color );
	void					SetAmbientColor				( const ZLColorVec& colorVec );
	void					SetAmbientColor				( float r, float g, float b, float a );

	void					SetDebug					( MOAIViewport* viewport, MOAICamera* camera );

	void					SetMtx						( u32 transformID );
	void					SetMtx						( u32 transformID, const ZLAffine3D& transform );
	void					SetMtx						( u32 transformID, const ZLMatrix4x4& mtx );
	
	void					SetPenColor					( u32 color );
	void					SetPenColor					( const ZLColorVec& colorVec );
	void					SetPenColor					( float r, float g, float b, float a );
	
	void					SetViewProj					( MOAIViewport* viewport, MOAICamera* camera, const ZLVec3D& parallax = ZLVec3D::AXIS );
	
	//----------------------------------------------------------------//
	static inline u32 GetAttrFlagForID ( u32 globalID ) {

		return globalID < TOTAL_GLOBALS ? 1 << globalID : 0;
	}
};

#endif
