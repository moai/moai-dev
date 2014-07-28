// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#ifdef DOXYGEN

//----------------------------------------------------------------//
/**	@lua	getBounds
	@text	Return the prop's local bounds or 'nil' if prop bounds is
			global or missing. The bounds are in model space and will
			be overridden by the prop's frame if it's been set (using
			setFrame ())
	
	@in		MOAIGraphicsProp2D self
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAIGraphicsProp2D::_getBounds ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIGraphicsProp2D self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIGraphicsProp2D::_getGrid ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIGraphicsProp2D self
	@out	number index
*/
int MOAIGraphicsProp2D::_getIndex ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	getPriority
	@text	Returns the current priority of the node or 'nil' if the
			priority is uninitialized.
	
	@in		MOAIGraphicsProp2D self
	@out	number priority		The node's priority or nil.
*/
int MOAIGraphicsProp2D::_getPriority ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	inside
	@text	Returns true if the given world space point falls inside
			the prop's bounds.
	
	@in		MOAIGraphicsProp2D self
	@in		number x
	@in		number y
	@in		number z
	@opt	number pad			Pad the hit bounds (in the prop's local space)
	@out	boolean isInside
*/
int	MOAIGraphicsProp2D::_inside ( lua_State* L ) {
}

//----------------------------------------------------------------//
/** @lua	setBlendMode
	@text	Set the blend mode.

	@overload	Reset the blend mode to MOAIGraphicsProp2D.BLEND_NORMAL (equivalent to src = GL_ONE, dst = GL_ONE_MINUS_SRC_ALPHA)

		@in		MOAIGraphicsProp2D self
		@out	nil

	@overload	Set blend mode using one of the Moai presets.

		@in		MOAIGraphicsProp2D self
		@in		number mode					One of MOAIGraphicsProp2D.BLEND_NORMAL, MOAIGraphicsProp2D.BLEND_ADD, MOAIGraphicsProp2D.BLEND_MULTIPLY.
		@out	nil
	
	@overload	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIGraphicsProp2D.
				See the OpenGL documentation for an explanation of blending constants.

		@in		MOAIGraphicsProp2D self
		@in		number srcFactor
		@in		number dstFactor
		@out	nil
*/
int MOAIGraphicsProp2D::_setBlendMode ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setCullMode
	@text	Sets and enables face culling.
	
	@in		MOAIGraphicsProp2D self
	@opt	number cullMode			Default value is MOAIGraphicsProp2D.CULL_NONE.
	@out	nil
*/
int MOAIGraphicsProp2D::_setCullMode ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setDeck
	@text	Sets or clears the deck to be indexed by the prop.
	
	@in		MOAIGraphicsProp2D self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp2D::_setDeck ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setDepthMask
	@text	Disables or enables depth writing.
	
	@in		MOAIGraphicsProp2D self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIGraphicsProp2D::_setDepthMask ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
	
	@in		MOAIGraphicsProp2D self
	@opt	number depthFunc		Default value is MOAIGraphicsProp2D.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIGraphicsProp2D::_setDepthTest ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setExpandForSort
	@text	Used when drawing with a layout scheme (i.e. MOAIGrid).
			Expanding for sort causes the prop to emit a sub-prim
			for each component of the layout. For example, when
			attaching a MOAIGrid to a prop, each cell of the grid
			will be added to the render queue for sorting against
			all other props and sub-prims. This is obviously less
			efficient, but still more efficient then using an
			separate prop for each cell or object.
	
	@in		MOAIGraphicsProp2D self
	@in		boolean expandForSort	Default value is false.
	@out	nil
*/
int MOAIGraphicsProp2D::_setExpandForSort ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setFrame
	@text	Sets the fitting frame of the prop.
	
	@overload	Clear the fitting frame.
	
		@in		MOAIGraphicsProp2D self
		@out	nil
	
	@overload	Set the fitting frame.
	
		@in		MOAIGraphicsProp2D self
		@in		number xMin
		@in		number yMin
		@in		number xMax
		@in		number yMax
		@out	nil
*/
int MOAIGraphicsProp2D::_setFrame ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setGrid
	@text	Sets or clears the prop's grid indexer. The grid indexer (if any)
			will override the standard indexer.
	
	@in		MOAIGraphicsProp2D self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp2D::_setGrid ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setGridScale
	@text	Scale applied to deck items before rendering to grid cell.
	
	@in		MOAIGraphicsProp2D self
	@opt	number xScale		Default value is 1.
	@opt	number yScale		Default value is 1.
	@out	nil
*/
int MOAIGraphicsProp2D::_setGridScale ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIGraphicsProp2D self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIGraphicsProp2D::_setIndex ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAIGraphicsProp2D self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp2D::_setParent ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setPriority
	@text	Sets or clears the node's priority. Clear the priority
			to have MOAIPartition automatically assign a priority
			to a node when it is added.
	
	@in		MOAIGraphicsProp2D self
	@opt	number priority		Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp2D::_setPriority ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setRemapper
	@text	Set a remapper for this prop to use when drawing deck members.
	
	@in		MOAIGraphicsProp2D self
	@opt	MOAIDeckRemapper remapper		Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp2D::_setRemapper ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setShader
	@text	Sets or clears the prop's shader. The prop's shader takes
			precedence over any shader specified by the deck or its
			elements.
	
	@in		MOAIGraphicsProp2D self
	@opt	MOAIShader shader	Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp2D::_setShader ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setTexture
	@text	Set or load a texture for this prop. The prop's texture will
			override the deck's texture.
	
	@in		MOAIGraphicsProp2D self
	@in		variant texture		A MOAITexture, MOAIMultiTexture, MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITextureBase.QUANTIZE, MOAITextureBase.TRUECOLOR, MOAITextureBase.PREMULTIPLY_ALPHA
	@out	MOAIGfxState texture
*/
int MOAIGraphicsProp2D::_setTexture ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setUVTransform
	@text	Sets or clears the prop's UV transform.
	
	@in		MOAIGraphicsProp2D self
	@opt	MOAITransformBase transform	Default value is nil.
	@out	nil
*/
int MOAIGraphicsProp2D::_setUVTransform ( lua_State* L ) {
}

//----------------------------------------------------------------//
/**	@lua	setVisible
	@text	Sets or clears the prop's visibility.
	
	@in		MOAIGraphicsProp2D self
	@opt	boolean visible		Default value is true.
	@out	nil
*/
int MOAIGraphicsProp2D::_setVisible ( lua_State* L ) {
}

#endif
