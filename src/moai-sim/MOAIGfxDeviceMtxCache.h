// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICEMTXCACHE_H
#define	MOAIGFXDEVICEMTXCACHE_H

#include <moai-sim/MOAIGfxDeviceStateCache.h>

//================================================================//
// MOAIGfxDeviceMtxCache
//================================================================//
class MOAIGfxDeviceMtxCache :
	public MOAIGfxDeviceStateCache {
public:
	
	enum {
		VTX_STAGE_MODEL,
		VTX_STAGE_WORLD,
		VTX_STAGE_VIEW,
		VTX_STAGE_PROJ,
	};
	
	enum {
		UV_STAGE_MODEL,
		UV_STAGE_TEXTURE,
	};
	
	enum {
		VTX_WORLD_TRANSFORM,
		VTX_VIEW_TRANSFORM,
		VTX_PROJ_TRANSFORM,
		TOTAL_VTX_TRANSFORMS,
	};

protected:
	
	u32						mVertexMtxInput;
	u32						mVertexMtxOutput;
	ZLMatrix4x4				mVertexTransforms [ TOTAL_VTX_TRANSFORMS ];

	bool					mCpuVertexTransform;
	ZLMatrix4x4				mCpuVertexTransformMtx; // composition of matrices to be applied via CPU
	bool					mCpuVertexTransformCache [ TOTAL_VTX_TRANSFORMS ];
	ZLMatrix4x4				mCpuVertexTransformCacheMtx [ TOTAL_VTX_TRANSFORMS ]; // composition of VIEW and PROJ matrices via CPU

	u32						mUVMtxInput;
	u32						mUVMtxOutput;
	ZLMatrix4x4				mUVTransform;

	bool					mCpuUVTransform;

	//----------------------------------------------------------------//
	void					UpdateCpuVertexMtx			();
	void					UpdateUVMtx					();
	
public:
	
	//----------------------------------------------------------------//
	ZLMatrix4x4				GetNormToWndMtx				() const;
	ZLMatrix4x4				GetNormToWndMtx				( const ZLRect& wndRect ) const;
	
	void					GetUVMtxMode				( u32& input, u32& output ) const;
	const ZLMatrix4x4&		GetUVTransform				() const;
	
	void					GetVertexMtxMode			( u32& input, u32& output ) const;
	const ZLMatrix4x4&		GetVertexTransform			( u32 id ) const;
	
	ZLMatrix4x4				GetViewProjMtx				() const;
	
	ZLMatrix4x4				GetWorldToWndMtx			() const;
	ZLMatrix4x4				GetWorldToWndMtx			( const ZLRect& wndRect ) const;
	
	ZLMatrix4x4				GetWndToNormMtx				() const;
	ZLMatrix4x4				GetWndToNormMtx				( const ZLRect& wndRect ) const;
	
	ZLMatrix4x4				GetWndToWorldMtx			() const;
	ZLMatrix4x4				GetWndToWorldMtx			( const ZLRect& wndRect ) const;
	
							MOAIGfxDeviceMtxCache		();
							~MOAIGfxDeviceMtxCache		();

	void					SetUVMtxMode				( u32 input, u32 output );
	void					SetUVTransform				();
	void					SetUVTransform				( const ZLAffine3D& transform );
	void					SetUVTransform				( const ZLMatrix4x4& transform );
	
	void					SetVertexMtxMode			( u32 input, u32 output );
	void					SetVertexTransform			( u32 id );
	void					SetVertexTransform			( u32 id, const ZLAffine3D& transform );
	void					SetVertexTransform			( u32 id, const ZLMatrix4x4& transform );
	
	void					UpdateViewVolume			();
};

#endif
