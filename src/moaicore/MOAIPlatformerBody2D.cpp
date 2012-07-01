// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionResultMgr.h>
#include <moaicore/MOAIPlatformerBody2D.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPlatformerBody2D::_setMove ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPlatformerBody2D, "U" );
	
	self->mMove.mX = state.GetValue < float >( 2, self->mMove.mX );
	self->mMove.mY = state.GetValue < float >( 3, self->mMove.mY );
	
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIPlatformerBody2D
//================================================================//

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, this->GetLocalToWorldMtx ());
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	
	gfxDevice.SetPenColor ( 0xffffffff );
	gfxDevice.SetPenWidth ( 1.0f );
	draw.DrawEllipseOutline ( 0, 0, this->mHRad, this->mVRad, 16 );
	
	USBox bounds = this->GetBounds ();
	
	MOAIPartition* partition = this->GetPartition ();
	MOAIPartitionResultBuffer& props = MOAIPartitionResultMgr::Get ().GetBuffer ();
	u32 totalResults = partition->GatherProps ( props, 0, bounds, 0 );
	
	if ( totalResults ) {
		
		MOAISurfaceBuffer2D buffer;
		MOAISurfaceSampler2D sampler;
		
		USAffine2D worldToSampler;
		worldToSampler.Init ( this->GetWorldToLocalMtx ());
		
		USRect sampleRect;
		
		sampleRect.mXMin = -this->mHRad;
		sampleRect.mYMin = -( this->mVRad + this->mSkirt );
		sampleRect.mXMax = this->mHRad;
		sampleRect.mYMax = this->mVRad + this->mHat;
		
		sampler.Init ( buffer, sampleRect, worldToSampler );
		
		for ( u32 i = 0; i < totalResults; ++i ) {
			MOAIPartitionResult* result = props.GetResultUnsafe ( i );
			MOAIProp* prop = result->mProp;
			prop->SampleSurfaces ( sampler );
		}
		
		u32 top = buffer.GetTop ();
		if ( top ) {
		
			gfxDevice.SetPenColor ( 0xff0000ff );
			gfxDevice.SetPenWidth ( 2.0f );
		
			for ( u32 i = 0; i < top; ++i ) {
				const MOAISurface2D& surface = buffer.GetSurface ( i );
				draw.DrawLine ( surface.mV0, surface.mV1 );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::GatherSurfacesForMove ( MOAISurfaceSampler2D& sampler, USVec2D& move ) {
	UNUSED ( sampler );
	UNUSED ( move );

	//u32 results = 0;
	//
	//USPartition < MOAIPrim >* partition = this->GetPartition ();
	//if ( !partition ) return;
	//
	//USRect sweptRect;
	//this->GetSweptRect ( move, sweptRect );
	//sweptRect.Inflate ( this->mHRad * 0.5f ); // TODO
	//
	//results = partition->GatherPrims ( sweptRect, 0, MOAIContentLibrary2D::CAN_GATHER_SURFACES );
	//
	//if ( results ) {
	//	
	//	USMatrix2D worldToSampleMtx;
	//	this->GetWorldMtxInv ( worldToSampleMtx );
	//	
	//	sampler.Init ( worldToSampleMtx, sweptRect );
	//	
	//	for ( u32 i = 0; i < results; ++i ) {
	//		MOAIPrim* prim = partition->GetResult ( i );
	//		prim->GatherSurfaces ( sampler );
	//	}
	//}
}

//----------------------------------------------------------------//
//u32 MOAIPlatformerBody2D::GetLocalFrame ( USRect& frame ) {
//	
//	frame.mXMin = -this->mHRad;
//	frame.mYMin = -this->mVRad;
//	frame.mXMax = this->mHRad;
//	frame.mYMax = this->mVRad;
//	
//	return FRAME_OK;
//}

//----------------------------------------------------------------//
u32 MOAIPlatformerBody2D::GetPropBounds ( USBox& bounds ) {

	USRect rect;

	rect.mXMin = -this->mHRad;
	rect.mYMin = -( this->mVRad + this->mSkirt );
	rect.mXMax = this->mHRad;
	rect.mYMax = this->mVRad + this->mHat;
	
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );
	return BOUNDS_OK;
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::GetRect ( USRect& rect ) {
	
	rect.mXMin = this->mLoc.mX - this->mHRad;
	rect.mYMin = this->mLoc.mY - ( this->mVRad + this->mSkirt );
	rect.mXMax = this->mLoc.mX + this->mHRad;
	rect.mYMax = this->mLoc.mY + ( this->mVRad + this->mHat );
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::GetSweptRect ( USVec2D& move, USRect& rect ) {

	USRect bounds;
	this->GetRect ( bounds );

	rect = bounds;
	bounds.Offset ( move.mX, move.mY );
	rect.Grow ( bounds );
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::GetWorldMtx ( USAffine2D& transform ) {
	
	transform.Scale (
		this->mHRad,
		this->mVRad
	);
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::GetWorldMtxInv ( USAffine2D& transform ) {
	
	transform.Scale (
		1.0f / this->mHRad,
		1.0f / this->mVRad
	);
}

//----------------------------------------------------------------//
MOAIPlatformerBody2D::MOAIPlatformerBody2D () :
	mMove ( 0.0f, 0.0f ),
	mHRad ( 32.0f ),
	mVRad ( 32.0f ),
	mSkirt ( 0.0f ),
	mHat ( 0.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
	RTTI_END
	
	//this->SetQueryMask ( MOAIContentLibrary2D::CAN_DRAW_DEBUG );
	
	this->SetAngles ( 60.5f, 120.0f );
}

//----------------------------------------------------------------//
MOAIPlatformerBody2D::~MOAIPlatformerBody2D () {
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::Move () {
	
	//MOAIMarioFsm2D state;

	//// Gather all the surfaces we think we might collide with
	//this->GatherSurfacesForMove ( state, this->mMove );

	//USMatrix2D worldMtx;
	//this->GetWorldMtx ( worldMtx );
	//state.DrawSurfaces ( worldMtx );

	//state.Init ( 8, this->mFloorCos, this->mCeilCos );
	//
	//state.SetSkirt ( this->mSkirt / this->mVRad );
	//state.SetHat ( this->mHat / this->mVRad );

	//USVec2D loc = this->mLoc;
	//USVec2D move = this->mMove;
	//
	//this->TransformToLocal ( loc );
	//this->TransformToLocal ( move );
	//
	//loc = state.Move ( loc, move );

	//this->TransformToWorld ( loc );

	//this->mLoc = loc;
	//
	//this->mMove.Init ( 0.0f, 0.0f );
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::OnDepNodeUpdate () {
	
	this->mLoc.mX += this->mMove.mX;
	this->mLoc.mY += this->mMove.mY;
	
	this->mMove.Init ( 0.0f, 0.0f );
	
	//this->Move ();
	MOAIProp::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setMove",			_setMove },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::SetAngles ( float floorAngle, float ceilAngle ) {

	this->mFloorCos = Cos ( floorAngle * ( float )D2R );
	this->mCeilCos = Cos ( ceilAngle * ( float )D2R );
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::SetMove ( float x, float y ) {

	this->mMove.Init ( x, y );
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::TransformToLocal ( USVec2D& vec ) {

	vec.Scale ( 1.0f / this->mHRad, 1.0f / this->mVRad );
}

//----------------------------------------------------------------//
void MOAIPlatformerBody2D::TransformToWorld ( USVec2D& vec ) {

	vec.Scale ( this->mHRad, this->mVRad );
}

