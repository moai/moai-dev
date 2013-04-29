// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#ifdef DOXYGEN

//----------------------------------------------------------------//
/**	@name	getRect
	@text	Return the prop's local bounds or 'nil' if prop bounds is
			global or missing. The bounds are in model space and will
			be overidden by the prop's frame if it's been set (using
			setFrame ())
	
	@in		MOAIProp2D self
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAIProp2D::_getBounds ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIProp2D self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIProp2D::_getGrid ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIProp2D self
	@out	number index
*/
int MOAIProp2D::_getIndex ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	getPriority
	@text	Returns the current priority of the node or 'nil' if the
			priority is uninitialized.
	
	@in		MOAIProp2D self
	@out	number priority		The node's priority or nil.
*/
int MOAIProp2D::_getPriority ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	inside
	@text	Returns true if the given world space point falls inside
			the prop's bounds.
	
	@in		MOAIProp2D self
	@in		number x
	@in		number y
	@in		number z
	@opt	number pad			Pad the hit bounds (in the prop's local space)
	@out	boolean isInside
*/
int	MOAIProp2D::_inside ( lua_State* L ) {
}

//----------------------------------------------------------------//
/** @name	setBlendMode
	@text	Set the blend mode.

	@overload	Reset the blend mode to MOAIProp2D.BLEND_NORMAL (equivalent to src = GL_ONE, dst = GL_ONE_MINUS_SRC_ALPHA)

		@in		MOAIProp2D self
		@out	nil

	@overload	Set blend mode using one of the Moai presets.

		@in		MOAIProp2D self
		@in		number mode					One of MOAIProp2D.BLEND_NORMAL, MOAIProp2D.BLEND_ADD, MOAIProp2D.BLEND_MULTIPLY.
		@out	nil
	
	@overload	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIProp2D.
				See the OpenGL documentation for an explanation of blending constants.

		@in		MOAIProp2D self
		@in		number srcFactor
		@in		number dstFactor
		@out	nil
*/
int MOAIProp2D::_setBlendMode ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setCullMode
	@text	Sets and enables face culling.
	
	@in		MOAIProp2D self
	@opt	number cullMode			Default value is MOAIProp2D.CULL_NONE.
	@out	nil
*/
int MOAIProp2D::_setCullMode ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setDeck
	@text	Sets or clears the deck to be indexed by the prop.
	
	@in		MOAIProp2D self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setDeck ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setDepthMask
	@text	Disables or enables depth writing.
	
	@in		MOAIProp2D self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIProp2D::_setDepthMask ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
	
	@in		MOAIProp2D self
	@opt	number depthFunc		Default value is MOAIProp2D.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIProp2D::_setDepthTest ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setExpandForSort
	@text	Used when drawing with a layout scheme (i.e. MOAIGrid).
			Expanding for sort causes the prop to emit a sub-prim
			for each component of the layout. For example, when
			attaching a MOAIGrid to a prop, each cell of the grid
			will be added to the render queue for sorting against
			all other props and sub-prims. This is obviously less
			efficient, but still more efficient then using an
			separate prop for each cell or object.
	
	@in		MOAIProp2D self
	@in		boolean expandForSort	Default value is false.
	@out	nil
*/
int MOAIProp2D::_setExpandForSort ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setFrame
	@text	Sets the fitting frame of the prop.
	
	@overload	Clear the fitting frame.
	
		@in		MOAIProp2D self
		@out	nil
	
	@overload	Set the fitting frame.
	
		@in		MOAIProp2D self
		@in		number xMin
		@in		number yMin
		@in		number xMax
		@in		number yMax
		@out	nil
*/
int MOAIProp2D::_setFrame ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setGrid
	@text	Sets or clears the prop's grid indexer. The grid indexer (if any)
			will override the standard indexer.
	
	@in		MOAIProp2D self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setGrid ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setGridScale
	@text	Scale applied to deck items before rendering to grid cell.
	
	@in		MOAIProp2D self
	@opt	number xScale		Default value is 1.
	@opt	number yScale		Default value is 1.
	@out	nil
*/
int MOAIProp2D::_setGridScale ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIProp2D self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIProp2D::_setIndex ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAIProp2D self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setParent ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setPriority
	@text	Sets or clears the node's priority. Clear the priority
			to have MOAIPartition automatically assign a priority
			to a node when it is added.
	
	@in		MOAIProp2D self
	@opt	number priority		Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setPriority ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setRemapper
	@text	Set a remapper for this prop to use when drawing deck members.
	
	@in		MOAIProp2D self
	@opt	MOAIDeckRemapper remapper		Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setRemapper ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setShader
	@text	Sets or clears the prop's shader. The prop's shader takes
			precedence over any shader specified by the deck or its
			elements.
	
	@in		MOAIProp2D self
	@opt	MOAIShader shader	Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setShader ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Set or load a texture for this prop. The prop's texture will
			override the deck's texture.
	
	@in		MOAIProp2D self
	@in		variant texture		A MOAITexture, MOAIMultiTexture, MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITextureBase.QUANTIZE, MOAITextureBase.TRUECOLOR, MOAITextureBase.PREMULTIPLY_ALPHA
	@out	MOAIGfxState texture
*/
int MOAIProp2D::_setTexture ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setUVTransform
	@text	Sets or clears the prop's UV transform.
	
	@in		MOAIProp2D self
	@opt	MOAITransformBase transform	Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setUVTransform ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@name	setVisible
	@text	Sets or clears the prop's visibility.
	
	@in		MOAIProp2D self
	@opt	boolean visible		Default value is true.
	@out	nil
*/
int MOAIProp2D::_setVisible ( lua_State* L ) {
}

#endif
