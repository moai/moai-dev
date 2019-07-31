// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_CPUCACHE_H
#define	MOAIGFXMGR_CPUCACHE_H

#include <moai-gfx/MOAIGfxMgrComponents.h>
#include <moai-gfx/MOAIGfxStateCPUCacheFrame.h>

class MOAIShaderUniformSchema;
class ZLCamera;
class ZLViewport;

//================================================================//
// MOAIGfxMgr_CPUCache
//================================================================//
class MOAIGfxMgr_CPUCache :
	public virtual MOAIGfxMgrComponents,
 	public MOAIGfxStateConstsCPU {
protected:
	
	MOAIGfxStateCPUCacheFrame	mStateFrameCPU;
	
	//----------------------------------------------------------------//
	const ZLMatrix4x4&		GetPrimaryMtx				( u32 mtxID, u64 mtxFlag );
	void					RestoreCPUState				( const MOAIGfxStateCPUCacheFrame& frame );
	void					SetDirtyFlags				( u64 dirtyFlags );
	void					StoreCPUState				( MOAIGfxStateCPUCacheFrame& frame ) const;
	void					UpdateFinalColor			();
	
public:

	GET ( const ZLColorVec&, AmbientColor, this->mStateFrameCPU.mAmbientColor )
	GET ( const ZLColorVec&, PenColor, this->mStateFrameCPU.mPenColor )
	GET ( const ZLColorVec&, FinalColor, this->mStateFrameCPU.mFinalColor )
	GET ( u32, FinalColor32, this->mStateFrameCPU.mFinalColor32 )
	
	GET_SET ( u32, ClearFlags, this->mStateFrameCPU.mClearFlags )
	GET_SET ( ZLColorVec&, ClearColor, this->mStateFrameCPU.mClearColor )
	GET_SET ( double, ClearDepth, this->mStateFrameCPU.mClearDepth )

	//----------------------------------------------------------------//
	const ZLMatrix4x4&		GetMtx						( u32 mtxID );
	void					GetVertexMtxMode			( u32& input, u32& output );
	const ZLMatrix4x4&		GetVertexTransform			( u32 id );
	const ZLFrustum&		GetViewVolume				();
	
	bool					IsInputMtx					( u32 mtxID );
	
							MOAIGfxMgr_CPUCache			();
	virtual					~MOAIGfxMgr_CPUCache		();

	void					SetAmbientColor				( u32 color );
	void					SetAmbientColor				( const ZLColorVec& colorVec );
	void					SetAmbientColor				( float r, float g, float b, float a );

	void					SetMtx						( u32 mtxID );
	void					SetMtx						( u32 mtxID, const ZLAffine3D& transform );
	void					SetMtx						( u32 mtxID, const ZLMatrix4x4& mtx );
	
	void					SetPenColor					( u32 color );
	void					SetPenColor					( const ZLColorVec& colorVec );
	void					SetPenColor					( float r, float g, float b, float a );
	
	void					SetViewProj					( ZLViewport* viewport, ZLCamera* camera, ZLCamera* debug = 0, const ZLVec3D& parallax = ZLVec3D::AXIS );
	
	//----------------------------------------------------------------//
	static inline u64 GetAttrFlagForID ( u32 globalID ) {

		return globalID < TOTAL_GLOBALS ? ( u64 )1 << globalID : 0;
	}
};

#endif
