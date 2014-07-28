// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#ifdef DOXYGEN

//----------------------------------------------------------------//
/**	@lua	addLoc
	@text	Adds a delta to the transform's location.
	
	@in		MOAITransform2D self
	@in		number xDelta
	@in		number yDelta
	@out	nil
*/
int MOAITransform2D::_addLoc ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	addPiv
	@text	Adds a delta to the transform's pivot.
	
	@in		MOAITransform2D self
	@in		number xDelta
	@in		number yDelta
	@out	nil
*/
int MOAITransform2D::_addPiv ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	addRot
	@text	Adds a delta to the transform's rotation
	
	@in		MOAITransform2D self
	@in		number xDelta		In degrees.
	@in		number yDelta		In degrees.
	@out	nil
*/
int MOAITransform2D::_addRot ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	addScl
	@text	Adds a delta to the transform's scale
	
	@in		MOAITransform2D self
	@in		number xSclDelta
	@opt	number ySclDelta		Default value is xSclDelta.
	@out	nil
*/
int MOAITransform2D::_addScl ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	getLoc
	@text	Returns the transform's current location.
	
	@in		MOAITransform2D self
	@out	number xLoc
	@out	number yLoc
*/
int	MOAITransform2D::_getLoc ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	getPiv
	@text	Returns the transform's current pivot.
	
	@in		MOAITransform2D self
	@out	number xPiv
	@out	number yPiv
*/
int	MOAITransform2D::_getPiv ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	getRot
	@text	Returns the transform's current rotation.
	
	@in		MOAITransform2D self
	@out	number zRot			Rotation in degrees.
*/
int	MOAITransform2D::_getRot ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	getScl
	@text	Returns the transform's current scale.
	
	@in		MOAITransform2D self
	@out	number xScl
	@out	number yScl
*/
int	MOAITransform2D::_getScl ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	modelToWorld
	@text	Transform a point in model space to world space.
	
	@in		MOAITransform2D self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@out	number x
	@out	number y
*/
int MOAITransform2D::_modelToWorld ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	move
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number xDelta		Delta to be added to x.
	@in		number yDelta		Delta to be added to y.
	@in		number zRotDelta	Delta to be added to z rot (in degrees).
	@in		number xSclDelta	Delta to be added to x scale.
	@in		number ySclDelta	Delta to be added to y scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_move ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	moveLoc
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number xDelta		Delta to be added to x.
	@in		number yDelta		Delta to be added to y.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_moveLoc ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	movePiv
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number xDelta		Delta to be added to xPiv.
	@in		number yDelta		Delta to be added to yPiv.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_movePiv ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	moveRot
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number zDelta		Delta to be added to zRot (in degrees).
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_moveRot ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	moveScl
	@text	Animate the transform by applying a delta. Creates and returns
			a MOAIEaseDriver initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number xSclDelta	Delta to be added to x scale.
	@in		number ySclDelta	Delta to be added to y scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_moveScl ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	seek
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number xGoal		Desired resulting value for x.
	@in		number yGoal		Desired resulting value for y.
	@in		number zRotGoal		Desired resulting value for z rot (in degrees).
	@in		number xSclGoal		Desired resulting value for x scale.
	@in		number ySclGoal		Desired resulting value for y scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_seek ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	seekLoc
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number xGoal		Desired resulting value for x.
	@in		number yGoal		Desired resulting value for y.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_seekLoc ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	seekPiv
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number xGoal		Desired resulting value for xPiv.
	@in		number yGoal		Desired resulting value for yPiv.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_seekPiv ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	seekRot
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number zRotGoal		Desired resulting value for z rot (in degrees).
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_seekRot ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	seekScl
	@text	Animate the transform by applying a delta. Delta is computed
			given a target value. Creates and returns a MOAIEaseDriver
			initialized to apply the delta.
	
	@in		MOAITransform2D self
	@in		number xSclGoal		Desired resulting value for x scale.
	@in		number ySclGoal		Desired resulting value for y scale.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAITransform2D::_seekScl ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setLoc
	@text	Sets the transform's location.
	
	@in		MOAITransform2D self
	@opt	number x				Default value is 0.
	@opt	number y				Default value is 0.
	@out	nil
*/
int MOAITransform2D::_setLoc ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAITransform2D self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAITransform2D::_setParent ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setPiv
	@text	Sets the transform's pivot.
	
	@in		MOAITransform2D self
	@opt	number xPiv			Default value is 0.
	@opt	number yPiv			Default value is 0.
	@out	nil
*/
int MOAITransform2D::_setPiv ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setRot
	@text	Sets the transform's rotation.
	
	@in		MOAITransform2D self
	@opt	number zRot			Default value is 0.
	@out	nil
*/
int MOAITransform2D::_setRot ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setScl
	@text	Sets the transform's scale.
	
	@in		MOAITransform2D self
	@in		number xScl
	@opt	number yScl			Default value is xScl.
	@out	nil
*/
int MOAITransform2D::_setScl ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	worldToModel
	@text	Transform a point in world space to model space.
	
	@in		MOAITransform2D self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@out	number x
	@out	number y
*/
int MOAITransform2D::_worldToModel ( lua_State* L ) {
}

#endif
