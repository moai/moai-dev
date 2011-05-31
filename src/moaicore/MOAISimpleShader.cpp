// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISimpleShader.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/** @name	setBlendMode
	@text	Set the fixed function blend mode.

	@overload	Reset the blend mode to MOAISimpleShader.BLEND_NORMAL (equivalent to src = GL_ONE, dst = GL_ONE_MINUS_SRC_ALPHA)

		@in		MOAISimpleShader self
		@out	nil

	@overload	Set blend mode using one of the Moai presets.

		@in		MOAISimpleShader self
		@in		number mode					One of MOAISimpleShader.BLEND_NORMAL, MOAISimpleShader.BLEND_ADD, MOAISimpleShader.BLEND_MULTIPLY.
		@out	nil
	
	@overload	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAISimpleShader.
				See the OpenGL documentation for an explanation of blending constants.

		@in		MOAISimpleShader self
		@in		number srcFactor
		@in		number dstFactor
		@out	nil
*/
int MOAISimpleShader::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISimpleShader, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		if ( state.IsType ( 3, LUA_TNUMBER )) {
		
			u32 srcFactor = state.GetValue < u32 >( 2, 0 );
			u32 dstFactor = state.GetValue < u32 >( 3, 0 );
			self->mBlendMode.SetBlend ( srcFactor, dstFactor );
		}
		else {
			
			u32 blendMode = state.GetValue < u32 >( 2, USBlendMode::BLEND_NORMAL );
			self->mBlendMode.SetBlend ( blendMode );
		}
	}
	else {
		self->mBlendMode.SetBlend ( USBlendMode::BLEND_NORMAL );
	}
	
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setParent
	@text	Set the parent of this shader. Must be another MOAISimpleShader.
			This shader will inherit the color of its parent.

	@in		MOAISimpleShader self
	@in		MOAISimpleShader parent
	@out	nil
*/
int MOAISimpleShader::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISimpleShader, "U" )
	
	MOAISimpleShader* parent = state.GetLuaObject < MOAISimpleShader >( 2 );
	self->SetParent ( parent );
	
	return 0;
}

//================================================================//
// MOAISimpleShader
//================================================================//

//----------------------------------------------------------------//
bool MOAISimpleShader::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	return MOAIColor::ApplyAttrOp ( attrID, attrOp );
}

//----------------------------------------------------------------//
void MOAISimpleShader::Bind () {

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	drawbuffer.SetPenColor ( this->mColor );
	drawbuffer.SetBlendMode ( this->mBlendMode );
}

//----------------------------------------------------------------//
MOAISimpleShader::MOAISimpleShader () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIShader )
		RTTI_EXTEND ( MOAIColor )
	RTTI_END
	
	this->mColor = *this;
}

//----------------------------------------------------------------//
MOAISimpleShader::~MOAISimpleShader () {
}

//----------------------------------------------------------------//
void MOAISimpleShader::OnDepNodeUpdate () {

	MOAIShader::OnDepNodeUpdate ();
	MOAIColor::OnDepNodeUpdate ();
	
	this->mColor = *this;
	
	if ( this->mParent ) {
	
		USColorVec parentColor = *this->mParent;
		
		this->mColor.mR *= parentColor.mR;
		this->mColor.mG *= parentColor.mG;
		this->mColor.mB *= parentColor.mB;
		this->mColor.mA *= parentColor.mA;
	}
}

//----------------------------------------------------------------//
void MOAISimpleShader::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIShader::RegisterLuaClass ( state );
	MOAIColor::RegisterLuaClass ( state );
	
	state.SetField ( -1, "BLEND_ADD", ( u32 )USBlendMode::BLEND_ADD );
	state.SetField ( -1, "BLEND_MULTIPLY", ( u32 )USBlendMode::BLEND_MULTIPLY );
	state.SetField ( -1, "BLEND_NORMAL", ( u32 )USBlendMode::BLEND_NORMAL );
	
	state.SetField ( -1, "GL_ONE", ( u32 )GL_ONE );
	state.SetField ( -1, "GL_ZERO", ( u32 )GL_ZERO );
	state.SetField ( -1, "GL_DST_ALPHA", ( u32 )GL_DST_ALPHA );
	state.SetField ( -1, "GL_DST_COLOR", ( u32 )GL_DST_COLOR );
	state.SetField ( -1, "GL_SRC_COLOR", ( u32 )GL_SRC_COLOR );
	state.SetField ( -1, "GL_ONE_MINUS_DST_ALPHA", ( u32 )GL_ONE_MINUS_DST_ALPHA );
	state.SetField ( -1, "GL_ONE_MINUS_DST_COLOR", ( u32 )GL_ONE_MINUS_DST_COLOR );
	state.SetField ( -1, "GL_ONE_MINUS_SRC_ALPHA", ( u32 )GL_ONE_MINUS_SRC_ALPHA );
	state.SetField ( -1, "GL_ONE_MINUS_SRC_COLOR", ( u32 )GL_ONE_MINUS_SRC_COLOR );
	state.SetField ( -1, "GL_SRC_ALPHA", ( u32 )GL_SRC_ALPHA );
	state.SetField ( -1, "GL_SRC_ALPHA_SATURATE", ( u32 )GL_SRC_ALPHA_SATURATE );
}

//----------------------------------------------------------------//
void MOAISimpleShader::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIShader::RegisterLuaFuncs ( state );
	MOAIColor::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setBlendMode",	_setBlendMode },
		{ "setParent",		_setParent },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISimpleShader::SetParent ( MOAISimpleShader* parent ) {

	this->SetDependentMember < MOAISimpleShader >( this->mParent, parent );
}

//----------------------------------------------------------------//
STLString MOAISimpleShader::ToString () {

	STLString repr;

	return repr;
}
