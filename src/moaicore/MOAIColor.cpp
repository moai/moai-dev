// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getColor
 @text	Returns the current color.
 
 @in		MOAIColor self
 @out	number r
 @out	number g
 @out	number b
 @out	number opacity
 */
int	MOAIColor::_getColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "U" )
	
	lua_pushnumber ( state, self->mR );
	lua_pushnumber ( state, self->mG );
	lua_pushnumber ( state, self->mB );
	lua_pushnumber ( state, self->mA );
	
	return 4;
}

//----------------------------------------------------------------//
/**	@name	getOpacity
 @text	Returns the current opacity.
 
 @in		MOAIColor self
 @out	number opacity
 */
int MOAIColor::_getOpacity( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "U" )
	
	lua_pushnumber ( state, self->mA );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setColor
	@text	Initialize the color.
	
	@in		MOAIColor self
	@in		number r	Default value is 0.
	@in		number g	Default value is 0.
	@in		number b	Default value is 0.
	@opt	number o	Default value is 1.
	@out	nil
*/
int MOAIColor::_setColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "UNNN" )

	float r = state.GetValue < float >( 2, 0.0f );
	float g = state.GetValue < float >( 3, 0.0f );
	float b = state.GetValue < float >( 4, 0.0f );
	float o = state.GetValue < float >( 5, 1.0f );
	
	self->Set ( r, g, b, o );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setOpacity
 @text	Set the opacity.
 
 @in		MOAIColor self
 @in		number o	Default value is 0.
 @out	nil
 */
int MOAIColor::_setOpacity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "UN" )
	
	float o = state.GetValue < float >( 2, 1.0f );
	
	self->Set (self->mR, self->mG, self->mB, o);
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAIColor self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIColor::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "U" )

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2, true );
	
	self->SetAttrLink ( PACK_ATTR ( MOAIColor, INHERIT_COLOR ), parent, PACK_ATTR ( MOAIColor, COLOR_TRAIT ));
	
	//MOAILog ( state, MOAILogMessages::MOAI_FunctionDeprecated_S, "setParent" );
	
	return 0;
}

//================================================================//
// MOAIColor
//================================================================//

//----------------------------------------------------------------//
bool MOAIColor::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIColorAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_R_COL:
			{
				this->mR = USFloat::Clamp ( attrOp.Apply ( this->mR, op, MOAIAttrOp::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			}
			case ATTR_G_COL:
			{
				this->mG = USFloat::Clamp ( attrOp.Apply ( this->mG, op, MOAIAttrOp::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			}
			case ATTR_B_COL:
			{
				this->mB = USFloat::Clamp ( attrOp.Apply ( this->mB, op, MOAIAttrOp::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			}
			case ATTR_OPACITY:
			{
				this->mA = USFloat::Clamp ( attrOp.Apply ( this->mA, op, MOAIAttrOp::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			}
			case COLOR_TRAIT:
				attrOp.ApplyNoAdd < USColorVec* >( &this->mColor, op, MOAIAttrOp::ATTR_READ );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
USColorVec MOAIColor::GetColorTrait () {

	return this->mColor;
}

//----------------------------------------------------------------//
MOAIColor::MOAIColor () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
	
	this->Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIColor::~MOAIColor () {
}

//----------------------------------------------------------------//
void MOAIColor::OnDepNodeUpdate () {

	this->mColor = *this;

	USColorVec* color = 0;
	
	color = this->GetLinkedValue < USColorVec* >( MOAIColorAttr::Pack ( INHERIT_COLOR ), 0 );
	if ( color ) {
		this->mColor.Modulate(*color);
	}

	color = this->GetLinkedValue < USColorVec* >( MOAIColorAttr::Pack ( INHERIT_COLOR_RAW ), 0 );
	if ( color ) {
		this->Set(color->mR, color->mG, color->mB, color->mA);
		this->mColor = *color;
	}

	color = this->GetLinkedValue < USColorVec* >( MOAIColorAttr::Pack ( ADD_COLOR ), 0 );
	if ( color ) {
		this->mColor.Add ( *color );
	}

	this->mColor.Modulate(USColorVec(this->mA,this->mA, this->mA, 1.0f));


}

//----------------------------------------------------------------//
void MOAIColor::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_R_COL", MOAIColorAttr::Pack ( ATTR_R_COL ));
	state.SetField ( -1, "ATTR_G_COL", MOAIColorAttr::Pack ( ATTR_G_COL ));
	state.SetField ( -1, "ATTR_B_COL", MOAIColorAttr::Pack ( ATTR_B_COL ));
	state.SetField ( -1, "ATTR_OPACITY", MOAIColorAttr::Pack ( ATTR_OPACITY ));
	
	state.SetField ( -1, "ADD_COLOR", MOAIColorAttr::Pack ( ADD_COLOR ));
	state.SetField ( -1, "INHERIT_COLOR_RAW", MOAIColorAttr::Pack ( INHERIT_COLOR_RAW ));
	state.SetField ( -1, "INHERIT_COLOR", MOAIColorAttr::Pack ( INHERIT_COLOR ));
	state.SetField ( -1, "COLOR_TRAIT", MOAIColorAttr::Pack ( COLOR_TRAIT ));
}

//----------------------------------------------------------------//
void MOAIColor::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getColor",				_getColor },
		{ "getOpacity",				_getOpacity},
		{ "setColor",				_setColor },
		{ "setOpacity",				_setOpacity },
		{ "setParent",				_setParent },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
