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
	@out	nil
*/
int MOAITransform::_addLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNN" )
	
	USVec2D loc = self->GetLoc ();
	
	loc.mX += state.GetValue < float >( 2, 0.0f );
	loc.mY += state.GetValue < float >( 3, 0.0f );
	
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
	@out	nil
*/
int MOAITransform::_addPiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNN" )
	
	USVec2D piv = self->GetLoc ();
	
	piv.mX += state.GetValue < float >( 2, 0.0f );
	piv.mY += state.GetValue < float >( 3, 0.0f );
	
	self->SetPiv ( piv );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addRot
	@text	Adds a delta to the transform's rotation
	
	@in		MOAITransform self
	@in		number rDelta		In degrees.
	@out	nil
*/
int MOAITransform::_addRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UN" )
	
	float d = self->GetRot ();
	d += state.GetValue < float >( 2, 0.0f );
	
	self->SetRot ( d );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addScl
	@text	Adds a delta to the transform's scale
	
	@in		MOAITransform self
	@in		number xSclDelta
	@opt	number ySclDelta		Default value is xSclDelta.
	@out	nil
*/
int MOAITransform::_addScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UN" )
	
	USVec2D scl = self->GetScl ();
	
	float xSclDelta = state.GetValue < float >( 2, 0.0f );
	float ySclDelta = state.GetValue < float >( 3, xSclDelta );
	
	scl.mX += xSclDelta;
	scl.mY += ySclDelta;
	
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
*/
int	MOAITransform::_getLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	lua_pushnumber ( state, self->mLoc.mX );
	lua_pushnumber ( state, self->mLoc.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@name	getPiv
	@text	Returns the transform's current pivot.
	
	@in		MOAITransform self
	@out	number xPiv
	@out	number yPiv
*/
int	MOAITransform::_getPiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	lua_pushnumber ( state, self->mPiv.mX );
	lua_pushnumber ( state, self->mPiv.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@name	getRot
	@text	Returns the transform's current rotation.
	
	@in		MOAITransform self
	@out	number rot			Rotation in degrees.
*/
int	MOAITransform::_getRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	lua_pushnumber ( state, self->mDegrees );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getScl
	@text	Returns the transform's current scale.
	
	@in		MOAITransform self
	@out	number xScl
	@out	number yScl
*/
int	MOAITransform::_getScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "U" )
	
	lua_pushnumber ( state, self->mScale.mX );
	lua_pushnumber ( state, self->mScale.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@name	modelToWorld
	@text	Transform a point in model space to world space.
	
	@in		MOAITransform self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
*/
int MOAITransform::_modelToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );

	USAffine2D modelToWorld = self->GetLocalToWorldMtx ();
	modelToWorld.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@name	move
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number xDelta		Delta to be added to x.
	@in		number yDelta		Delta to be added to y.
	@in		number rDelta		Delta to be added to r (in degrees).
	@in		number xSclDelta	Delta to be added to x scale.
	@in		number ySclDelta	Delta to be added to y scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_move ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNNNNNN" )

	float xLoc		= state.GetValue < float >( 2, 0.0f );
	float yLoc		= state.GetValue < float >( 3, 0.0f );
	float zRot		= state.GetValue < float >( 4, 0.0f );
	float xScl		= state.GetValue < float >( 5, 0.0f );
	float yScl		= state.GetValue < float >( 6, 0.0f );
	float delay		= state.GetValue < float >( 7, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 8, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 5 );
		
		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_X_LOC ), xLoc, mode );
		action->SetLink ( 1, self, MOAITransformAttr::Pack ( ATTR_Y_LOC ), yLoc, mode );
		action->SetLink ( 2, self, MOAITransformAttr::Pack ( ATTR_Z_ROT ), zRot, mode );
		action->SetLink ( 3, self, MOAITransformAttr::Pack ( ATTR_X_SCL ), xScl, mode );
		action->SetLink ( 4, self, MOAITransformAttr::Pack ( ATTR_Y_SCL ), yScl, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );
		
		return 1;
	}

	self->mLoc.mX += xLoc;
	self->mLoc.mY += yLoc;
	self->mDegrees += zRot;
	self->mScale.mX += xScl;
	self->mScale.mY += yScl;
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
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_moveLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNNN" )
	
	float xLoc		= state.GetValue < float >( 2, 0.0f );
	float yLoc		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 5, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 2 );
		
		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_X_LOC ), xLoc, mode );
		action->SetLink ( 1, self, MOAITransformAttr::Pack ( ATTR_Y_LOC ), yLoc, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mLoc.mX += xLoc;
	self->mLoc.mY += yLoc;
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
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_movePiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNNN" )
	
	float xPiv		= state.GetValue < float >( 2, 0.0f );
	float yPiv		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 5, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 2 );
		
		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_X_PIV ), xPiv, mode );
		action->SetLink ( 1, self, MOAITransformAttr::Pack ( ATTR_Y_PIV ), yPiv, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mPiv.mX += xPiv;
	self->mPiv.mY += yPiv;
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	moveRot
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number rDelta		Delta to be added to r (in degrees).
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_moveRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNN" )
	
	float zRot		= state.GetValue < float >( 2, 0.0f );
	float delay		= state.GetValue < float >( 3, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 4, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );
		
		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_Z_ROT ), zRot, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mDegrees += zRot;
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
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_moveScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNNN" )

	float xScl		= state.GetValue < float >( 2, 0.0f );
	float yScl		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );

	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 5, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 2 );
		
		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_X_SCL ), xScl, mode );
		action->SetLink ( 1, self, MOAITransformAttr::Pack ( ATTR_Y_SCL ), yScl, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mScale.mX += xScl;
	self->mScale.mY += yScl;
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
	@in		number rGoal		Desired resulting value for r (in degrees).
	@in		number xSclGoal		Desired resulting value for x scale.
	@in		number ySclGoal		Desired resulting value for y scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seek ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNNNNNN" )
	
	float xLoc		= state.GetValue < float >( 2, 0.0f );
	float yLoc		= state.GetValue < float >( 3, 0.0f );
	float zRot		= state.GetValue < float >( 4, 0.0f );
	float xScl		= state.GetValue < float >( 5, 0.0f );
	float yScl		= state.GetValue < float >( 6, 0.0f );
	float delay		= state.GetValue < float >( 7, 0.0f );
	
	if ( delay > 0.0f ) {
		
		u32 mode = state.GetValue < u32 >( 8, USInterpolate::kSmooth );

		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 5 );

		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_X_LOC ), xLoc - self->mLoc.mX, mode );
		action->SetLink ( 1, self, MOAITransformAttr::Pack ( ATTR_Y_LOC ), yLoc - self->mLoc.mY, mode );
		action->SetLink ( 2, self, MOAITransformAttr::Pack ( ATTR_Z_ROT ), zRot - self->mDegrees, mode );
		action->SetLink ( 3, self, MOAITransformAttr::Pack ( ATTR_X_SCL ), xScl - self->mScale.mX, mode );
		action->SetLink ( 4, self, MOAITransformAttr::Pack ( ATTR_Y_SCL ), yScl - self->mScale.mY, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mLoc.mX = xLoc;
	self->mLoc.mY = yLoc;
	self->mDegrees = zRot;
	self->mScale.mX = xScl;
	self->mScale.mY = yScl;
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
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seekLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNNN" )

	float xLoc		= state.GetValue < float >( 2, 0.0f );
	float yLoc		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	
	if ( delay > 0.0f ) {

		u32 mode = state.GetValue < u32 >( 5, USInterpolate::kSmooth );		
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 2 );

		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_X_LOC ), xLoc - self->mLoc.mX, mode );
		action->SetLink ( 1, self, MOAITransformAttr::Pack ( ATTR_Y_LOC ), yLoc - self->mLoc.mY, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mLoc.mX = xLoc;
	self->mLoc.mY = yLoc;
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
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seekPiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNNN" )

	float xPiv		= state.GetValue < float >( 2, 0.0f );
	float yPiv		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	
	if ( delay > 0.0f ) {

		u32 mode = state.GetValue < u32 >( 5, USInterpolate::kSmooth );		
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 2 );

		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_X_PIV ), xPiv - self->mPiv.mX, mode );
		action->SetLink ( 1, self, MOAITransformAttr::Pack ( ATTR_Y_PIV ), yPiv - self->mPiv.mY, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mPiv.mX = xPiv;
	self->mPiv.mY = yPiv;
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekRot
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform self
	@in		number rGoal		Desired resulting value for r (in degrees).
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seekRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNN" )

	float zRot		= state.GetValue < float >( 2, 0.0f );
	float delay		= state.GetValue < float >( 3, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 4, USInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );
		
		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_Z_ROT ), zRot - self->mDegrees, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mDegrees = zRot;
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
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform::_seekScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNNN" )

	float xScl		= state.GetValue < float >( 2, 0.0f );
	float yScl		= state.GetValue < float >( 3, 0.0f );
	float delay		= state.GetValue < float >( 4, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 5, USInterpolate::kSmooth );

		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 2 );

		action->SetLink ( 0, self, MOAITransformAttr::Pack ( ATTR_X_SCL ), xScl - self->mScale.mX, mode );
		action->SetLink ( 1, self, MOAITransformAttr::Pack ( ATTR_Y_SCL ), yScl - self->mScale.mY, mode );
		
		action->SetLength ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mScale.mX = xScl;
	self->mScale.mY = yScl;
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLoc
	@text	Sets the transform's location.
	
	@in		MOAITransform self
	@in		number x
	@in		number y
	@out	nil
