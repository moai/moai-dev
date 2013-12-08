// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#ifdef DOXYGEN

//----------------------------------------------------------------//
/**	@name	clear
	@text	Remove all props from the layer's partition.
	
	@in		MOAILayer2D self
	@out	nil
*/
int MOAILayer2D::_clear ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	getFitting
	@text	Computes a camera fitting for a given world rect along with
			an optional screen space padding. To do a fitting, compute
			the world rect based on whatever you are fitting to, use
			this method to get the fitting, then animate the camera
			to match.
	
	@in		MOAILayer2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number xPad
	@opt	number yPad
	@out	number x		X center of fitting (use for camera location).
	@out	number y		Y center of fitting (use for camera location).
	@out	number s		Scale of fitting (use for camera scale).
*/
int MOAILayer2D::_getFitting ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	getPartition
	@text	Returns the partition (if any) currently attached to this layer.
	
	@in		MOAILayer2D self
	@out	MOAIPartition partition
*/
int	MOAILayer2D::_getPartition ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	getSortMode
	@text	Get the sort mode for rendering.
	
	@in		MOAILayer2D self
	@out	number sortMode
*/
int MOAILayer2D::_getSortMode ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	getSortScale
	@text	Return the scalar applied to axis sorts.
	
	@in		MOAILayer2D self
	@out	number x
	@out	number y
	@out	number priority
*/
int	MOAILayer2D::_getSortScale ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	insertProp
	@text	Adds a prop to the layer's partition.
	
	@in		MOAILayer2D self
	@in		MOAIProp prop
	@out	nil
*/
int	MOAILayer2D::_insertProp ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	removeProp
	@text	Removes a prop from the layer's partition.
	
	@in		MOAILayer2D self
	@in		MOAIProp prop
	@out	nil
*/
int	MOAILayer2D::_removeProp ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setBox2DWorld
	@text	Sets a Box2D world for debug drawing.
	
	@in		MOAILayer2D self
	@in		MOAIBox2DWorld world
	@out	nil
*/
int MOAILayer2D::_setBox2DWorld ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setCamera
	@text	Sets a camera for the layer. If no camera is supplied,
			layer will render using the identity matrix as view/proj.
	
	@overload
	
		@in		MOAILayer2D self
		@opt	MOAICamera2D camera		Default value is nil.
		@out	nil
	
	@overload
	
		@in		MOAILayer2D self
		@opt	MOAICamera camera		Default value is nil.
		@out	nil
*/
int MOAILayer2D::_setCamera ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setCpSpace
	@text	Sets a Chipmunk space for debug drawing.
	
	@in		MOAILayer2D self
	@in		MOAICpSpace space
	@out	nil
*/
int MOAILayer2D::_setCpSpace ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setFrameBuffer
	@text	Attach a frame buffer. Layer will render to frame buffer
			instead of the main view.
	
	@in		MOAILayer2D self
	@in		MOAIFrameBufferTexture frameBuffer
	@out	nil
*/
int MOAILayer2D::_setFrameBuffer ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setParallax
	@text	Sets the parallax scale for this layer. This is simply a
			scalar applied to the view transform before rendering.
	
	@in		MOAILayer2D self
	@in		number xParallax	Default value is 1.
	@in		number yParallax	Default value is 1.
	@out	nil
*/
int MOAILayer2D::_setParallax ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setPartition
	@text	Sets a partition for the layer to use. The layer will automatically
			create a partition when the first prop is added if no partition
			has been set.
	
	@in		MOAILayer2D self
	@in		MOAIPartition partition
	@out	nil
*/
int MOAILayer2D::_setPartition ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setSortMode
	@text	Set the sort mode for rendering.
	
	@in		MOAILayer2D self
	@in		number sortMode		One of MOAILayer2D.SORT_NONE, MOAILayer2D.SORT_PRIORITY_ASCENDING,
								MOAILayer2D.SORT_PRIORITY_DESCENDING, MOAILayer2D.SORT_X_ASCENDING,
								MOAILayer2D.SORT_X_DESCENDING, MOAILayer2D.SORT_Y_ASCENDING,
								MOAILayer2D.SORT_Y_DESCENDING
	@out	nil
*/
int MOAILayer2D::_setSortMode ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setSortScale
	@text	Set the scalar applied to axis sorts.
	
	@in		MOAILayer2D self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number priority		Default value is 1.
	@out	nil
*/
int	MOAILayer2D::_setSortScale ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setViewport
	@text	Set the layer's viewport.
	
	@in		MOAILayer2D self
	@in		MOAIViewport viewport
	@out	nil
*/
int MOAILayer2D::_setViewport ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	showDebugLines
	@text	Display debug lines for props in this layer.
	
	@in		MOAILayer2D self
	@opt	bool showDebugLines		Default value is 'true'.
	@out	nil
*/
int	MOAILayer2D::_showDebugLines ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	wndToWorld
	@text	Project a point from window space into world space.
	
	@in		MOAILayer2D self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
*/
int MOAILayer2D::_wndToWorld ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	worldToWnd
	@text	Transform a point from world space to window space.
	
	@in		MOAILayer2D self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
*/
int MOAILayer2D::_worldToWnd ( lua_State* L ) {
}

#endif
