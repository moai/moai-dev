// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISimpleShader.h>
#include <moaicore/MOAIEaseDriver.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/** @name	setBlendMode
	@text	Set the fixed function blend mode.

	@in		MOAISimpleShader self
	@in		number mode				One of MOAISimpleShader.BLEND_NORMAL, MOAISimpleShader.BLEND_ADD, MOAISimpleShader.BLEND_MULTIPLY.
	@out	nil
*/
int MOAISimpleShader::_setBlendMode ( lua_State* L ) {
	LUA_SETUP ( MOAISimpleShader, "UN" )

	u32 blendMode = state.GetValue < u32 >( 2, USBlendMode::BLEND_NORMAL );
	self->mBlendMode.SetBlend ( blendMode );
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
	LUA_SETUP ( MOAISimpleShader, "U" )
	
	MOAISimpleShader* parent = state.GetLuaObject < MOAISimpleShader >( 2 );
	self->SetParent ( parent );
	
	return 0;
}

//================================================================//
// MOAISimpleShader
//================================================================//

//----------------------------------------------------------------//
void MOAISimpleShader::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	MOAIColor::ApplyAttrOp ( attrID, attrOp );
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
		RTTI_EXTEND ( MOAIColor )
	RTTI_END
	
	this->mColor = *this;
}

//----------------------------------------------------------------//
MOAISimpleShader::~MOAISimpleShader () {
}

//----------------------------------------------------------------//
void MOAISimpleShader::OnDepNodeUpdate () {

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
	
	MOAIColor::RegisterLuaClass ( state );
	
	state.SetField ( -1, "BLEND_NORMAL", ( u32 )USBlendMode::BLEND_NORMAL );
	state.SetField ( -1, "BLEND_ADD", ( u32 )USBlendMode::BLEND_ADD );
	state.SetField ( -1, "BLEND_MULTIPLY", ( u32 )USBlendMode::BLEND_MULTIPLY );
}

//----------------------------------------------------------------//
void MOAISimpleShader::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIColor::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "setBlendMode",	_setBlendMode },
		{ "setParent",		_setParent },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISimpleShader::SetParent ( MOAISimpleShader* parent ) {

	this->mParent = parent;
	this->SetAttrLink ( ATTR_PARENT, parent, NULL_ATTR );
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
STLString MOAISimpleShader::ToString () {

	STLString repr;

	return repr;
}
