// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIVRMGR_H
#define MOAIVRMGR_H

#if MOAI_WITH_LIBGVR

#include <vr/gvr/capi/include/gvr.h>

//================================================================//
// MOAIVrMgr
//================================================================//
class MOAIVrMgr :
	public ZLContextClass < MOAIVrMgr > {
private:

	gvr_context*	mGVRContext;
	ZLMatrix4x4		mHeadTransform;

public:

	GET ( const ZLMatrix4x4&, HeadTransform, mHeadTransform );

	//----------------------------------------------------------------//
	static ZLMatrix4x4		GvrMatrixToZLMatrix		( const gvr_mat4f& gvrMtx );
							MOAIVrMgr				();
							~MOAIVrMgr				();
	void					Update					();
};

#endif
#endif
