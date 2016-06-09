// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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

	// GLOBAL IDs
	enum {
	
		// NOTE: GROUP ALL MATRICES TOGETHER
		INVERSE_PROJ_MTX,
		INVERSE_UV_MTX,
		INVERSE_VIEW_MTX,
		INVERSE_VIEW_PROJ_MTX,
		INVERSE_WORLD_MTX,
		INVERSE_WORLD_VIEW_MTX,
		INVERSE_WORLD_VIEW_PROJ_MTX,
		PROJ_MTX,
		UV_MTX,
		VIEW_MTX,
		VIEW_PROJ_MTX,
		WORLD_MTX,
		WORLD_VIEW_MTX,
		WORLD_VIEW_PROJ_MTX,	// NOTE: WHATEVER FOLLOWS THIS *MUST* BE ALIASED AS TOTAL_MATRICES (BELOW)
		
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
	
		INVERSE_PROJ_MTX_MASK				= ID_TO_FLAG ( INVERSE_PROJ_MTX ),
		INVERSE_UV_MTX_MASK					= ID_TO_FLAG ( INVERSE_UV_MTX ),
		INVERSE_VIEW_MTX_MASK				= ID_TO_FLAG ( INVERSE_VIEW_MTX ),
		INVERSE_VIEW_PROJ_MTX_MASK			= ID_TO_FLAG ( INVERSE_VIEW_PROJ_MTX ) | ID_TO_FLAG ( INVERSE_PROJ_MTX ) | ID_TO_FLAG ( INVERSE_VIEW_MTX ),
		INVERSE_WORLD_MTX_MASK				= ID_TO_FLAG ( INVERSE_WORLD_MTX ),
		INVERSE_WORLD_VIEW_MTX_MASK			= ID_TO_FLAG ( INVERSE_WORLD_VIEW_MTX ) | ID_TO_FLAG ( INVERSE_VIEW_MTX ),
		INVERSE_WORLD_VIEW_PROJ_MTX_MASK	= ID_TO_FLAG ( INVERSE_WORLD_VIEW_PROJ_MTX ) | ID_TO_FLAG ( INVERSE_PROJ_MTX ) | ID_TO_FLAG ( INVERSE_VIEW_MTX ),
		VIEW_PROJ_MTX_MASK					= ID_TO_FLAG ( VIEW_PROJ_MTX ),
		VIEW_VOLUME_MASK					= ID_TO_FLAG ( VIEW_VOLUME ) | ID_TO_FLAG ( INVERSE_PROJ_MTX ) | ID_TO_FLAG ( INVERSE_VIEW_MTX ),
		WORLD_VIEW_MTX_MASK					= ID_TO_FLAG ( WORLD_VIEW_MTX ),
		WORLD_VIEW_PROJ_MTX_MASK			= ID_TO_FLAG ( WORLD_VIEW_PROJ_MTX ),
	
		BASE_ATTRS_MASK						= ID_TO_FLAG ( PROJ_MTX )
											| ID_TO_FLAG ( UV_MTX )
											| ID_TO_FLAG ( VIEW_MTX )
											| ID_TO_FLAG ( WORLD_MTX )
											| ID_TO_FLAG ( PEN_COLOR ),
		
		PEN_COLOR_DIRTY_MASK				= ID_TO_FLAG ( PEN_COLOR ),
		
		PROJ_MTX_DIRTY_MASK					= ID_TO_FLAG ( PROJ_MTX )
											| ID_TO_FLAG ( INVERSE_PROJ_MTX )
											| ID_TO_FLAG ( INVERSE_VIEW_PROJ_MTX )
											| ID_TO_FLAG ( INVERSE_WORLD_VIEW_PROJ_MTX )
											| ID_TO_FLAG ( VIEW_PROJ_MTX )
											| ID_TO_FLAG ( WORLD_VIEW_PROJ_MTX )
											| ID_TO_FLAG ( VIEW_VOLUME ),
		
		UV_MTX_DIRTY_MASK					= ID_TO_FLAG ( UV_MTX )
											| ID_TO_FLAG ( INVERSE_UV_MTX ),
		
		VIEW_MTX_DIRTY_MASK					= ID_TO_FLAG ( VIEW_MTX )
											| ID_TO_FLAG ( INVERSE_VIEW_MTX )
											| ID_TO_FLAG ( INVERSE_VIEW_PROJ_MTX )
											| ID_TO_FLAG ( INVERSE_WORLD_VIEW_MTX )
											| ID_TO_FLAG ( INVERSE_WORLD_VIEW_PROJ_MTX )
											| ID_TO_FLAG ( VIEW_PROJ_MTX )
											| ID_TO_FLAG ( WORLD_VIEW_MTX )
											| ID_TO_FLAG ( WORLD_VIEW_PROJ_MTX )
											| ID_TO_FLAG ( VIEW_VOLUME ),
		
		WORLD_MTX_DIRTY_MASK				= ID_TO_FLAG ( WORLD_MTX )
											| ID_TO_FLAG ( INVERSE_WORLD_MTX )
											| ID_TO_FLAG ( INVERSE_WORLD_VIEW_MTX )
											| ID_TO_FLAG ( INVERSE_WORLD_VIEW_PROJ_MTX )
											| ID_TO_FLAG ( WORLD_VIEW_MTX )
											| ID_TO_FLAG ( WORLD_VIEW_PROJ_MTX ),
	};

protected:
	
	ZLMatrix4x4				mVertexTransforms [ TOTAL_MATRICES ]; // composition of VIEW and PROJ matrices via CPU
	u32						mDirtyFlags;
	u32						mShaderFlags;
	
	ZLFrustum				mViewVolume;
	
	ZLColorVec				mAmbientColor;
	ZLColorVec				mPenColor;
	ZLColorVec				mFinalColor;
	u32						mFinalColor32;
	
	u32						mBufferWidth;
	u32						mBufferHeight;
	
	//----------------------------------------------------------------//
	void					CheckGlobalsAndFlush		();
	void					UpdateFinalColor			();
	
public:

	GET ( const ZLColorVec&, AmbientColor, mAmbientColor )
	GET ( const ZLColorVec&, PenColor, mPenColor )
	GET ( const ZLColorVec&, FinalColor, mFinalColor )
	GET ( u32, FinalColor32, mFinalColor32 )

	GET_SET ( u32, ShaderFlags, mShaderFlags )

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
	
							MOAIGfxGlobalsCache			();
							~MOAIGfxGlobalsCache		();

	void					SetAmbientColor				( u32 color );
	void					SetAmbientColor				( const ZLColorVec& colorVec );
	void					SetAmbientColor				( float r, float g, float b, float a );

	void					SetMtx						( u32 transformID );
	void					SetMtx						( u32 transformID, const ZLAffine3D& transform );
	void					SetMtx						( u32 transformID, const ZLMatrix4x4& mtx );
	
	void					SetPenColor					( u32 color );
	void					SetPenColor					( const ZLColorVec& colorVec );
	void					SetPenColor					( float r, float g, float b, float a );
	
	//----------------------------------------------------------------//
	static inline u32 GetAttrFlagForID ( u32 globalID ) {

		return globalID < TOTAL_GLOBALS ? 1 << globalID : 0;
	}
};

#endif
