// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAILayoutFrame.h>
#include <moaicore/MOAISprite2D.h>
#include <moaicore/MOAIGfxLayer2D.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//================================================================//
// MOAISprite2D
//================================================================//

//----------------------------------------------------------------//
void MOAISprite2D::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch( attrID ) {
		case ATTR_CONTENT_ADDR:
			this->mIndex = attrOp.Op ( this->mIndex );
			break;
		default:
			MOAITransform2D::ApplyAttrOp ( attrID, attrOp );
	}
}

//----------------------------------------------------------------//
u32 MOAISprite2D::CountAttributes () {

	return MOAISprite2D::TOTAL_ATTR;
}

//----------------------------------------------------------------//
void MOAISprite2D::Draw () {

	if ( !this->BindGfxSource ()) return;

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	if ( this->mUVTransform ) {
		USAffine2D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		drawbuffer.SetUVTransform ( uvMtx );
	}

	if ( this->mDeck ) {
		this->LoadShader ();
		this->mDeck->Draw ( *this, this->mIndex );
	}
	
	MOAILayoutFrame* parentFrame = USCast < MOAILayoutFrame >( this->mParent );
	if ( parentFrame ) {
		drawbuffer.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
void MOAISprite2D::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	if ( this->mDeck ) {
		if ( debugLines.Bind ( MOAIDebugLines::SPRITE_MODEL_BOUNDS )) {
			
			debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
			debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
			
			USRect bounds = this->mDeck->GetBounds ( this->mIndex );
			debugLines.DrawRect ( bounds );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::SPRITE_WORLD_BOUNDS )) {
		debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
		debugLines.DrawRect ( this->GetWorldBounds ());
	}
	
	debugLines.SetPenColor ( 0x40ffffff );
	debugLines.SetPenWidth ( 2 );
	
	if ( this->mDeck ) {
		this->mDeck->DrawDebug ( *this, this->mIndex );
	}
}

//----------------------------------------------------------------//
void MOAISprite2D::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {

	if ( !this->mDeck ) return;
		
	sampler.SetSourcePrim ( this );
	
	this->mDeck->GatherSurfaces ( this->mIndex, sampler );
}

//----------------------------------------------------------------//
u32 MOAISprite2D::GetLocalFrame ( USRect& frame ) {
	
	if ( this->mDeck ) {
		frame = this->mDeck->GetBounds ( this->mIndex );
		return FRAME_OK;
	}
	return FRAME_EMPTY;
}

//----------------------------------------------------------------//
bool MOAISprite2D::Inside ( USVec2D vec ) {

	if ( !this->mDeck ) return false;
	
	const USAffine2D& worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );
	
	return this->mDeck->Contains ( this->mIndex, vec );
}

//----------------------------------------------------------------//
bool MOAISprite2D::Inside ( USVec2D vec, MOAIGfxLayer2D& scene ) {
	
	if ( !this->mDeck ) return false;
	
	// here we're getting the location of the rectangle in world space
	USAffine2D modelToWorld = this->GetLocalToWorldMtx ();
	USVec2D loc ( 0.0f, 0.0f );
	modelToWorld.Transform ( loc );
	
	USAffine2D worldToWnd;
	scene.GetWorldToWndMtx ( worldToWnd );

	worldToWnd.Transform ( vec );
	worldToWnd.Transform ( loc );
	
	vec.Sub ( loc );

	return this->mDeck->Contains ( this->mIndex, vec );
}

//----------------------------------------------------------------//
MOAISprite2D::MOAISprite2D () :
	mIndex ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxProp2D )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISprite2D::~MOAISprite2D () {
}

//----------------------------------------------------------------//
void MOAISprite2D::OnDepNodeUpdate () {

	this->MOAIProp2D::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
void MOAISprite2D::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIProp2D::RegisterLuaClass ( state );
	MOAIGfxProp2D::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_CONTENT_ADDR", ( u32 )ATTR_CONTENT_ADDR );
}

//----------------------------------------------------------------//
void MOAISprite2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIProp2D::RegisterLuaFuncs ( state );
	MOAIGfxProp2D::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "getContentAddr",			_getContentAddr },
		{ "inside",					_inside },
		{ "setContentAddr",			_setContentAddr },
		{ "setUVTransform",			_setUVTransform },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAISprite2D::ToString () {

	STLString repr( MOAITransform2D::ToString () );

	PRETTY_PRINT ( repr, mUVTransform )

	if ( mDeck ) {

		PRETTY_PRINT ( repr, mDeck )
		PRETTY_PRINT ( repr, mIndex )
	}

	return repr;
}

