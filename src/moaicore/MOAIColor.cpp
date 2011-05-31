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
/**	@name	moveColor
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

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 4 );
	
	float r			= state.GetValue < float >( 2, 0.0f );
	float g			= state.GetValue < float >( 3, 0.0f );
	float b			= state.GetValue < float >( 4, 0.0f );
	float a			= state.GetValue < float >( 5, 0.0f );
	float delay		= state.GetValue < float >( 6, 0.0f );
	u32 mode		= state.GetValue < u32 >( 7, USInterpolate::kSmooth );
	
	action->SetLink ( 0, self, MOAIColorAttr::Pack ( ATTR_R_COL ), r, mode );
	action->SetLink ( 1, self, MOAIColorAttr::Pack ( ATTR_G_COL ), g, mode );
	action->SetLink ( 2, self, MOAIColorAttr::Pack ( ATTR_B_COL ), b, mode );
	action->SetLink ( 3, self, MOAIColorAttr::Pack ( ATTR_A_COL ), a, mode );
	
	action->SetLength ( delay );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	seekColor
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

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 4 );
	
	float r			= state.GetValue < float >( 2, 0.0f );
	float g			= state.GetValue < float >( 3, 0.0f );
	float b			= state.GetValue < float >( 4, 0.0f );
	float a			= state.GetValue < float >( 5, 0.0f );
	float delay		= state.GetValue < float >( 6, 0.0f );
	u32 mode		= state.GetValue < u32 >( 7, USInterpolate::kSmooth );
	
	action->SetLink ( 0, self, MOAIColorAttr::Pack ( ATTR_R_COL ), r - self->mR, mode );
	action->SetLink ( 1, self, MOAIColorAttr::Pack ( ATTR_G_COL ), g - self->mG, mode );
	action->SetLink ( 2, self, MOAIColorAttr::Pack ( ATTR_B_COL ), b - self->mB, mode );
	action->SetLink ( 3, self, MOAIColorAttr::Pack ( ATTR_A_COL ), a - self->mA, mode );
	
	action->SetLength ( delay );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setColor
	@text	Initialize the color.
	
	@in		MOAIColor self
	@in		number r
	@in		number g
	@in		number b
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIColor::_setColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "UNNN" )

	float r = state.GetValue < float >( 2, 0.0f );
	float g = state.GetValue < float >( 3, 0.0f );
	float b = state.GetValue < float >( 4, 0.0f );
	float a = state.GetValue < float >( 5, 1.0f );

	self->Set ( r, g, b, a );
	self->ScheduleUpdate ();

	return 0;
}

//================================================================//
// MOAIColor
//================================================================//

//----------------------------------------------------------------//
bool MOAIColor::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	if ( MOAIColorAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_R_COL:
				this->mR = USFloat::Clamp ( attrOp.Op ( this->mR ), 0.0f, 1.0f );
				return true;
			case ATTR_G_COL:
				this->mG = USFloat::Clamp ( attrOp.Op ( this->mG ), 0.0f, 1.0f );
				return true;
			case ATTR_B_COL:
				this->mB = USFloat::Clamp ( attrOp.Op ( this->mB ), 0.0f, 1.0f );
				return true;
			case ATTR_A_COL:
				this->mA = USFloat::Clamp ( attrOp.Op ( this->mA ), 0.0f, 1.0f );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
MOAIColor::MOAIColor () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
	
	this->Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIColor::~MOAIColor () {
}

//----------------------------------------------------------------//
void MOAIColor::RegisterLuaClass ( USLuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_R_COL", MOAIColorAttr::Pack ( ATTR_R_COL ));
	state.SetField ( -1, "ATTR_G_COL", MOAIColorAttr::Pack ( ATTR_G_COL ));
	state.SetField ( -1, "ATTR_B_COL", MOAIColorAttr::Pack ( ATTR_B_COL ));
	state.SetField ( -1, "ATTR_A_COL", MOAIColorAttr::Pack ( ATTR_A_COL ));
}

//----------------------------------------------------------------//
void MOAIColor::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "moveColor",				_moveColor },
		{ "seekColor",				_seekColor },
		{ "setColor",				_setColor },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIColor::ToString () {

	STLString repr;

	return repr;
}
