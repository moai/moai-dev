// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIForceAction.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>moveColor ( self, red, green, blue, alpha, duration )</tt>\n
	\n
	Animate the color by adding a delta.
	@param self (in)
	@param red (in) Red color value.
	@param green (in) Green color value.
	@param blue (in) Blue color value.
	@param alpha (in) Alpha transparency value.
	@param delay (in) Time period to move.
*/
int MOAIColor::_moveColor ( lua_State* L ) {
	LUA_SETUP ( MOAIColor, "UNNNNN" )

	MOAIForceAction* action = new MOAIForceAction ();
	action->ReserveForces ( 4 );
	
	float r			= state.GetValue < float >( 2, 0.0f );
	float g			= state.GetValue < float >( 3, 0.0f );
	float b			= state.GetValue < float >( 4, 0.0f );
	float a			= state.GetValue < float >( 5, 0.0f );
	float delay		= state.GetValue < float >( 6, 0.0f );
	u32 mode		= state.GetValue < u32 >( 7, USInterpolate::kSmooth );
	
	action->SetForce ( 0, self, MOAIColor::ATTR_R_COL, r, mode );
	action->SetForce ( 1, self, MOAIColor::ATTR_G_COL, g, mode );
	action->SetForce ( 2, self, MOAIColor::ATTR_B_COL, b, mode );
	action->SetForce ( 3, self, MOAIColor::ATTR_A_COL, a, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>_seekColor ( self, red, green, blue, alpha, duration )</tt>\n
	\n
	Animate the color by adding a delta.
	@param self (in)
	@param red (in) Red color value.
	@param green (in) Green color value.
	@param blue (in) Blue color value.
	@param alpha (in) Alpha transparency value.
	@param delay (in) Time period to move.
*/
int MOAIColor::_seekColor ( lua_State* L ) {
	LUA_SETUP ( MOAIColor, "UNNNNN" )

	MOAIForceAction* action = new MOAIForceAction ();
	action->ReserveForces ( 4 );
	
	float r			= state.GetValue < float >( 2, 0.0f );
	float g			= state.GetValue < float >( 3, 0.0f );
	float b			= state.GetValue < float >( 4, 0.0f );
	float a			= state.GetValue < float >( 5, 0.0f );
	float delay		= state.GetValue < float >( 6, 0.0f );
	u32 mode		= state.GetValue < u32 >( 7, USInterpolate::kSmooth );
	
	action->SetForce ( 0, self, MOAIColor::ATTR_R_COL, r - self->mR, mode );
	action->SetForce ( 1, self, MOAIColor::ATTR_G_COL, g - self->mG, mode );
	action->SetForce ( 2, self, MOAIColor::ATTR_B_COL, b - self->mB, mode );
	action->SetForce ( 3, self, MOAIColor::ATTR_A_COL, a - self->mA, mode );
	
	action->SetDelay ( delay );
	action->Start ();
	action->PushLuaInstance ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>setColor ( self, red, green, blue, alpha )</tt>\n
	\n
	Initializes the color.
	@param self (in)
	@param red (in) Red color value.  Floating point.  Range between 0.0 and 1.0.
	@param green (in) Green color value.  Floating point.  Range between 0.0 and 1.0.
	@param blue (in) Blue color value.  Floating point.  Range between 0.0 and 1.0.
	@param alpha (in) Alpha transparency value.  Floating point.  Range between 0.0 and 1.0.
*/
int MOAIColor::_setColor ( lua_State* L ) {
	LUA_SETUP ( MOAIColor, "UNNN" )

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
void MOAIColor::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch( attrID ) {
		case ATTR_R_COL:
			this->mR = USFloat::Clamp ( attrOp.Op ( this->mR ), 0.0f, 1.0f );
			break;
		case ATTR_G_COL:
			this->mG = USFloat::Clamp ( attrOp.Op ( this->mG ), 0.0f, 1.0f );
			break;
		case ATTR_B_COL:
			this->mB = USFloat::Clamp ( attrOp.Op ( this->mB ), 0.0f, 1.0f );
			break;
		case ATTR_A_COL:
			this->mA = USFloat::Clamp ( attrOp.Op ( this->mA ), 0.0f, 1.0f );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIColor::Bind () {

	this->LoadGfxState ();
}

//----------------------------------------------------------------//
u32 MOAIColor::CountAttributes () {

	return TOTAL_ATTR;
}

//----------------------------------------------------------------//
MOAIColor::MOAIColor () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIShader )
	RTTI_END
	
	this->Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIColor::~MOAIColor () {
}

//----------------------------------------------------------------//
void MOAIColor::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIShader::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_R_COL", ( u32 )ATTR_R_COL );
	state.SetField ( -1, "ATTR_G_COL", ( u32 )ATTR_G_COL );
	state.SetField ( -1, "ATTR_B_COL", ( u32 )ATTR_B_COL );
	state.SetField ( -1, "ATTR_A_COL", ( u32 )ATTR_A_COL );
}

//----------------------------------------------------------------//
void MOAIColor::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIShader::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
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
