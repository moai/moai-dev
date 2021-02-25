// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLVIEWPROJ_H
#define	ZLVIEWPROJ_H

#include <zl-core/ZLAffine3D.h>
#include <zl-core/ZLVec3D.h>

class ZLCamera;
class ZLViewport;

//================================================================//
// ZLViewProj
//================================================================//
// TODO: doxygen
namespace ZLViewProj {
	
	// TODO: this namespace is temporary. won't be needed after merge with 2.0.
	
	//----------------------------------------------------------------//
	
	// won't need these after the merge with 2.0 as matrix classes have static const idents.
	ZLMatrix4x4			GetProjectionMtx		( const ZLViewport* viewport, const ZLCamera* camera );
	ZLMatrix4x4			GetViewMtx				( const ZLCamera* camera );
	ZLMatrix4x4			GetViewMtx				( const ZLCamera* camera, const ZLVec3D& parallax );
	
	// these will probably go back to the layer... and have counterparts in the matrix cache
	ZLMatrix4x4			GetWndToWorldMtx		( const ZLViewport* viewport, const ZLCamera* camera, const ZLAffine3D& localToWorldMtx, const ZLVec3D& parallax = ZLVec3D::AXIS );
	ZLMatrix4x4			GetWorldToWndMtx		( const ZLViewport* viewport, const ZLCamera* camera, const ZLAffine3D& localToWorldMtx, const ZLVec3D& parallax = ZLVec3D::AXIS );
};

#endif
