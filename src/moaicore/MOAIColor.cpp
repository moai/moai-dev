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

	float r			= state.GetValue < float >( 2, 0.0f );
	float g			= state.GetValue < float >( 3, 0.0f );
	float b			= state.GetValue < float >( 4, 0.0f );
	float a			= state.GetValue < float >( 5, 0.0f );
	float delay		= state.GetValue < float >( 6, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 7, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 4 );
		
		action->SetLink ( 0, self, MOAIColorAttr::Pack ( ATTR_R_COL ), r, mode );
		action->SetLink ( 1, self, MOAIColorAttr::Pack ( ATTR_G_COL ), g, mode );
		action->SetLink ( 2, self, MOAIColorAttr::Pack ( ATTR_B_COL ), b, mode );
		action->SetLink ( 3, self, MOAIColorAttr::Pack ( ATTR_A_COL ), a, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mR += r;
	self->mG += g;
	self->mB += b;
	self->mA += a;
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

	float r			= state.GetValue < float >( 2, 0.0f );
	float g			= state.GetValue < float >( 3, 0.0f );
	float b			= state.GetValue < float >( 4, 0.0f );
	float a			= state.GetValue < float >( 5, 0.0f );
	float delay		= state.GetValue < float >( 6, 0.0f );
	
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 7, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 4 );
		
		action->SetLink ( 0, self, MOAIColorAttr::Pack ( ATTR_R_COL ), r - self->mR, mode );
		action->SetLink ( 1, self, MOAIColorAttr::Pack ( ATTR_G_COL ), g - self->mG, mode );
		action->SetLink ( 2, self, MOAIColorAttr::Pack ( ATTR_B_COL ), b - self->mB, mode );
		action->SetLink ( 3, self, MOAIColorAttr::Pack ( ATTR_A_COL ), a - self->mA, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mR = r;
	self->mG = g;
	self->mB = b;
	self->mA = a;
	self->ScheduleUpdate ();
	
	return 0;
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

//----------------------------------------------------------------//
/**	@name	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAIColor self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIColor::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIColor, "U" )

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2 );
	
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
				this->mR = USFloat::Clamp ( attrOp.Apply ( this->mR, op, MOAINode::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			case ATTR_G_COL:
				this->mG = USFloat::Clamp ( attrOp.Apply ( this->mG, op, MOAINode::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			case ATTR_B_COL:
				this->mB = USFloat::Clamp ( attrOp.Apply ( this->mB, op, MOAINode::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			case ATTR_A_COL:
				this->mA = USFloat::Clamp ( attrOp.Apply ( this->mA, op, MOAINode::ATTR_READ_WRITE ), 0.0f, 1.0f );
				return true;
			case COLOR_TRAIT:
				attrOp.Apply < USColorVec >( this->mColor, op, MOAINode::ATTR_READ );
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
}

//----------------------------------------------------------------//
MOAIColor::~MOAIColor () {
}

//----------------------------------------------------------------//
void MOAIColor::OnDepNodeUpdate () {

	this->mColor = *this;

	USColorVec color;
	
	if ( this->GetLinkedValue < USColorVec >( MOAIColorAttr::Pack ( INHERIT_COLOR ), color )) {
		this->mColor.Modulate ( color );
	}
	
	if ( this->GetLinkedValue < USColorVec >( MOAIColorAttr::Pack ( ADD_COLOR ), color )) {
		this->mColor.Add ( color );
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