*/
int MOAITransform::_setLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNN" )
	
	USVec2D loc;
	
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	
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

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2 );
	
	self->SetAttrLink ( PACK_ATTR ( MOAITransform, INHERIT_TRANSFORM ), parent, PACK_ATTR ( MOAITransformBase, TRANSFORM_TRAIT ));
	
	//MOAILog ( state, MOAILogMessages::MOAI_FunctionDeprecated_S, "setParent" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPiv
	@text	Sets the transform's pivot.
	
	@in		MOAITransform self
	@in		number xPiv
	@in		number yPiv
	@out	nil
*/
int MOAITransform::_setPiv ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNN" )
	
	USVec2D piv;
	
	piv.mX = state.GetValue < float >( 2, 0.0f );
	piv.mY = state.GetValue < float >( 3, 0.0f );
	
	self->SetPiv ( piv );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRot
	@text	Sets the transform's rotation.
	
	@in		MOAITransform self
	@in		number rot			Rotation in degrees.
	@out	nil
*/
int MOAITransform::_setRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UN" )
	
	float d = state.GetValue < float >( 2, 0.0f );
	
	self->SetRot ( d );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setScl
	@text	Sets the transform's scale.
	
	@in		MOAITransform self
	@in		number xScl
	@opt	number yScl				Default value is xScl.
	@out	nil
