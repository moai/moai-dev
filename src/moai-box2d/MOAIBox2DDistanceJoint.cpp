// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DDistanceJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getDampingRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@out	number dampingRatio
*/
int MOAIBox2DDistanceJoint::_getDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	state.Push ( joint->GetDampingRatio ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFrequency
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@out	number frequency				In Hz.
*/
int MOAIBox2DDistanceJoint::_getFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	state.Push ( joint->GetFrequency ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getLength
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@out	number length					In units, converted from meters.
*/
int MOAIBox2DDistanceJoint::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	state.Push ( joint->GetLength () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setDampingRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@opt	number dampingRatio		Default value is 0.
	@out	nil
*/
int MOAIBox2DDistanceJoint::_setDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float dampingRatio = state.GetValue < float >( 2, 0.0f );

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	joint->SetDampingRatio ( dampingRatio );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFrequency
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@opt	number frequency		In Hz. Default value is 0.
	@out	nil
*/
int MOAIBox2DDistanceJoint::_setFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float frequency = state.GetValue < float >( 2, 0.0f );

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	joint->SetFrequency ( frequency );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setLength
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@opt	number length		in units, converted to meters. Default value is 0.
	@out	nil
*/
int MOAIBox2DDistanceJoint::_setLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float length = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	joint->SetLength ( length );
	
	return 0;
}

//================================================================//
// MOAIBox2DDistanceJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DDistanceJoint::MOAIBox2DDistanceJoint ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIBox2DPrim ( context ),
	MOAIBox2DJoint ( context ) {
	
	RTTI_BEGIN ( MOAIBox2DDistanceJoint )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIBox2DDistanceJoint >)
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DDistanceJoint::~MOAIBox2DDistanceJoint () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DDistanceJoint::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIBox2DDistanceJoint::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getDampingRatio",		_getDampingRatio },
		{ "getFrequency",			_getFrequency },
		{ "getLength",				_getLength },
		{ "setDampingRatio",		_setDampingRatio },
		{ "setFrequency",			_setFrequency },
		{ "setLength",				_setLength },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
