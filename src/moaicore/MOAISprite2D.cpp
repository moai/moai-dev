// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIFrame.h>
#include <moaicore/MOAISprite2D.h>
#include <moaicore/MOAILayer2D.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( addr ) getContentAddr ( self )</tt>\n
\n
	Gets the current content address.
	@param self (in)
	@param addr (out)
*/
int MOAISprite2D::_getContentAddr ( lua_State* L ) {
	LUA_SETUP ( MOAISprite2D, "U" )

	lua_pushnumber ( state, self->mContentAddr );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>inside (mX, mY)</tt>\n
	\n
	Returns true if a point is inside of this sprite.
	@param mX The X coordinate of the point to be checked.
	@param mY The Y coordinate of the point to be checked.
	@return True if the point is inside, otherwise false.
*/
int	MOAISprite2D::_inside ( lua_State* L ) {
	LUA_SETUP ( MOAISprite2D, "UNN" )

	USVec2D vec;
	vec.mX	= state.GetValue < float >( 2, 0.0f );
	vec.mY	= state.GetValue < float >( 3, 0.0f );

	MOAILayer2D* scene = state.GetLuaData < MOAILayer2D >( 4 );
	
	if ( scene ) {
		bool result = self->Inside ( vec, *scene );
		lua_pushboolean ( state, result );
	}
	else {
		bool result = self->Inside ( vec );
		lua_pushboolean ( state, result );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>setContentAddr ( self, addr )</tt>\n
\n
	Sets the index for the asset to display in the content library.
	@param self (in)
	@param addr (in)
*/
int MOAISprite2D::_setContentAddr ( lua_State* L ) {
	LUA_SETUP ( MOAISprite2D, "UN" )

	self->mContentAddr = state.GetValue < u32 >( 2, 0 );
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
int MOAISprite2D::_setUVTransform ( lua_State* L ) {
	LUA_SETUP ( MOAISprite2D, "UU" )
	
	MOAITransform2D* transform = state.GetLuaData < MOAITransform2D >( 2 );
	if ( !transform ) return 0;

	self->mUVTransform = transform;

	return 0;
}

//================================================================//
// MOAISprite2D
//================================================================//

//----------------------------------------------------------------//
void MOAISprite2D::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch( attrID ) {
		case ATTR_CONTENT_ADDR:
			this->mContentAddr = attrOp.Op ( this->mContentAddr );
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

	if ( this->mGfxSource ) {
		this->LoadShader ();
		this->mGfxSource->Draw ( *this, this->mContentAddr );
	}
	
	MOAIFrame* parentFrame = USCast < MOAIFrame >( this->mParent );
	if ( parentFrame ) {
		drawbuffer.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
void MOAISprite2D::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	if ( this->mGfxSource ) {
		if ( debugLines.Bind ( MOAIDebugLines::SPRITE_MODEL_BOUNDS )) {
			
			debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
			debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
			
			USRect bounds = this->mGfxSource->GetBounds ( this->mContentAddr );
			debugLines.DrawRect ( bounds );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::SPRITE_WORLD_BOUNDS )) {
		debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
		debugLines.DrawRect ( this->GetWorldBounds ());
	}
	
	debugLines.SetPenColor ( 0x40ffffff );
	debugLines.SetPenWidth ( 2 );
	
	if ( this->mGfxSource ) {
		this->mGfxSource->DrawDebug ( *this, this->mContentAddr );
	}
}

//----------------------------------------------------------------//
void MOAISprite2D::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {

	if ( !this->mGfxSource ) return;
		
	sampler.SetSourcePrim ( this );
	
	this->mGfxSource->GatherSurfaces ( this->mContentAddr, sampler );
}

//----------------------------------------------------------------//
u32 MOAISprite2D::GetLocalFrame ( USRect& frame ) {
	
	if ( this->mGfxSource ) {
		frame = this->mGfxSource->GetBounds ( this->mContentAddr );
		return FRAME_OK;
	}
	return FRAME_EMPTY;
}

//----------------------------------------------------------------//
bool MOAISprite2D::Inside ( USVec2D vec ) {

	if ( !this->mGfxSource ) return false;
	
	const USAffine2D& worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );
	
	return this->mGfxSource->Contains ( this->mContentAddr, vec );
}

//----------------------------------------------------------------//
bool MOAISprite2D::Inside ( USVec2D vec, MOAILayer2D& scene ) {
	
	if ( !this->mGfxSource ) return false;
	
	// here we're getting the location of the rectangle in world space
	USAffine2D modelToWorld = this->GetLocalToWorldMtx ();
	USVec2D loc ( 0.0f, 0.0f );
	modelToWorld.Transform ( loc );
	
	USAffine2D worldToWnd;
	scene.GetWorldToWndMtx ( worldToWnd );

	worldToWnd.Transform ( vec );
	worldToWnd.Transform ( loc );
	
	vec.Sub ( loc );

	return this->mGfxSource->Contains ( this->mContentAddr, vec );
}

//----------------------------------------------------------------//
MOAISprite2D::MOAISprite2D () :
	mContentAddr ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxPrim2D )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISprite2D::~MOAISprite2D () {
}

//----------------------------------------------------------------//
void MOAISprite2D::OnDepNodeUpdate () {

	this->MOAIPrim::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
void MOAISprite2D::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaClass ( state );
	MOAIGfxPrim2D::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_CONTENT_ADDR", ( u32 )ATTR_CONTENT_ADDR );
}

//----------------------------------------------------------------//
void MOAISprite2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaFuncs ( state );
	MOAIGfxPrim2D::RegisterLuaFuncs ( state );
	
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

	if ( mGfxSource ) {

		PRETTY_PRINT ( repr, mGfxSource )
		PRETTY_PRINT ( repr, mContentAddr )
	}

	return repr;
}