*/
int MOAITransform::_setScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UN" )
	
	USVec2D scl;
	
	scl.mX = state.GetValue < float >( 2, 0.0f );
	scl.mY = state.GetValue < float >( 3, scl.mX );
	
	self->SetScl ( scl );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	worldToModel
	@text	Transform a point in world space to model space.
	
	@in		MOAITransform self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
*/
int MOAITransform::_worldToModel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransform, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );

	USAffine2D worldToModel = self->GetWorldToLocalMtx ();
	worldToModel.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
}

//================================================================//
// MOAITransform
//================================================================//

//----------------------------------------------------------------//
bool MOAITransform::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAITransformAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_X_PIV:
				this->mPiv.mX = attrOp.Apply ( this->mPiv.mX, op, MOAINode::ATTR_READ_WRITE );
				return true;
			case ATTR_Y_PIV:
				this->mPiv.mY = attrOp.Apply ( this->mPiv.mY, op, MOAINode::ATTR_READ_WRITE );
				return true;
			case ATTR_X_LOC:
				this->mLoc.mX = attrOp.Apply ( this->mLoc.mX, op, MOAINode::ATTR_READ_WRITE );
				return true;
			case ATTR_Y_LOC:
				this->mLoc.mY = attrOp.Apply ( this->mLoc.mY, op, MOAINode::ATTR_READ_WRITE );
				return true;
			case ATTR_Z_ROT:
				this->mDegrees = attrOp.Apply ( this->mDegrees, op, MOAINode::ATTR_READ_WRITE );
				return true;
			case ATTR_X_SCL:
				this->mScale.mX = attrOp.Apply ( this->mScale.mX, op, MOAINode::ATTR_READ_WRITE );
				return true;
			case ATTR_Y_SCL:
				this->mScale.mY = attrOp.Apply ( this->mScale.mY, op, MOAINode::ATTR_READ_WRITE );
				return true;
		}
	}
	return MOAITransformBase::ApplyAttrOp ( attrID, attrOp, op );
}

