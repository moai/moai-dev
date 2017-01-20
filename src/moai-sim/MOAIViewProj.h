// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVIEWPROJ_H
#define	MOAIVIEWPROJ_H

class MOAICamera;
class MOAIViewport;

//================================================================//
// MOAIViewProj
//================================================================//
// TODO: doxygen
namespace MOAIViewProj {
	
	// TODO: this namespace is temporary. won't be needed after merge with 2.0.
	
	//----------------------------------------------------------------//
	
	// won't need these after the merge with 2.0 as matrix classes have static const idents.
	ZLMatrix4x4			GetDebugMtx				( const MOAIViewport* viewport, const MOAICamera* camera );
	ZLMatrix4x4			GetProjectionMtx		( const MOAIViewport* viewport, const MOAICamera* camera );
	ZLMatrix4x4			GetViewMtx				( const MOAICamera* camera );
	ZLMatrix4x4			GetViewMtx				( const MOAICamera* camera, const ZLVec3D& parallax );
	
	// these will probably go back to the layer... and have counterparts in the matrix cache
	ZLMatrix4x4			GetWndToWorldMtx		( const MOAIViewport* viewport, const MOAICamera* camera, const ZLAffine3D& localToWorldMtx, const ZLVec3D& parallax = ZLVec3D::AXIS );
	ZLMatrix4x4			GetWorldToWndMtx		( const MOAIViewport* viewport, const MOAICamera* camera, const ZLAffine3D& localToWorldMtx, const ZLVec3D& parallax = ZLVec3D::AXIS );
};

#endif
