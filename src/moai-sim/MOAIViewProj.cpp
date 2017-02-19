// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIViewport.h>
#include <moai-sim/MOAIViewProj.h>

//================================================================//
// MOAIViewProj
//================================================================//

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIViewProj::GetProjectionMtx ( const MOAIViewport* viewport, const MOAICamera* camera ) {

	return camera ? camera->GetProjMtx ( *viewport ) : ( viewport ? viewport->GetProjMtx () : ZLMatrix4x4::IDENT );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIViewProj::GetViewMtx ( const MOAICamera* camera ) {

	return camera ? camera->GetViewMtx () : ZLMatrix4x4::IDENT;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIViewProj::GetViewMtx ( const MOAICamera* camera, const ZLVec3D& parallax ) {

	if ( camera ) {
	
		ZLMatrix4x4 view ( camera->GetViewMtx ());
		view.m [ ZLMatrix4x4::C3_R0 ] *= parallax.mX;
		view.m [ ZLMatrix4x4::C3_R1 ] *= parallax.mY;
		view.m [ ZLMatrix4x4::C3_R2 ] *= parallax.mZ;
		return view;
	}
	return ZLMatrix4x4::IDENT;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIViewProj::GetWndToWorldMtx ( const MOAIViewport* viewport, const MOAICamera* camera, const ZLAffine3D& localToWorldMtx, const ZLVec3D& parallax ) {

	ZLMatrix4x4 wndToWorld = MOAIViewProj::GetWorldToWndMtx ( viewport, camera, localToWorldMtx, parallax );
	wndToWorld.Inverse ();
	return wndToWorld;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIViewProj::GetWorldToWndMtx ( const MOAIViewport* viewport, const MOAICamera* camera, const ZLAffine3D& localToWorldMtx, const ZLVec3D& parallax ) {
	
	ZLMatrix4x4 worldToWnd;
	
	if ( viewport ) {
		worldToWnd = MOAIViewProj::GetViewMtx ( camera, parallax );
		worldToWnd.Append ( MOAIViewProj::GetProjectionMtx ( viewport, camera ));
		worldToWnd.Append ( viewport->GetNormToWndMtx ());
	}
	else {
		worldToWnd.Ident ();
	}
	
	ZLMatrix4x4 mtx ( localToWorldMtx );
	worldToWnd.Append ( mtx );
	
	return worldToWnd;
}
