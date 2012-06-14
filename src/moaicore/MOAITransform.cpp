// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	addLoc
	@text	Adds a delta to the transform's location.
	
	@in		MOAITransform self
	@in		number xDelta
	@in		number yDelta
	@in		number zDelta
	@out	nil
*/
int MOAITransform::_addLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	USVec3D loc = self->GetLoc ();
	
	loc.mX += state.GetValue < float >( 2, 0.0f );
	loc.mY += state.GetValue < float >( 3, 0.0f );
	loc.mZ += state.GetValue < float >( 4, 0.0f );
	
	self->SetLoc ( loc );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addPiv
	@text	Adds a delta to the transform's pivot.
	
	@in		MOAITransform self
	@in		number xDelta
	@in		number yDelta
	@in		number zDelta
	@out	nil
*/
int MOAITransform::_addPiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	USVec3D piv = self->GetLoc ();
	
	piv.mX += state.GetValue < float >( 2, 0.0f );
	piv.mY += state.GetValue < float >( 3, 0.0f );
	piv.mZ += state.GetValue < float >( 4, 0.0f );
	
	self->SetPiv ( piv );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addRot
	@text	Adds a delta to the transform's rotation
	
	@in		MOAITransform self
	@in		number xDelta		In degrees.
	@in		number yDelta		In degrees.
	@in		number zDelta		In degrees.
	@out	nil
*/
int MOAITransform::_addRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	USVec3D rot = self->GetRot ();
	rot.mX += state.GetValue < float >( 2, 0.0f );
	rot.mY += state.GetValue < float >( 3, 0.0f );
	rot.mZ += state.GetValue < float >( 4, 0.0f );
	
	self->SetRot ( rot );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addScl
	@text	Adds a delta to the transform's scale
	
	@in		MOAITransform self
	@in		number xSclDelta
	@opt	number ySclDelta		Default value is xSclDelta.
	@opt	number zSclDelta		Default value is 0.
	@out	nil
*/
int MOAITransform::_addScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	USVec3D scl = self->GetScl ();
	
	float xSclDelta = state.GetValue < float >( 2, 0.0f );
	float ySclDelta = state.GetValue < float >( 3, xSclDelta );
	float zSclDelta = state.GetValue < float >( 4, 0.0f );
	
	scl.mX += xSclDelta;
	scl.mY += ySclDelta;
	scl.mZ += zSclDelta;
	
	self->SetScl ( scl );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getLoc
	@text	Returns the transform's current location.
	
	@in		MOAITransform self
	@out	number xLoc
	@out	number yLoc
	@out	number zLoc
