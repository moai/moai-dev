// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAISim.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getColor
	@text	Return the color.
	
	@in		MOAIProp self
	@out	number rDelta
	@out	number gDelta
	@out	number bDelta
	@out	number aDelta
*/
int MOAIColor::_getColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "U" )

	state.Push ( self->mR );
	state.Push ( self->mG );
	state.Push ( self->mB );
	state.Push ( self->mA );
	
	return 4;
}

//----------------------------------------------------------------//
/**	@lua	moveColor
	@text	Animate the color by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAIColor self
	@in		number rDelta		Delta to be added to r.
	@in		number gDelta		Delta to be added to g.
	@in		number bDelta		Delta to be added to b.
	@in		number aDelta		Delta to be added to a.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAIColor::_moveColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "UNNNNN" )

	float delay		= state.GetValue < float >( 6, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 7, ZLInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForMove ( state, 2, self, 4, mode,
			AttrID::Pack ( ATTR_R_COL ).ToRaw (), 0.0f,
			AttrID::Pack ( ATTR_G_COL ).ToRaw (), 0.0f,
			AttrID::Pack ( ATTR_B_COL ).ToRaw (), 0.0f,
			AttrID::Pack ( ATTR_A_COL ).ToRaw (), 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ( 0, false );
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	if ( !state.CheckVector ( 2, 4, 0, 0 )) {
		self->mR += state.GetValue < float >( 2, 0.0f );
		self->mG += state.GetValue < float >( 3, 0.0f );
		self->mB += state.GetValue < float >( 4, 0.0f );
		self->mA += state.GetValue < float >( 5, 0.0f );
		self->ScheduleUpdate ();
	}
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIColor::_packRGBA ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "" )

	float r		= state.GetValue < float >( 1, 1.0f );
	float g		= state.GetValue < float >( 2, 1.0f );
	float b		= state.GetValue < float >( 3, 1.0f );
	float a		= state.GetValue < float >( 4, 1.0f );

	state.Push ( ZLColor::PackRGBA ( r, g, b, a ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	seekColor
	@text	Animate the color by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAIColor self
	@in		number rGoal		Desired resulting value for r.
	@in		number gGoal		Desired resulting value for g.
	@in		number bGoal		Desired resulting value for b.
	@in		number aGoal		Desired resulting value for a.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAIColor::_seekColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "UNNNNN" )

	float delay		= state.GetValue < float >( 6, 0.0f );
	
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 7, ZLInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForSeek ( state, 2, self, 4, mode,
			AttrID::Pack ( ATTR_R_COL ).ToRaw (), self->mR, 0.0f,
			AttrID::Pack ( ATTR_G_COL ).ToRaw (), self->mG, 0.0f,
			AttrID::Pack ( ATTR_B_COL ).ToRaw (), self->mB, 0.0f,
			AttrID::Pack ( ATTR_A_COL ).ToRaw (), self->mA, 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ( 0, false );
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	ZLColorVec color = state.GetColor ( 2, 0.0f, 0.0f, 0.0f, 0.0f );
	if ( !color.IsEqual ( *self )) {
		self->Set ( color.mR, color.mG, color.mB, color.mA );
		self->ScheduleUpdate ();
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setColor
	@text	Initialize the color.
	
	@in		MOAIColor self
	@in		number r	Default value is 0.
	@in		number g	Default value is 0.
	@in		number b	Default value is 0.
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIColor::_setColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "UNNN" )

	ZLColorVec color = state.GetColor ( 2, 0.0f, 0.0f, 0.0f, 1.0f );
	if ( !color.IsEqual ( *self )) {
		self->Set ( color.mR, color.mG, color.mB, color.mA );
		self->ScheduleUpdate ();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAIColor self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIColor::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "U" )

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2, true );
	
	self->SetAttrLink ( AttrID_INHERIT_COLOR (), parent, AttrID_COLOR_TRAIT ());
	
	//MOAILogF ( state, MOAISTRING_FunctionDeprecated_S, "setParent" );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIColor::_unpackRGBA ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "" )

	u32 rgba = state.GetValue < u32 >( 1, 0xffffffff );

	ZLColorVec color;
	color.SetRGBA ( rgba );

	state.Push ( color );
	return 4;
}

//================================================================//
// MOAIColor
//================================================================//

//----------------------------------------------------------------//
MOAIColor* MOAIColor::AffirmColor ( MOAILuaState& state, int idx ) {

	MOAIColor* color = 0;
	
	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		color = state.GetLuaObject < MOAIColor >( idx, false );
	}
	else {
	
		float r = state.GetValue < float >( 2, 0.0f );
		float g = state.GetValue < float >( 3, 0.0f );
		float b = state.GetValue < float >( 4, 0.0f );
		float a = state.GetValue < float >( 5, 1.0f );

		color = new MOAIColor ();
		color->Set ( r, g, b, a );
		color->ScheduleUpdate ();
	}
	return color;
}

//----------------------------------------------------------------//
ZLColorVec MOAIColor::GetColorTrait () const {

	return this->mColor;
}

//----------------------------------------------------------------//
bool MOAIColor::IsClear () {

	return this->mColor.IsClear ();
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
void MOAIColor::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
	state.SetField ( -1, "ATTR_R_COL",		AttrID_ATTR_R_COL ().ToRaw ());
	state.SetField ( -1, "ATTR_G_COL",		AttrID_ATTR_G_COL ().ToRaw ());
	state.SetField ( -1, "ATTR_B_COL",		AttrID_ATTR_B_COL ().ToRaw ());
	state.SetField ( -1, "ATTR_A_COL",		AttrID_ATTR_A_COL ().ToRaw ());
	
	state.SetField ( -1, "ADD_COLOR",		AttrID_ADD_COLOR ().ToRaw ());
	state.SetField ( -1, "INHERIT_COLOR",	AttrID_INHERIT_COLOR ().ToRaw ());
	state.SetField ( -1, "COLOR_TRAIT",		AttrID_COLOR_TRAIT ().ToRaw ());
	
	luaL_Reg regTable [] = {
		{ "packRGBA",				_packRGBA },
		{ "unpackRGBA",				_unpackRGBA },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIColor::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
	luaL_Reg regTable [] = {
		{ "getColor",				_getColor },
		{ "moveColor",				_moveColor },
		{ "seekColor",				_seekColor },
		{ "setColor",				_setColor },
		{ "setParent",				_setParent },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIColor::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {

		switch ( attrID.Unpack ()) {
		
			case ATTR_R_COL:
				this->mR = ZLFloat::Clamp ( attr.Apply ( this->mR, op, ZLAttribute::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
				
			case ATTR_G_COL:
				this->mG = ZLFloat::Clamp ( attr.Apply ( this->mG, op, ZLAttribute::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
				
			case ATTR_B_COL:
				this->mB = ZLFloat::Clamp ( attr.Apply ( this->mB, op, ZLAttribute::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
				
			case ATTR_A_COL:
				this->mA = ZLFloat::Clamp ( attr.Apply ( this->mA, op, ZLAttribute::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
				
			case COLOR_TRAIT:
				attr.ApplyNoAdd ( this->mColor, op, ZLAttribute::ATTR_READ );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIColor::MOAINode_Update () {

	this->mColor = *this;
	
	ZLAttribute attr;
	if ( this->PullLinkedAttr ( AttrID::Pack ( INHERIT_COLOR ), attr )) {
		this->mColor.Modulate ( attr.GetValue ( ZLColorVec::WHITE ));
	}
	
	if ( this->PullLinkedAttr ( AttrID::Pack ( ADD_COLOR ), attr )) {
		this->mColor.Add ( attr.GetValue ( ZLColorVec::WHITE ));
	}
}

