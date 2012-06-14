#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// MOAISurfaceSampler2D
//================================================================//

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::AddSurfaceFromLocal ( USVec2D v0, USVec2D v1 ) {

	if ( this->mTop < kMaxSurfaces ) {
		
		USRect localRect = this->mLocalRect;
	
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
void MOAISurfaceSampler2D::AddSurfaceFromWorld ( USVec2D v0, USVec2D v1 ) {

	if ( this->mTop < kMaxSurfaces ) {
		
		USRect worldRect = this->mWorldRect;
	
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
USRect MOAISurfaceSampler2D::GetLocalRect () {

	return this->mLocalRect;
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::Init ( const USAffine3D& worldToSampleMtx, const USRect& worldRect ) {

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
void MOAISurfaceSampler2D::SetObjectMtx ( const USAffine3D& localToWorld, const USAffine3D& worldToLocal ) {

	this->mLocalToSampleMtx = localToWorld;
	this->mLocalToSampleMtx.Append ( this->mWorldToSampleMtx );
	
	this->mLocalRect = this->mWorldRect;
	worldToLocal.Transform ( this->mLocalRect );
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::SetSourcePrim ( MOAITransform* sourcePrim ) {

	if ( sourcePrim ) {
	
		const USAffine3D& localToWorldMtx = sourcePrim->GetLocalToWorldMtx ();
		const USAffine3D& worldToLocalMtx = sourcePrim->GetWorldToLocalMtx ();
		
		this->mSourcePrim = sourcePrim;
		this->SetObjectMtx ( localToWorldMtx, worldToLocalMtx );
		
	}
	else {
		this->mSourcePrim = 0;
		this->SetObjectMtx ();
	}
}