*/
int	MOAITransform::_getLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	lua_pushnumber ( state, self->mLoc.mX );
	lua_pushnumber ( state, self->mLoc.mY );
	lua_pushnumber ( state, self->mLoc.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@name	getPiv
	@text	Returns the transform's current pivot.
	
	@in		MOAITransform self
	@out	number xPiv
	@out	number yPiv
	@out	number zPiv
*/
int	MOAITransform::_getPiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	lua_pushnumber ( state, self->mPiv.mX );
	lua_pushnumber ( state, self->mPiv.mY );
	lua_pushnumber ( state, self->mPiv.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@name	getRot
	@text	Returns the transform's current rotation.
	
	@in		MOAITransform self
	@out	number xRot			Rotation in degrees.
	@out	number yRot			Rotation in degrees.
	@out	number zRot			Rotation in degrees.
*/
int	MOAITransform::_getRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	lua_pushnumber ( state, self->mRot.mX );
	lua_pushnumber ( state, self->mRot.mY );
	lua_pushnumber ( state, self->mRot.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@name	getScl
	@text	Returns the transform's current scale.
	
	@in		MOAITransform self
	@out	number xScl
	@out	number yScl
	@out	number zScl
*/
int	MOAITransform::_getScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	lua_pushnumber ( state, self->mScale.mX );
	lua_pushnumber ( state, self->mScale.mY );
	lua_pushnumber ( state, self->mScale.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@name	modelToWorld
	@text	Transform a point in model space to world space.
	
	@in		MOAITransform self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number z			Default value is 0.
	@out	number x
	@out	number y
	@out	number z
*/
int MOAITransform::_modelToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	USVec3D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );

	USAffine3D modelToWorld = self->GetLocalToWorldMtx ();
	modelToWorld.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );
	lua_pushnumber ( state, loc.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@name	move
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xDelta		Delta to be added to x.
	@in		number yDelta		Delta to be added to y.
	@in		number zDelta		Delta to be added to z.
	@in		number xRotDelta	Delta to be added to x rot (in degrees).
	@in		number yRotDelta	Delta to be added to y rot (in degrees).
	@in		number zRotDelta	Delta to be added to z rot (in degrees).
	@in		number xSclDelta	Delta to be added to x scale.
	@in		number ySclDelta	Delta to be added to y scale.
	@in		number zSclDelta	Delta to be added to z scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_move ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	float delay		= state.GetValue < float >( 11, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 12, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForMove ( state, 2, self, 9, mode,
			MOAITransformAttr::Pack ( ATTR_X_LOC ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_LOC ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_LOC ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_X_ROT ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_ROT ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_ROT ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_X_SCL ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_SCL ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_SCL ), 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );
		
		return 1;
	}

	self->mLoc.mX += state.GetValue < float >( 2, 0.0f );
	self->mLoc.mY += state.GetValue < float >( 3, 0.0f );
	self->mLoc.mZ += state.GetValue < float >( 4, 0.0f );
	
	self->mRot.mX += state.GetValue < float >( 5, 0.0f );
	self->mRot.mY += state.GetValue < float >( 6, 0.0f );
	self->mRot.mZ += state.GetValue < float >( 7, 0.0f );
	
	self->mScale.mX += state.GetValue < float >( 8, 0.0f );
	self->mScale.mY += state.GetValue < float >( 9, 0.0f );
	self->mScale.mZ += state.GetValue < float >( 10, 0.0f );
	
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	moveLoc
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xDelta		Delta to be added to x.
	@in		number yDelta		Delta to be added to y.
	@in		number zDelta		Delta to be added to z.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_moveLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	float delay		= state.GetValue < float >( 5, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 6, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForMove ( state, 2, self, 3, mode,
			MOAITransformAttr::Pack ( ATTR_X_LOC ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_LOC ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_LOC ), 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mLoc.mX += state.GetValue < float >( 2, 0.0f );
	self->mLoc.mY += state.GetValue < float >( 3, 0.0f );
	self->mLoc.mZ += state.GetValue < float >( 4, 0.0f );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	movePiv
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xDelta		Delta to be added to xPiv.
	@in		number yDelta		Delta to be added to yPiv.
	@in		number zDelta		Delta to be added to zPiv.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_movePiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	float delay		= state.GetValue < float >( 5, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 6, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForMove ( state, 2, self, 3, mode,
			MOAITransformAttr::Pack ( ATTR_X_PIV ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_PIV ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_PIV ), 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mPiv.mX += state.GetValue < float >( 2, 0.0f );
	self->mPiv.mY += state.GetValue < float >( 3, 0.0f );
	self->mPiv.mZ += state.GetValue < float >( 4, 0.0f );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	moveRot
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xDelta		Delta to be added to xRot (in degrees).
	@in		number yDelta		Delta to be added to yRot (in degrees).
	@in		number zDelta		Delta to be added to zRot (in degrees).
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_moveRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	float delay		= state.GetValue < float >( 5, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 6, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForMove ( state, 2, self, 3, mode,
			MOAITransformAttr::Pack ( ATTR_X_ROT ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_ROT ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_ROT ), 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mRot.mX += state.GetValue < float >( 2, 0.0f );
	self->mRot.mY += state.GetValue < float >( 3, 0.0f );
	self->mRot.mZ += state.GetValue < float >( 4, 0.0f );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	moveScl
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xSclDelta	Delta to be added to x scale.
	@in		number ySclDelta	Delta to be added to y scale.
	@in		number zSclDelta	Delta to be added to z scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_moveScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	float delay		= state.GetValue < float >( 5, 0.0f );

	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 6, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForMove ( state, 2, self, 3, mode,
			MOAITransformAttr::Pack ( ATTR_X_SCL ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_SCL ), 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_SCL ), 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mScale.mX += state.GetValue < float >( 2, 0.0f );
	self->mScale.mY += state.GetValue < float >( 3, 0.0f );
	self->mScale.mZ += state.GetValue < float >( 4, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seek
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xGoal		Desired resulting value for x.
	@in		number yGoal		Desired resulting value for y.
	@in		number zGoal		Desired resulting value for z.
	@in		number xRotGoal		Desired resulting value for x rot (in degrees).
	@in		number yRotGoal		Desired resulting value for y rot (in degrees).
	@in		number zRotGoal		Desired resulting value for z rot (in degrees).
	@in		number xSclGoal		Desired resulting value for x scale.
	@in		number ySclGoal		Desired resulting value for y scale.
	@in		number zSclGoal		Desired resulting value for z scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seek ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	float delay		= state.GetValue < float >( 11, 0.0f );
	
	if ( delay > 0.0f ) {
		
		u32 mode = state.GetValue < u32 >( 12, USInterpolate::kSmooth );

		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForSeek ( state, 2, self, 9, mode,
			MOAITransformAttr::Pack ( ATTR_X_LOC ), self->mLoc.mX, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_LOC ), self->mLoc.mY, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_LOC ), self->mLoc.mZ, 0.0f,
			MOAITransformAttr::Pack ( ATTR_X_ROT ), self->mRot.mX, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_ROT ), self->mRot.mY, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_ROT ), self->mRot.mZ, 0.0f,
			MOAITransformAttr::Pack ( ATTR_X_SCL ), self->mScale.mX, 1.0f,
			MOAITransformAttr::Pack ( ATTR_Y_SCL ), self->mScale.mY, 1.0f,
			MOAITransformAttr::Pack ( ATTR_Z_SCL ), self->mScale.mZ, 1.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mLoc.mX = state.GetValue < float >( 2, 0.0f );
	self->mLoc.mY = state.GetValue < float >( 3, 0.0f );
	self->mLoc.mZ = state.GetValue < float >( 4, 0.0f );
	self->mRot.mX = state.GetValue < float >( 5, 0.0f );
	self->mRot.mY = state.GetValue < float >( 6, 0.0f );
	self->mRot.mZ = state.GetValue < float >( 7, 0.0f );
	self->mScale.mX = state.GetValue < float >( 8, 0.0f );
	self->mScale.mY = state.GetValue < float >( 9, 0.0f );
	self->mScale.mZ = state.GetValue < float >( 10, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekLoc
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xGoal		Desired resulting value for x.
	@in		number yGoal		Desired resulting value for y.
	@in		number zGoal		Desired resulting value for z.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seekLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	float delay		= state.GetValue < float >( 5, 0.0f );
	
	if ( delay > 0.0f ) {

		u32 mode = state.GetValue < u32 >( 6, USInterpolate::kSmooth );		
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForSeek ( state, 2, self, 3, mode,
			MOAITransformAttr::Pack ( ATTR_X_LOC ), self->mLoc.mX, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_LOC ), self->mLoc.mY, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_LOC ), self->mLoc.mZ, 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mLoc.mX = state.GetValue < float >( 2, 0.0f );
	self->mLoc.mY = state.GetValue < float >( 3, 0.0f );
	self->mLoc.mZ = state.GetValue < float >( 4, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekPiv
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xGoal		Desired resulting value for xPiv.
	@in		number yGoal		Desired resulting value for yPiv.
	@in		number zGoal		Desired resulting value for zPiv.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seekPiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	float delay		= state.GetValue < float >( 5, 0.0f );
	
	if ( delay > 0.0f ) {

		u32 mode = state.GetValue < u32 >( 6, USInterpolate::kSmooth );		
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForSeek ( state, 2, self, 3, mode,
			MOAITransformAttr::Pack ( ATTR_X_PIV ), self->mPiv.mX, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_PIV ), self->mPiv.mY, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_PIV ), self->mPiv.mZ, 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mPiv.mX = state.GetValue < float >( 2, 0.0f );
	self->mPiv.mY = state.GetValue < float >( 3, 0.0f );
	self->mPiv.mZ = state.GetValue < float >( 4, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekRot
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xRotGoal		Desired resulting value for x rot (in degrees).
	@in		number yRotGoal		Desired resulting value for y rot (in degrees).
	@in		number zRotGoal		Desired resulting value for z rot (in degrees).
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seekRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	float delay		= state.GetValue < float >( 5, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 6, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForSeek ( state, 2, self, 3, mode,
			MOAITransformAttr::Pack ( ATTR_X_ROT ), self->mRot.mX, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Y_ROT ), self->mRot.mY, 0.0f,
			MOAITransformAttr::Pack ( ATTR_Z_ROT ), self->mRot.mZ, 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mRot.mX = state.GetValue < float >( 2, 0.0f );
	self->mRot.mY = state.GetValue < float >( 3, 0.0f );
	self->mRot.mZ = state.GetValue < float >( 4, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekScl
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xSclGoal		Desired resulting value for x scale.
	@in		number ySclGoal		Desired resulting value for y scale.
	@in		number zSclGoal		Desired resulting value for z scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seekScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	float delay		= state.GetValue < float >( 5, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 6, USInterpolate::kSmooth );

		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForSeek ( state, 2, self, 3, mode,
			MOAITransformAttr::Pack ( ATTR_X_SCL ), self->mScale.mX, 1.0f,
			MOAITransformAttr::Pack ( ATTR_Y_SCL ), self->mScale.mY, 1.0f,
			MOAITransformAttr::Pack ( ATTR_Z_SCL ), self->mScale.mZ, 1.0f
		);
		
		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mScale.mY = state.GetValue < float >( 3, 1.0f );
	self->mScale.mZ = state.GetValue < float >( 4, 1.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLoc
	@text	Sets the transform's location.
	
	@in		MOAITransform self
	@opt	number x				Default value is 0.
	@opt	number y				Default value is 0.
	@opt	number z				Default value is 0.
	@out	nil
*/
int MOAITransform::_setLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	USVec3D loc;
	
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );
	
	self->SetLoc ( loc );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAITransform self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAITransform::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2, true );
	
	self->SetAttrLink ( PACK_ATTR ( MOAITransform, INHERIT_TRANSFORM ), parent, PACK_ATTR ( MOAITransformBase, TRANSFORM_TRAIT ));
	
	//MOAILog ( state, MOAILogMessages::MOAI_FunctionDeprecated_S, "setParent" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPiv
	@text	Sets the transform's pivot.
	
	@in		MOAITransform self
	@opt	number xPiv			Default value is 0.
	@opt	number yPiv			Default value is 0.
	@opt	number zPiv			Default value is 0.
	@out	nil
*/
int MOAITransform::_setPiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	USVec3D piv;
	
	piv.mX = state.GetValue < float >( 2, 0.0f );
	piv.mY = state.GetValue < float >( 3, 0.0f );
	piv.mZ = state.GetValue < float >( 4, 0.0f );
	
	self->SetPiv ( piv );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRot
	@text	Sets the transform's rotation.
	
	@in		MOAITransform self
	@opt	number xRot			Default value is 0.
	@opt	number yRot			Default value is 0.
	@opt	number zRot			Default value is 0.
	@out	nil
*/
int MOAITransform::_setRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	USVec3D rot;
	
	rot.mX = state.GetValue < float >( 2, 0.0f );
	rot.mY = state.GetValue < float >( 3, 0.0f );
	rot.mZ = state.GetValue < float >( 4, 0.0f );
	
	self->SetRot ( rot );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setScl
	@text	Sets the transform's scale.
	
	@in		MOAITransform self
	@in		number xScl
	@opt	number yScl			Default value is xScl.
	@opt	number zScl			Default value is 1.
	@out	nil
*/
int MOAITransform::_setScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	USVec3D scl;
	
	scl.mX = state.GetValue < float >( 2, 0.0f );
	scl.mY = state.GetValue < float >( 3, scl.mX );
	scl.mZ = state.GetValue < float >( 4, 1.0f );
	
	self->SetScl ( scl );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setShearByX
	@text	Sets the shear for the Y and Z axes by X.
	
	@in		MOAITransform self
	@in		number yx			Default value is 0.
	@opt	number zx			Default value is 0.
	@out	nil
*/
int MOAITransform::_setShearByX ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	self->mShearYX = state.GetValue < float >( 2, 0.0f );
	self->mShearZX = state.GetValue < float >( 3, 0.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setShearByY
	@text	Sets the shear for the X and Z axes by Y.
	
	@in		MOAITransform self
	@in		number xy			Default value is 0.
	@opt	number zy			Default value is 0.
	@out	nil
*/
int MOAITransform::_setShearByY ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	self->mShearXY = state.GetValue < float >( 2, 0.0f );
	self->mShearZY = state.GetValue < float >( 3, 0.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setShearByZ
	@text	Sets the shear for the X and Y axes by Z.
	
	@in		MOAITransform self
	@in		number xz			Default value is 0.
	@opt	number yz			Default value is 0.
	@out	nil
*/
int MOAITransform::_setShearByZ ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	self->mShearXZ = state.GetValue < float >( 2, 0.0f );
	self->mShearYZ = state.GetValue < float >( 3, 0.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	worldToModel
	@text	Transform a point in world space to model space.
	
	@in		MOAITransform self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number z			Default value is 0.
	@out	number x
	@out	number y
	@out	number z
*/
int MOAITransform::_worldToModel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )

	USVec3D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );

	USAffine3D worldToModel = self->GetWorldToLocalMtx ();
	worldToModel.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );
	lua_pushnumber ( state, loc.mZ );

	return 3;
}

//================================================================//
// MOAITransform
//================================================================//

//----------------------------------------------------------------//
bool MOAITransform::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAITransformAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_X_PIV:
				this->mPiv.mX = attrOp.Apply ( this->mPiv.mX, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_Y_PIV:
				this->mPiv.mY = attrOp.Apply ( this->mPiv.mY, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_Z_PIV:
				this->mPiv.mZ = attrOp.Apply ( this->mPiv.mZ, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_X_LOC:
				this->mLoc.mX = attrOp.Apply ( this->mLoc.mX, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_Y_LOC:
				this->mLoc.mY = attrOp.Apply ( this->mLoc.mY, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_Z_LOC:
				this->mLoc.mZ = attrOp.Apply ( this->mLoc.mZ, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_X_ROT:
				this->mRot.mX = attrOp.Apply ( this->mRot.mX, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_Y_ROT:
				this->mRot.mY = attrOp.Apply ( this->mRot.mY, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_Z_ROT:
				this->mRot.mZ = attrOp.Apply ( this->mRot.mZ, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_X_SCL:
				this->mScale.mX = attrOp.Apply ( this->mScale.mX, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_Y_SCL:
				this->mScale.mY = attrOp.Apply ( this->mScale.mY, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_Z_SCL:
				this->mScale.mZ = attrOp.Apply ( this->mScale.mZ, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
			case ATTR_ROTATE_QUAT: {
				// TODO: cache rotation as quat to support read/write, delta adds?
				USQuaternion quat;
				attrOp.SetFlags ( MOAIAttrOp::ATTR_READ_WRITE );

				if ( op == MOAIAttrOp::ADD ) {

					quat.Set ( this->mRot.mX, this->mRot.mY, this->mRot.mZ );
					quat = attrOp.Apply < USQuaternion >( quat, op, MOAIAttrOp::ATTR_WRITE );
					quat.Get ( this->mRot.mX, this->mRot.mY, this->mRot.mZ );
				}
				else if ( op != MOAIAttrOp::CHECK ) {

					quat.Set ( 0.0f, 0.0f, 0.0f, 0.0f );
					quat = attrOp.Apply < USQuaternion >( quat, op, MOAIAttrOp::ATTR_WRITE );
					quat.Get ( this->mRot.mX, this->mRot.mY, this->mRot.mZ );
				}
				return true;
			}
			case ATTR_TRANSLATE:
				this->mLoc = attrOp.Apply < USVec3D >( this->mLoc, op, MOAIAttrOp::ATTR_READ_WRITE );
				return true;
		}
	}
	return MOAITransformBase::ApplyAttrOp ( attrID, attrOp, op );
}

//----------------------------------------------------------------//
void MOAITransform::BuildTransforms () {
	
	if ( this->mRot.mZ >= 360.0f ) {
		this->mRot.mZ = ( float )fmod ( this->mRot.mZ, 360.0f );
	}
	else if ( this->mRot.mZ < 0.0f ) {
		this->mRot.mZ = 360.0f + ( float )fmod ( this->mRot.mZ, 360.0f );
	}

	this->mLocalToWorldMtx.ScRoTr (
		this->mScale.mX,
		this->mScale.mY,
		this->mScale.mZ,
		this->mRot.mX * ( float )D2R,
		this->mRot.mY * ( float )D2R,
		this->mRot.mZ * ( float )D2R,
		this->mLoc.mX,
		this->mLoc.mY,
		this->mLoc.mZ
	);
	
	USAffine3D shear;
	shear.Shear ( this->mShearYX, this->mShearZX, this->mShearXY, this->mShearZY, this->mShearXZ, this->mShearYZ );
	this->mLocalToWorldMtx.Prepend ( shear );
	
	const USAffine3D* inherit = this->GetLinkedValue < USAffine3D* >( MOAITransformAttr::Pack ( INHERIT_TRANSFORM ), 0 );
	if ( inherit ) {
		this->mLocalToWorldMtx.Append ( *inherit );
	}
	else {
	
		inherit = this->GetLinkedValue < USAffine3D* >( MOAITransformAttr::Pack ( INHERIT_LOC ), 0 );
		if ( inherit ) {
			
			USVec3D loc = this->mLoc;
			inherit->Transform ( loc );
			
			this->mLocalToWorldMtx.m [ USAffine3D::C3_R0 ] = loc.mX;
			this->mLocalToWorldMtx.m [ USAffine3D::C3_R1 ] = loc.mY;
			this->mLocalToWorldMtx.m [ USAffine3D::C3_R2 ] = loc.mZ;
		}
	}
	
	if (( this->mPiv.mX != 0.0f ) || ( this->mPiv.mY != 0.0f ) || ( this->mPiv.mZ != 0.0f )) {
		
		USAffine3D pivot;
		pivot.Translate ( -this->mPiv.mX, -this->mPiv.mY, -this->mPiv.mZ );
		this->mLocalToWorldMtx.Prepend ( pivot );
	}
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}

//----------------------------------------------------------------//
USAffine3D MOAITransform::GetBillboardMtx ( const USAffine3D& faceCameraMtx ) const {

	USAffine3D billboardMtx = this->GetLocalToWorldMtx ();
		
	USVec3D piv;
	USVec3D worldLoc;
	
	// world space location for prop
	worldLoc.mX = billboardMtx.m [ USAffine3D::C3_R0 ];
	worldLoc.mY = billboardMtx.m [ USAffine3D::C3_R1 ];
	worldLoc.mZ = billboardMtx.m [ USAffine3D::C3_R2 ];
	
	// just the rotate/scale matrices
	billboardMtx.m [ USAffine3D::C3_R0 ] = 0.0f;
	billboardMtx.m [ USAffine3D::C3_R1 ] = 0.0f;
	billboardMtx.m [ USAffine3D::C3_R2 ] = 0.0f;
	
	// remove original pivot
	piv = this->mPiv;
	billboardMtx.Transform ( piv );
	worldLoc.Add ( piv );
	
	// orient to face the camera
	billboardMtx.Append ( faceCameraMtx );
	
	// add new pivot
	piv = this->mPiv;
	billboardMtx.Transform ( piv );
	worldLoc.Sub ( piv );
	
	// remove the original pivot
	billboardMtx.m [ USAffine3D::C3_R0 ] = worldLoc.mX;
	billboardMtx.m [ USAffine3D::C3_R1 ] = worldLoc.mY;
	billboardMtx.m [ USAffine3D::C3_R2 ] = worldLoc.mZ;
	
	return billboardMtx;
}

//----------------------------------------------------------------//
const USAffine3D& MOAITransform::GetLocalToWorldMtx () const {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine3D& MOAITransform::GetWorldToLocalMtx () const {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAITransform::MOAITransform () :
	mShearYX ( 0.0f ),
	mShearZX ( 0.0f ),
	mShearXY ( 0.0f ),
	mShearZY ( 0.0f ),
	mShearXZ ( 0.0f ),
	mShearYZ ( 0.0f ),
	mPiv ( 0.0f, 0.0f, 0.0f ),
	mLoc ( 0.0f, 0.0f, 0.0f ),
	mScale ( 1.0f, 1.0f, 1.0f ),
	mRot ( 0.0f, 0.0f, 0.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransformBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITransform::~MOAITransform () {
}

//----------------------------------------------------------------//
void MOAITransform::OnDepNodeUpdate () {
	
	this->BuildTransforms ();
}

//----------------------------------------------------------------//
void MOAITransform::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITransformBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_X_PIV",			MOAITransformAttr::Pack ( ATTR_X_PIV ));
	state.SetField ( -1, "ATTR_Y_PIV",			MOAITransformAttr::Pack ( ATTR_Y_PIV ));
	state.SetField ( -1, "ATTR_Z_PIV",			MOAITransformAttr::Pack ( ATTR_Z_PIV ));
	state.SetField ( -1, "ATTR_X_LOC",			MOAITransformAttr::Pack ( ATTR_X_LOC ));
	state.SetField ( -1, "ATTR_Y_LOC",			MOAITransformAttr::Pack ( ATTR_Y_LOC ));
	state.SetField ( -1, "ATTR_Z_LOC",			MOAITransformAttr::Pack ( ATTR_Z_LOC ));
	state.SetField ( -1, "ATTR_X_ROT",			MOAITransformAttr::Pack ( ATTR_X_ROT ));
	state.SetField ( -1, "ATTR_Y_ROT",			MOAITransformAttr::Pack ( ATTR_Y_ROT ));
	state.SetField ( -1, "ATTR_Z_ROT",			MOAITransformAttr::Pack ( ATTR_Z_ROT ));
	state.SetField ( -1, "ATTR_X_SCL",			MOAITransformAttr::Pack ( ATTR_X_SCL ));
	state.SetField ( -1, "ATTR_Y_SCL",			MOAITransformAttr::Pack ( ATTR_Y_SCL ));
	state.SetField ( -1, "ATTR_Z_SCL",			MOAITransformAttr::Pack ( ATTR_Z_SCL ));
	state.SetField ( -1, "ATTR_ROTATE_QUAT",	MOAITransformAttr::Pack ( ATTR_ROTATE_QUAT ));
	state.SetField ( -1, "ATTR_TRANSLATE",		MOAITransformAttr::Pack ( ATTR_TRANSLATE ));
	
	state.SetField ( -1, "INHERIT_LOC",			MOAITransformAttr::Pack ( INHERIT_LOC ));
	state.SetField ( -1, "INHERIT_TRANSFORM",	MOAITransformAttr::Pack ( INHERIT_TRANSFORM ));
}

//----------------------------------------------------------------//
void MOAITransform::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransformBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "addLoc",				_addLoc },
		{ "addPiv",				_addPiv },
		{ "addRot",				_addRot },
		{ "addScl",				_addScl },
		{ "getLoc",				_getLoc },
		{ "getPiv",				_getPiv },
		{ "getRot",				_getRot },
		{ "getScl",				_getScl },
		{ "modelToWorld",		_modelToWorld },
		{ "move",				_move },
		{ "moveLoc",			_moveLoc },
		{ "movePiv",			_movePiv },
		{ "moveRot",			_moveRot },
		{ "moveScl",			_moveScl },
		{ "seek",				_seek },
		{ "seekLoc",			_seekLoc },
		{ "setParent",			_setParent },
		{ "seekPiv",			_seekPiv },
		{ "seekRot",			_seekRot },
		{ "seekScl",			_seekScl },
		{ "setLoc",				_setLoc },
		{ "setPiv",				_setPiv },
		{ "setRot",				_setRot },
		{ "setScl",				_setScl },
		{ "setShearByX",		_setShearByX },
		{ "setShearByY",		_setShearByY },
		{ "setShearByZ",		_setShearByZ },
		{ "worldToModel",		_worldToModel },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITransform::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );
	
	this->mPiv.mX = state.GetField < float >( -1, "mPiv.mX", 0.0f );
	this->mPiv.mY = state.GetField < float >( -1, "mPiv.mY", 0.0f );
	
	this->mLoc.mX = state.GetField < float >( -1, "mLoc.mX", 0.0f );
	this->mLoc.mY = state.GetField < float >( -1, "mLoc.mY", 0.0f );
	
	this->mScale.mX = state.GetField < float >( -1, "mScale.mX", 1.0f );
	this->mScale.mY = state.GetField < float >( -1, "mScale.mY", 1.0f );
	
	this->mRot.mZ = state.GetField < float >( -1, "mDegrees", 0.0f );
}

//----------------------------------------------------------------//
void MOAITransform::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	state.SetField ( -1, "mPiv.mX", this->mPiv.mX );
	state.SetField ( -1, "mPiv.mY", this->mPiv.mY );
	
	state.SetField ( -1, "mLoc.mX", this->mLoc.mX );
	state.SetField ( -1, "mLoc.mY", this->mLoc.mY );
	
	state.SetField ( -1, "mScale.mX", this->mScale.mX );
	state.SetField ( -1, "mScale.mY", this->mScale.mY );
	
	state.SetField ( -1, "mDegrees", this->mRot.mZ );
}

//----------------------------------------------------------------//
void MOAITransform::SetLoc ( float x, float y, float z ) {

	this->mLoc.mX = x;
	this->mLoc.mY = y;
	this->mLoc.mZ = z;
}

//----------------------------------------------------------------//
void MOAITransform::SetPiv ( float x, float y, float z ) {

	this->mPiv.mX = x;
	this->mPiv.mY = y;
	this->mPiv.mZ = z;
}

//----------------------------------------------------------------//
void MOAITransform::SetRot ( float x, float y, float z ) {

	this->mRot.mX = x;
	this->mRot.mY = y;
	this->mRot.mZ = z;
}

//----------------------------------------------------------------//
void MOAITransform::SetScl ( float x, float y, float z ) {

	this->mScale.mX = x;
	this->mScale.mY = y;
	this->mScale.mZ = z;
}

