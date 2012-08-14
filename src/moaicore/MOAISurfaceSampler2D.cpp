#include "pch.h"
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// MOAISurfaceBuffer2D
//================================================================//

//----------------------------------------------------------------//
const MOAISurface2D& MOAISurfaceBuffer2D::GetSurface ( u32 id ) const {

	assert ( id < this->mTop );
	return this->mSurfaces [ id ];
}

//----------------------------------------------------------------//
MOAISurfaceBuffer2D::MOAISurfaceBuffer2D () :
	mTop ( 0 ) {
}

//----------------------------------------------------------------//
MOAISurfaceBuffer2D::~MOAISurfaceBuffer2D () {
}

//================================================================//
// MOAISurfaceSampler2D
//================================================================//

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::Init ( MOAISurfaceBuffer2D& buffer, const USRect& sampleRect, const USAffine3D& worldToSampler ) {

	this->mBuffer = &buffer;
	this->mSampleRect = sampleRect;
	
	this->mSourceProp = 0;
	
	this->mWorldToSampler = worldToSampler;
	this->mTransform.Ident ();
}

//----------------------------------------------------------------//
MOAISurfaceSampler2D::MOAISurfaceSampler2D () {
}

//----------------------------------------------------------------//
MOAISurfaceSampler2D::~MOAISurfaceSampler2D	 () {
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::PushSurface ( const MOAISurfaceEdge2D& surface ) {

	if ( this->mBuffer->mTop < MOAISurfaceBuffer2D::MAX_SURFACES ) {
		
		USVec2D v0 = surface.mV0;
		USVec2D v1 = surface.mV1;
		
		this->mTransform.Transform ( v0 );
		this->mTransform.Transform ( v1 );
		
		USRect& sampleRect = this->mSampleRect;

		if (( v0.mX <= sampleRect.mXMin ) && ( v1.mX <= sampleRect.mXMin )) return;
		if (( v0.mX >= sampleRect.mXMax ) && ( v1.mX >= sampleRect.mXMax )) return;
		if (( v0.mY <= sampleRect.mYMin ) && ( v1.mY <= sampleRect.mYMin )) return;
		if (( v0.mY >= sampleRect.mYMax ) && ( v1.mY >= sampleRect.mYMax )) return;
		
		MOAISurface2D& surface = this->mBuffer->mSurfaces [ this->mBuffer->mTop++ ];
		surface.Init ( v0, v1 );
	}
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::SetSourceProp ( MOAIProp* prop ) {

	this->mSourceProp = prop;
	this->mTransform = this->mWorldToSampler;

	if ( prop ) {
		USAffine3D propToWorld;
		propToWorld.Init ( prop->GetLocalToWorldMtx ());
		this->mTransform.Prepend ( propToWorld );
	}
}
