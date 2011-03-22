// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIFrame.h>
#include <moaicore/MOAIGfxPrim2D.h>
#include <moaicore/MOAILayer2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>setGfxSource ( self, lib )</tt>\n
	\n
	Associates a library with the prim.
	@param self (in)
	@param lib (in) Library to be used.
*/
int MOAIGfxPrim2D::_setGfxSource ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxPrim2D, "UU" )

	self->mGfxSource = state.GetLuaData < MOAIContentLibrary2D >( 2 );

	if ( self->mGfxSource ) {
		self->SetQueryMask ( self->mGfxSource->GetContentMask ());
	}
	else {
		self->SetQueryMask ( 0 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setShader ( self, shader )</tt>\n
\n
	Sets the shader to use for this primitive.
	@param self (in)
	@param shader (in)
*/
int MOAIGfxPrim2D::_setShader ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxPrim2D, "UU" )

	self->mShader = state.GetLuaData < MOAIShader >( 2 );
	self->SetAttrLink ( MOAIGfxPrim2D::ATTR_SHADER, self->mShader, NULL_ATTR );
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIGfxPrim2D
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxPrim2D::BindGfxSource () {

	if ( this->mGfxSource ) {
		return this->mGfxSource->Bind ();
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxPrim2D::Draw () {
}

//----------------------------------------------------------------//
void MOAIGfxPrim2D::LoadShader () {

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	if ( this->mShader ) {
		this->mShader->Bind ();
	}
	else {
		drawbuffer.SetPenColor ( 1.0f, 1.0f, 1.0f, 1.0f );
		drawbuffer.SetBlendMode ( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	}
	
	MOAIFrame* parent = USCast < MOAIFrame >( this->mParent );
	if ( parent ) {
		USRect scissorRect = parent->GetScissorRect ();			
		drawbuffer.SetScissorRect ( scissorRect );
	}
	else {
		drawbuffer.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
MOAIGfxPrim2D::MOAIGfxPrim2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPrim )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxPrim2D::~MOAIGfxPrim2D () {
}

//----------------------------------------------------------------//
void MOAIGfxPrim2D::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGfxPrim2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	LuaReg regTable [] = {
		{ "setGfxSource",	_setGfxSource },
		{ "setShader",		_setShader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIGfxPrim2D::ToString () {

	STLString repr( MOAITransform2D::ToString () );

	PRETTY_PRINT ( repr, mGfxSource )

	return repr;
}
