// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILayoutFrame.h>
#include <moaicore/MOAIGfxProp2D.h>
#include <moaicore/MOAILayer2D.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>() setShader ( self, shader )</tt>\n
\n
	Sets the shader to use for this primitive.
	@param self (in)
	@param shader (in)
*/
int MOAIGfxProp2D::_setShader ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxProp2D, "UU" )

	if ( self->mShader ) {
		self->ClearDependency ( *self->mShader );
	}

	self->mShader = state.GetLuaData < MOAIShader >( 2 );
	self->SetDependency ( *self->mShader );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setUVTransform ( self, transform )</tt>\n
\n
	Sets a transform to be applied to the sprite source's UV coordinated
	prior to rendering.
	@param self (in)
	@param transform (in)
*/
int MOAIGfxProp2D::_setUVTransform ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxProp2D, "UU" )
	
	MOAITransform2D* transform = state.GetLuaData < MOAITransform2D >( 2 );
	if ( !transform ) return 0;

	self->mUVTransform = transform;

	return 0;
}

//================================================================//
// MOAIGfxProp2D
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxProp2D::BindDeck () {

	if ( this->mDeck ) {
		return this->mDeck->Bind ();
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxProp2D::Draw () {

	if ( !this->BindDeck ()) return;

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	if ( this->mUVTransform ) {
		USAffine2D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		drawbuffer.SetUVTransform ( uvMtx );
	}

	this->LoadShader ();
	
	if ( this->mGrid ) {
	
		USTileCoord c0;
		USTileCoord c1;
		
		this->GetBoundsInView ( c0, c1 );
		this->mDeck->Draw ( *this, *this->mGrid, c0, c1 );
	}
	else {
		this->mDeck->Draw ( *this, this->mIndex );
	}
	
	MOAILayoutFrame* parentFrame = USCast < MOAILayoutFrame >( this->mParent );
	if ( parentFrame ) {
		drawbuffer.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxProp2D::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	if ( this->mDeck ) {
		if ( debugLines.Bind ( MOAIDebugLines::SPRITE_MODEL_BOUNDS )) {
			
			debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
			debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
			
			USRect bounds = this->mDeck->GetBounds ( this->mIndex );
			debugLines.DrawRect ( bounds );
			
			if ( this->mGrid ) {
		
				debugLines.SetPenColor ( 0x40ffffff );
				debugLines.SetPenWidth ( 2 );
		
				USTileCoord c0;
				USTileCoord c1;
				
				this->GetBoundsInView ( c0, c1 );
				this->mDeck->DrawDebug ( *this, *this->mGrid, c0, c1 );
			}
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
void MOAIGfxProp2D::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {

	if ( !this->mDeck ) return;
		
	sampler.SetSourcePrim ( this );
	
	if ( this->mGrid ) {
		
		USRect localRect = sampler.GetLocalRect ();
		
		USTileCoord c0;
		USTileCoord c1;
		
		this->GetBoundsInRect ( localRect, c0, c1 );
		this->mDeck->GatherSurfaces ( *this->mGrid, c0, c1, sampler );
	}
	else {
		this->mDeck->GatherSurfaces ( this->mIndex, sampler );
	}
}

//----------------------------------------------------------------//
u32 MOAIGfxProp2D::GetLocalFrame ( USRect& frame ) {
	
	if ( this->mGrid ) {
	
		frame = this->mGrid->GetBounds ();
		return this->mRepeat ? FRAME_GLOBAL : FRAME_OK;
	}
	else if ( this->mDeck ) {
	
		frame = this->mDeck->GetBounds ( this->mIndex );
		return FRAME_OK;
	}
	
	return FRAME_EMPTY;
}

//----------------------------------------------------------------//
void MOAIGfxProp2D::LoadShader () {

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	if ( this->mShader ) {
		this->mShader->Bind ();
	}
	else {
		drawbuffer.SetPenColor ( 1.0f, 1.0f, 1.0f, 1.0f );
		drawbuffer.SetBlendMode ( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	}
	
	MOAILayoutFrame* parent = USCast < MOAILayoutFrame >( this->mParent );
	if ( parent ) {
		USRect scissorRect = parent->GetScissorRect ();			
		drawbuffer.SetScissorRect ( scissorRect );
	}
	else {
		drawbuffer.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
MOAIGfxProp2D::MOAIGfxProp2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp2D )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxProp2D::~MOAIGfxProp2D () {
}

//----------------------------------------------------------------//
void MOAIGfxProp2D::OnDepNodeUpdate () {

	this->MOAIProp2D::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
void MOAIGfxProp2D::RegisterLuaClass ( USLuaState& state ) {

	MOAIProp2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxProp2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIProp2D::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "setShader",			_setShader },
		{ "setUVTransform",		_setUVTransform },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIGfxProp2D::ToString () {

	STLString repr( MOAITransform2D::ToString () );

	PRETTY_PRINT ( repr, mDeck )

	return repr;
}
