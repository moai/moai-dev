#include "pch.h"
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// MOAISurfaceSampler2D
//================================================================//

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::AddSurfaceFromLocal ( ZLVec2D v0, ZLVec2D v1 ) {

	if ( this->mTop < kMaxSurfaces ) {
		
		ZLRect localRect = this->mLocalRect;
	
		if (( v0.mX <= localRect.mXMin ) && ( v1.mX <= localRect.mXMin )) return;
		if (( v0.mX >= localRect.mXMax ) && ( v1.mX >= localRect.mXMax )) return;
		if (( v0.mY <= localRect.mYMin ) && ( v1.mY <= localRect.mYMin )) return;
		if (( v0.mY >= localRect.mYMax ) && ( v1.mY >= localRect.mYMax )) return;
		
		this->mLocalToSampleMtx.Transform ( v0 );
		this->mLocalToSampleMtx.Transform ( v1 );
	
		MOAISurface2D& surface = this->mSurfaces [ this->mTop++ ];
		surface.Init ( v0, v1 );
		surface.mSourcePrim = this->mSourcePrim;
	}
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::AddSurfaceFromWorld ( ZLVec2D v0, ZLVec2D v1 ) {

	if ( this->mTop < kMaxSurfaces ) {
		
		ZLRect worldRect = this->mWorldRect;
	
		if (( v0.mX <= worldRect.mXMin ) && ( v1.mX <= worldRect.mXMin )) return;
		if (( v0.mX >= worldRect.mXMax ) && ( v1.mX >= worldRect.mXMax )) return;
		if (( v0.mY <= worldRect.mYMin ) && ( v1.mY <= worldRect.mYMin )) return;
		if (( v0.mY >= worldRect.mYMax ) && ( v1.mY >= worldRect.mYMax )) return;
		
		this->mWorldToSampleMtx.Transform ( v0 );
		this->mWorldToSampleMtx.Transform ( v1 );
		
		MOAISurface2D& surface = this->mSurfaces [ this->mTop++ ];
		surface.Init ( v0, v1 );
		surface.mSourcePrim = this->mSourcePrim;
	}
}


//----------------------------------------------------------------//
void MOAISurfaceSampler2D::Clear () {

	this->mTop = 0;
}

//----------------------------------------------------------------//
ZLRect MOAISurfaceSampler2D::GetLocalRect () {

	return this->mLocalRect;
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::Init ( const ZLAffine3D& worldToSampleMtx, const ZLRect& worldRect ) {

	this->Clear ();

	this->mWorldToSampleMtx = worldToSampleMtx;
	this->mWorldRect = worldRect;
}

//----------------------------------------------------------------//
MOAISurfaceSampler2D::MOAISurfaceSampler2D () :
	mSourcePrim ( 0 ),
	mTop ( 0 ) {
}

//----------------------------------------------------------------//
MOAISurfaceSampler2D::~MOAISurfaceSampler2D () {
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::SetObjectMtx () {

	this->mLocalToSampleMtx = this->mWorldToSampleMtx;
	this->mLocalRect = this->mWorldRect;
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::SetObjectMtx ( const ZLAffine3D& localToWorld, const ZLAffine3D& worldToLocal ) {

	this->mLocalToSampleMtx = localToWorld;
	this->mLocalToSampleMtx.Append ( this->mWorldToSampleMtx );
	
	this->mLocalRect = this->mWorldRect;
	worldToLocal.Transform ( this->mLocalRect );
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::SetSourcePrim ( MOAITransform* sourcePrim ) {

	if ( sourcePrim ) {
	
		const ZLAffine3D& localToWorldMtx = sourcePrim->GetLocalToWorldMtx ();
		const ZLAffine3D& worldToLocalMtx = sourcePrim->GetWorldToLocalMtx ();
		
		this->mSourcePrim = sourcePrim;
		this->SetObjectMtx ( localToWorldMtx, worldToLocalMtx );
		
	}
	else {
		this->mSourcePrim = 0;
		this->SetObjectMtx ();
	}
}