//----------------------------------------------------------------//
void MOAITransform::BuildTransforms ( float xOff, float yOff, float xStretch, float yStretch ) {
	
	this->mLocalToWorldMtx.ScRoTr (
		this->mScale.mX * xStretch,
		this->mScale.mY * yStretch,
		( float )D2R * this->mDegrees,
		this->mLoc.mX + xOff,
		this->mLoc.mY + yOff
	);
	
	const USAffine2D* inherit = this->GetLinkedValue < USAffine2D >( MOAITransformAttr::Pack ( INHERIT_TRANSFORM ));
	if ( inherit ) {
		this->mLocalToWorldMtx.Append ( *inherit );
	}
	else {
	
		inherit = this->GetLinkedValue < USAffine2D >( MOAITransformAttr::Pack ( INHERIT_LOC ));
		if ( inherit ) {
			
			USVec2D loc = this->mLoc;
			inherit->Transform ( loc );
			
			this->mLocalToWorldMtx.m [ USAffine2D::C2_R0 ] = loc.mX;
			this->mLocalToWorldMtx.m [ USAffine2D::C2_R1 ] = loc.mY;
		}
	}
	
	if (( this->mPiv.mX != 0.0f ) || ( this->mPiv.mY != 0.0f )) {
		
		USAffine2D pivot;
		pivot.Translate ( -this->mPiv.mX, -this->mPiv.mY );
		this->mLocalToWorldMtx.Prepend ( pivot );
	}
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}

//----------------------------------------------------------------//
const USAffine2D& MOAITransform::GetLocalToWorldMtx () {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine2D& MOAITransform::GetWorldToLocalMtx () {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAITransform::MOAITransform () :
	mPiv ( 0.0f, 0.0f ),
	mLoc ( 0.0f, 0.0f ),
	mScale ( 1.0f, 1.0f ),
	mDegrees ( 0.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransformBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITransform::~MOAITransform () {
}

//----------------------------------------------------------------//
void MOAITransform::OnDepNodeUpdate () {
	
	this->BuildTransforms ( 0.0f, 0.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
void MOAITransform::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITransformBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_X_PIV",			MOAITransformAttr::Pack ( ATTR_X_PIV ));
	state.SetField ( -1, "ATTR_Y_PIV",			MOAITransformAttr::Pack ( ATTR_Y_PIV ));
	state.SetField ( -1, "ATTR_X_LOC",			MOAITransformAttr::Pack ( ATTR_X_LOC ));
	state.SetField ( -1, "ATTR_Y_LOC",			MOAITransformAttr::Pack ( ATTR_Y_LOC ));
	state.SetField ( -1, "ATTR_Z_ROT",			MOAITransformAttr::Pack ( ATTR_Z_ROT ));
	state.SetField ( -1, "ATTR_X_SCL",			MOAITransformAttr::Pack ( ATTR_X_SCL ));
	state.SetField ( -1, "ATTR_Y_SCL",			MOAITransformAttr::Pack ( ATTR_Y_SCL ));
	
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
	
	this->mDegrees = state.GetField < float >( -1, "mDegrees", 0.0f );
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
	
	state.SetField ( -1, "mDegrees", this->mDegrees );
}

//----------------------------------------------------------------//
void MOAITransform::SetLoc ( float x, float y ) {

	this->mLoc.mX = x;
	this->mLoc.mY = y;
}

//----------------------------------------------------------------//
void MOAITransform::SetPiv ( float x, float y ) {

	this->mPiv.mX = x;
	this->mPiv.mY = y;
}

//----------------------------------------------------------------//
void MOAITransform::SetScl ( float x, float y ) {

	this->mScale.mX = x;
	this->mScale.mY = y;
}

