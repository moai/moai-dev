// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMTXCACHE_H
#define	MOAIGFXMTXCACHE_H

//================================================================//
// MOAIGfxMtxCache
//================================================================//
class MOAIGfxMtxCache {
public:

	enum {
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
		WORLD_VIEW_PROJ_MTX,
		TOTAL_MATRICES,
	};

	enum {
		INVERSE_PROJ_MTX_FLAG				= 0x0001,
		INVERSE_UV_MTX_FLAG					= 0x0002,
		INVERSE_VIEW_MTX_FLAG				= 0x0004,
		INVERSE_VIEW_PROJ_MTX_FLAG			= 0x0008,
		INVERSE_WORLD_MTX_FLAG				= 0x0010,
		INVERSE_WORLD_VIEW_MTX_FLAG			= 0x0020,
		INVERSE_WORLD_VIEW_PROJ_MTX_FLAG	= 0x0040,
		VIEW_PROJ_MTX_FLAG					= 0x0080,
		VIEW_VOLUME_FLAG					= 0x0100,
		WORLD_VIEW_MTX_FLAG					= 0x0200,
		WORLD_VIEW_PROJ_MTX_FLAG			= 0x0400,
	};

	enum {
		INVERSE_PROJ_MTX_MASK				= INVERSE_PROJ_MTX_FLAG,
		INVERSE_UV_MTX_MASK					= INVERSE_UV_MTX_FLAG,
		INVERSE_VIEW_MTX_MASK				= INVERSE_VIEW_MTX_FLAG,
		INVERSE_VIEW_PROJ_MTX_MASK			= INVERSE_VIEW_PROJ_MTX_FLAG | INVERSE_PROJ_MTX_FLAG | INVERSE_VIEW_MTX_FLAG,
		INVERSE_WORLD_MTX_MASK				= INVERSE_WORLD_MTX_FLAG,
		INVERSE_WORLD_VIEW_MTX_MASK			= INVERSE_WORLD_VIEW_MTX_FLAG | INVERSE_VIEW_MTX_FLAG,
		INVERSE_WORLD_VIEW_PROJ_MTX_MASK	= INVERSE_WORLD_VIEW_PROJ_MTX_FLAG | INVERSE_PROJ_MTX_FLAG | INVERSE_VIEW_MTX_FLAG,
		VIEW_PROJ_MTX_MASK					= VIEW_PROJ_MTX_FLAG,
		VIEW_VOLUME_MASK					= VIEW_VOLUME_FLAG | INVERSE_PROJ_MTX_FLAG | INVERSE_VIEW_MTX_FLAG,
		WORLD_VIEW_MTX_MASK					= WORLD_VIEW_MTX_FLAG,
		WORLD_VIEW_PROJ_MTX_MASK			= WORLD_VIEW_PROJ_MTX_FLAG,
	};

	enum {
		PROJ_MTX_DIRTY_MASK					= INVERSE_PROJ_MTX_FLAG | INVERSE_VIEW_PROJ_MTX_FLAG | INVERSE_WORLD_VIEW_PROJ_MTX_FLAG | VIEW_PROJ_MTX_FLAG | WORLD_VIEW_PROJ_MTX_FLAG | VIEW_VOLUME_MASK,
		UV_MTX_DIRTY_MASK					= INVERSE_UV_MTX_FLAG,
		VIEW_MTX_DIRTY_MASK					= INVERSE_VIEW_MTX_FLAG | INVERSE_VIEW_PROJ_MTX_FLAG | INVERSE_WORLD_VIEW_MTX_FLAG | INVERSE_WORLD_VIEW_PROJ_MTX_FLAG | VIEW_PROJ_MTX_FLAG | WORLD_VIEW_MTX_FLAG | WORLD_VIEW_PROJ_MTX_FLAG | VIEW_VOLUME_MASK,
		WORLD_MTX_DIRTY_MASK				= INVERSE_WORLD_MTX_FLAG | INVERSE_WORLD_VIEW_MTX_FLAG | INVERSE_WORLD_VIEW_PROJ_MTX_FLAG | WORLD_VIEW_MTX_FLAG | WORLD_VIEW_PROJ_MTX_FLAG,
	};

protected:
	
	ZLMatrix4x4				mVertexTransforms [ TOTAL_MATRICES ]; // composition of VIEW and PROJ matrices via CPU
	u32						mDirtyFlags;
	
	ZLFrustum				mViewVolume;
	
public:

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
	
							MOAIGfxMtxCache				();
							~MOAIGfxMtxCache			();

	void					SetMtx						( u32 id );
	void					SetMtx						( u32 id, const ZLAffine3D& transform );
	void					SetMtx						( u32 transformID, const ZLMatrix4x4& mtx );
	
	//void					UpdateViewVolume			();
};

#endif
