// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSHADERGLOBALSCACHE_H
#define	MOAIGFXSHADERGLOBALSCACHE_H

#include <moai-sim/MOAIGfxStateCache.h>

class MOAIShaderUniformBuffer;

#define ID_TO_FLAG(id) (( u64 )1 << id )

#define PRIMARY_TO_FULL_MASK(mask) (( mask ) | (( mask ) << MATRIX_SET_SIZE ) | (( mask ) << ( MATRIX_SET_SIZE * 2 )) | (( mask ) << ( MATRIX_SET_SIZE * 3 )))

//================================================================//
// MOAIGfxGlobalsCache
//================================================================//
class MOAIGfxGlobalsCache :
	public MOAIGfxStateCache {
public:

	// MUST NOT EXCEED 64 GLOBALS FOR NOW
	static const u32 MAX_GLOBALS = 64; // used to trip an assert

	enum {
		PRIMARY_MATRICES			= 0,
		INVERSE_MATRICES			= 1,
		NORMAL_MATRICES				= 2,
		NORMAL_INVERSE_MATRICES		= 3,
	};

	// VERY IMPORTANT: the following four sets of enums must align with each other. each set
	// represents a different derivation of the same matrix.

	// primary matrices
	enum {
		
		// settable matrices
		CLIP_TO_WINDOW_MTX,
		MODEL_TO_UV_MTX,
		MODEL_TO_WORLD_MTX,
		VIEW_TO_CLIP_MTX,
		WORLD_TO_DISPLAY_MTX,
		WORLD_TO_VIEW_MTX,
		
		// derived matrices
		CLIP_TO_DISPLAY_MTX,
		MODEL_TO_CLIP_MTX,
		MODEL_TO_DISPLAY_MTX,
		MODEL_TO_VIEW_MTX,
		VIEW_TO_DISPLAY_MTX,
		WORLD_TO_CLIP_MTX,
		
		MATRIX_SET_SIZE,
	};
	
	// inverse matrices
	enum {
		WINDOW_TO_CLIP_MTX = INVERSE_MATRICES * MATRIX_SET_SIZE,
		UV_TO_MODEL_MTX,
		WORLD_TO_MODEL_MTX,
		CLIP_TO_VIEW_MTX,
		DISPLAY_TO_WORLD_MTX,
		VIEW_TO_WORLD_MTX,
		DISPLAY_TO_CLIP_MTX,
		CLIP_TO_MODEL_MTX,
		DISPLAY_TO_MODEL_MTX,
		VIEW_TO_MODEL_MTX,
		DISPLAY_TO_VIEW_MTX,
		CLIP_TO_WORLD_MTX,
	};
	
	// normal matrices
	enum {
		NORMAL_CLIP_TO_WINDOW_MTX = NORMAL_MATRICES * MATRIX_SET_SIZE ,
		NORMAL_MODEL_TO_UV_MTX,
		NORMAL_MODEL_TO_WORLD_MTX,
		NORMAL_VIEW_TO_CLIP_MTX,
		NORMAL_WORLD_TO_DISPLAY_MTX,
		NORMAL_WORLD_TO_VIEW_MTX,
		NORMAL_CLIP_TO_DISPLAY_MTX,
		NORMAL_MODEL_TO_CLIP_MTX,
		NORMAL_MODEL_TO_DISPLAY_MTX,
		NORMAL_MODEL_TO_VIEW_MTX,
		NORMAL_VIEW_TO_DISPLAY_MTX,
		NORMAL_WORLD_TO_CLIP_MTX,
	};
	
	// normal inverse matrices
	enum {
		NORMAL_WINDOW_TO_CLIP_MTX = NORMAL_INVERSE_MATRICES * MATRIX_SET_SIZE,
		NORMAL_UV_TO_MODEL_MTX,
		NORMAL_WORLD_TO_MODEL_MTX,
		NORMAL_CLIP_TO_VIEW_MTX,
		NORMAL_DISPLAY_TO_WORLD_MTX,
		NORMAL_VIEW_TO_WORLD_MTX,
		NORMAL_DISPLAY_TO_CLIP_MTX,
		NORMAL_CLIP_TO_MODEL_MTX,
		NORMAL_DISPLAY_TO_MODEL_MTX,
		NORMAL_VIEW_TO_MODEL_MTX,
		NORMAL_DISPLAY_TO_VIEW_MTX,
		NORMAL_CLIP_TO_WORLD_MTX,
	};
	
	static const u32 TOTAL_MATRICES = MATRIX_SET_SIZE * 4;
	
	//static const u64 PRIMARY_MATRIX_MASK = ( u64 )( MATRIX_SET_SIZE - 1 );
	
	enum {
		
		PEN_COLOR			= MATRIX_SET_SIZE * 4,
		VIEW_VOLUME,
		
		// NOTE: we don't need a dirty mask for the view dimensions; the state cache will flush if the view rect changes
		VIEW_HALF_HEIGHT,
		VIEW_HALF_WIDTH,
		VIEW_HEIGHT,
		VIEW_WIDTH,
		
		TOTAL_GLOBALS,
	};

	// masks for all the non-settable (derived) matrices
	
	static const u64 CLIP_TO_DISPLAY_MTX_MASK				= ID_TO_FLAG ( CLIP_TO_DISPLAY_MTX );
	static const u64 CLIP_TO_WINDOW_MTX_MASK				= ID_TO_FLAG ( CLIP_TO_WINDOW_MTX );
	static const u64 MODEL_TO_CLIP_MTX_MASK					= ID_TO_FLAG ( MODEL_TO_CLIP_MTX );
	static const u64 MODEL_TO_DISPLAY_MTX_MASK				= ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX );
	static const u64 MODEL_TO_UV_MTX_MASK					= ID_TO_FLAG ( MODEL_TO_UV_MTX );
	static const u64 MODEL_TO_VIEW_MTX_MASK					= ID_TO_FLAG ( MODEL_TO_VIEW_MTX );
	static const u64 PEN_COLOR_MASK							= ID_TO_FLAG ( PEN_COLOR );
	static const u64 VIEW_TO_DISPLAY_MTX_MASK				= ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX );
	static const u64 VIEW_VOLUME_MASK						= ID_TO_FLAG ( VIEW_VOLUME );
	static const u64 WORLD_TO_CLIP_MTX_MASK					= ID_TO_FLAG ( WORLD_TO_CLIP_MTX );
	static const u64 WORLD_TO_DISPLAY_MTX_MASK				= ID_TO_FLAG ( WORLD_TO_DISPLAY_MTX );
		
	// for each settable matrix, here are the masks they will dirty
		
	static const u64 CLIP_TO_WINDOW_MTX_DIRTY_MASK			= PRIMARY_TO_FULL_MASK ( ID_TO_FLAG ( CLIP_TO_WINDOW_MTX ));
		
	static const u64 MODEL_TO_UV_MTX_DIRTY_MASK				= PRIMARY_TO_FULL_MASK ( ID_TO_FLAG ( MODEL_TO_UV_MTX ));
		
	static const u64 MODEL_TO_WORLD_MTX_DIRTY_MASK			= PRIMARY_TO_FULL_MASK (
																  ID_TO_FLAG ( MODEL_TO_CLIP_MTX )
																| ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( MODEL_TO_VIEW_MTX )
																| ID_TO_FLAG ( MODEL_TO_WORLD_MTX )
															);
	
	static const u64 VIEW_TO_CLIP_MTX_DIRTY_MASK			= PRIMARY_TO_FULL_MASK (
																  ID_TO_FLAG ( CLIP_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( MODEL_TO_CLIP_MTX )
																| ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( VIEW_TO_CLIP_MTX )
																| ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( VIEW_VOLUME )
																| ID_TO_FLAG ( WORLD_TO_CLIP_MTX )
															);
	
	static const u64 WORLD_TO_DISPLAY_MTX_DIRTY_MASK		= PRIMARY_TO_FULL_MASK (
																  ID_TO_FLAG ( CLIP_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX )
															);
	
	static const u64 WORLD_TO_VIEW_MTX_DIRTY_MASK			= PRIMARY_TO_FULL_MASK (
																  ID_TO_FLAG ( CLIP_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( MODEL_TO_CLIP_MTX )
																| ID_TO_FLAG ( MODEL_TO_VIEW_MTX )
																| ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( VIEW_VOLUME )
																| ID_TO_FLAG ( WORLD_TO_CLIP_MTX )
																| ID_TO_FLAG ( WORLD_TO_VIEW_MTX )
															);

	// user to clear dirty flags for the base attributes (only used to trigger shader updates)
		
	static const u64 BASE_ATTRS_MASK						=	  ID_TO_FLAG ( MODEL_TO_UV_MTX )
																| ID_TO_FLAG ( MODEL_TO_WORLD_MTX )
																| ID_TO_FLAG ( CLIP_TO_WINDOW_MTX )
																| ID_TO_FLAG ( PEN_COLOR )
																| ID_TO_FLAG ( VIEW_TO_CLIP_MTX )
																| ID_TO_FLAG ( WORLD_TO_DISPLAY_MTX )
																| ID_TO_FLAG ( WORLD_TO_VIEW_MTX );

protected:

	u64						mDirtyFlags;
	u64						mShaderFlags;
	
	ZLMatrix4x4				mMatrices [ TOTAL_MATRICES ];
	
	ZLFrustum				mViewVolume;
	
	ZLColorVec				mAmbientColor;
	ZLColorVec				mPenColor;
	ZLColorVec				mFinalColor;
	u32						mFinalColor32;
	
	u32						mClearFlags;
	ZLColorVec				mClearColor;
	double					mClearDepth;
		
	//----------------------------------------------------------------//
	const ZLMatrix4x4&		GetPrimaryMtx				( u32 mtxID, u64 mtxFlag );
	void					SetDirtyFlags				( u64 dirtyFlags );
	void					UpdateFinalColor			();
	
public:

	GET ( const ZLColorVec&, AmbientColor, mAmbientColor )
	GET ( const ZLColorVec&, PenColor, mPenColor )
	GET ( const ZLColorVec&, FinalColor, mFinalColor )
	GET ( u32, FinalColor32, mFinalColor32 )

	GET_SET ( u64, ShaderFlags, mShaderFlags )
	
	GET_SET ( u32, ClearFlags, mClearFlags )
	GET_SET ( ZLColorVec&, ClearColor, mClearColor )
	GET_SET ( double, ClearDepth, mClearDepth )

	//----------------------------------------------------------------//
	
	ZLMatrix4x4				GetNormToWndMtx				();
	ZLMatrix4x4				GetNormToWndMtx				( const ZLRect& wndRect );
	
	const ZLMatrix4x4&		GetMtx						( u32 mtxID );
	
	void					GetVertexMtxMode			( u32& input, u32& output );
	const ZLMatrix4x4&		GetVertexTransform			( u32 id );
	
	ZLMatrix4x4				GetWorldToWndMtx			();
	ZLMatrix4x4				GetWorldToWndMtx			( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWndToNormMtx				();
	ZLMatrix4x4				GetWndToNormMtx				( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWndToWorldMtx			();
	ZLMatrix4x4				GetWndToWorldMtx			( const ZLRect& wndRect );
	
	const ZLFrustum&		GetViewVolume				();
	
	bool					IsInputMtx					( u32 mtxID );
	
							MOAIGfxGlobalsCache			();
							~MOAIGfxGlobalsCache		();

	void					SetAmbientColor				( u32 color );
	void					SetAmbientColor				( const ZLColorVec& colorVec );
	void					SetAmbientColor				( float r, float g, float b, float a );

	void					SetMtx						( u32 mtxID );
	void					SetMtx						( u32 mtxID, const ZLAffine3D& transform );
	void					SetMtx						( u32 mtxID, const ZLMatrix4x4& mtx );
	
	void					SetPenColor					( u32 color );
	void					SetPenColor					( const ZLColorVec& colorVec );
	void					SetPenColor					( float r, float g, float b, float a );
	
	void					SetViewProj					( MOAIViewport* viewport, MOAICamera* camera, MOAICamera* debug = 0, const ZLVec3D& parallax = ZLVec3D::AXIS );
	
	//----------------------------------------------------------------//
	static inline u64 GetAttrFlagForID ( u32 globalID ) {

		return globalID < TOTAL_GLOBALS ? ( u64 )1 << globalID : 0;
	}
};

#endif
