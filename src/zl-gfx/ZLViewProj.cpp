// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLCamera.h>
#include <zl-gfx/ZLViewport.h>
#include <zl-gfx/ZLViewProj.h>

//================================================================//
// ZLViewProj
//================================================================//

//----------------------------------------------------------------//
ZLMatrix4x4 ZLViewProj::GetProjectionMtx ( const ZLViewport* viewport, const ZLCamera* camera ) {

	return camera ? camera->GetProjMtx ( *viewport ) : ( viewport ? viewport->GetProjMtx () : ZLMatrix4x4::IDENT );
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLViewProj::GetViewMtx ( const ZLCamera* camera ) {

	return camera ? camera->GetViewMtx () : ZLMatrix4x4::IDENT;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLViewProj::GetViewMtx ( const ZLCamera* camera, const ZLVec3D& parallax ) {

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
ZLMatrix4x4 ZLViewProj::GetWndToWorldMtx ( const ZLViewport* viewport, const ZLCamera* camera, const ZLAffine3D& localToWorldMtx, const ZLVec3D& parallax ) {

	ZLMatrix4x4 wndToWorld = ZLViewProj::GetWorldToWndMtx ( viewport, camera, localToWorldMtx, parallax );
	wndToWorld.Inverse ();
	return wndToWorld;
}

//----------------------------------------------------------------//
ZLMatrix4x4 ZLViewProj::GetWorldToWndMtx ( const ZLViewport* viewport, const ZLCamera* camera, const ZLAffine3D& localToWorldMtx, const ZLVec3D& parallax ) {
	
	ZLMatrix4x4 worldToWnd;
	
	if ( viewport ) {
		worldToWnd = ZLViewProj::GetViewMtx ( camera, parallax );
		worldToWnd.Append ( ZLViewProj::GetProjectionMtx ( viewport, camera ));
		worldToWnd.Append ( viewport->GetNormToWndMtx ());
	}
	else {
		worldToWnd.Ident ();
	}
	
	ZLMatrix4x4 mtx ( localToWorldMtx );
	worldToWnd.Append ( mtx );
	
	return worldToWnd;
}
