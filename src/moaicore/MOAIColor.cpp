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

	float delay		= state.GetValue < float >( 6, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 7, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForMove ( state, 2, self, 4, mode,
			MOAIColorAttr::Pack ( ATTR_R_COL ), 0.0f,
			MOAIColorAttr::Pack ( ATTR_G_COL ), 0.0f,
			MOAIColorAttr::Pack ( ATTR_B_COL ), 0.0f,
			MOAIColorAttr::Pack ( ATTR_A_COL ), 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mR += state.GetValue < float >( 2, 0.0f );
	self->mG += state.GetValue < float >( 3, 0.0f );
	self->mB += state.GetValue < float >( 4, 0.0f );
	self->mA += state.GetValue < float >( 5, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
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

	float delay		= state.GetValue < float >( 6, 0.0f );
	
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 7, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForSeek ( state, 2, self, 4, mode,
			MOAIColorAttr::Pack ( ATTR_R_COL ), self->mR, 0.0f,
			MOAIColorAttr::Pack ( ATTR_G_COL ), self->mG, 0.0f,
			MOAIColorAttr::Pack ( ATTR_B_COL ), self->mB, 0.0f,
			MOAIColorAttr::Pack ( ATTR_A_COL ), self->mA, 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mR = state.GetValue < float >( 2, 0.0f );
	self->mG = state.GetValue < float >( 3, 0.0f );
	self->mB = state.GetValue < float >( 4, 0.0f );
	self->mA = state.GetValue < float >( 5, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setColor
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

	float r = state.GetValue < float >( 2, 0.0f );
	float g = state.GetValue < float >( 3, 0.0f );
	float b = state.GetValue < float >( 4, 0.0f );
	float a = state.GetValue < float >( 5, 1.0f );

	self->Set ( r, g, b, a );
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
				this->mR = USFloat::Clamp ( attrOp.Apply ( this->mR, op, MOAIAttrOp::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			case ATTR_G_COL:
				this->mG = USFloat::Clamp ( attrOp.Apply ( this->mG, op, MOAIAttrOp::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			case ATTR_B_COL:
				this->mB = USFloat::Clamp ( attrOp.Apply ( this->mB, op, MOAIAttrOp::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			case ATTR_A_COL:
				this->mA = USFloat::Clamp ( attrOp.Apply ( this->mA, op, MOAIAttrOp::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
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
		this->mColor.Modulate ( *color );
	}
	
	color = this->GetLinkedValue < USColorVec* >( MOAIColorAttr::Pack ( ADD_COLOR ), 0 );
	if ( color ) {
		this->mColor.Add ( *color );
	}
}

//----------------------------------------------------------------//
void MOAIColor::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_R_COL", MOAIColorAttr::Pack ( ATTR_R_COL ));
	state.SetField ( -1, "ATTR_G_COL", MOAIColorAttr::Pack ( ATTR_G_COL ));
	state.SetField ( -1, "ATTR_B_COL", MOAIColorAttr::Pack ( ATTR_B_COL ));
	state.SetField ( -1, "ATTR_A_COL", MOAIColorAttr::Pack ( ATTR_A_COL ));
	
	state.SetField ( -1, "ADD_COLOR", MOAIColorAttr::Pack ( ADD_COLOR ));
	state.SetField ( -1, "INHERIT_COLOR", MOAIColorAttr::Pack ( INHERIT_COLOR ));
	state.SetField ( -1, "COLOR_TRAIT", MOAIColorAttr::Pack ( COLOR_TRAIT ));
}

//----------------------------------------------------------------//
void MOAIColor::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "moveColor",				_moveColor },
		{ "seekColor",				_seekColor },
		{ "setColor",				_setColor },
		{ "setParent",				_setParent },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
