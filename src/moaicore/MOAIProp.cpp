// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICollisionShape.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILayoutFrame.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionResultBuffer.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getBounds
	@text	Return the prop's local bounds or 'nil' if prop bounds is
			global or missing. The bounds are in model space and will
			be overidden by the prop's frame if it's been set (using
			setFrame ())
	
	@in		MOAIProp self
	@out	number xMin
	@out	number yMin
	@out	number zMin
	@out	number xMax
	@out	number yMax
	@out	number zMax
*/
int MOAIProp::_getBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	USBox bounds;

	u32 status = self->GetPropBounds ( bounds );
	if ( status != BOUNDS_OK ) return 0;

	state.Push ( bounds.mMin.mX );
	state.Push ( bounds.mMin.mY );
	state.Push ( bounds.mMin.mZ );
	
	state.Push ( bounds.mMax.mX );
	state.Push ( bounds.mMax.mY );
	state.Push ( bounds.mMax.mZ );

	return 6;
}

//----------------------------------------------------------------//
/**	@name	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIProp self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIProp::_getGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	if ( self->mGrid ) {
		self->mGrid->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIProp self
	@out	number index
*/
int MOAIProp::_getIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	lua_pushnumber ( state, self->mIndex );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPriority
	@text	Returns the current priority of the node or 'nil' if the
			priority is uninitialized.
	
	@in		MOAIProp self
	@out	number priority		The node's priority or nil.
*/
int MOAIProp::_getPriority ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	if ( self->mPriority != UNKNOWN_PRIORITY ) {
		lua_pushnumber ( state, self->mPriority );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	inside
	@text	Returns true if the given world space point falls inside
			the prop's bounds.
	
	@in		MOAIProp self
	@in		number x
	@in		number y
	@in		number z
	@opt	number pad			Pad the hit bounds (in the prop's local space)
	@out	boolean isInside
*/
int	MOAIProp::_inside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	USVec3D vec;
	vec.mX	= state.GetValue < float >( 2, 0.0f );
	vec.mY	= state.GetValue < float >( 3, 0.0f );
	vec.mZ	= state.GetValue < float >( 4, 0.0f );

	float pad = state.GetValue < float >( 5, 0.0f );

	bool result = self->Inside ( vec, pad );
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/** @name	setBlendMode
	@text	Set the blend mode.

	@overload	Reset the blend mode to MOAIProp.BLEND_NORMAL (equivalent to src = GL_ONE, dst = GL_ONE_MINUS_SRC_ALPHA)

		@in		MOAIProp self
		@out	nil

	@overload	Set blend mode using one of the Moai presets.

		@in		MOAIProp self
		@in		number mode					One of MOAIProp.BLEND_NORMAL, MOAIProp.BLEND_ADD, MOAIProp.BLEND_MULTIPLY.
		@out	nil
	
	@overload	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIProp.
				See the OpenGL documentation for an explanation of blending constants.

		@in		MOAIProp self
		@in		number srcFactor
		@in		number dstFactor
		@out	nil
*/
int MOAIProp::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		if ( state.IsType ( 3, LUA_TNUMBER )) {
		
			u32 srcFactor = state.GetValue < u32 >( 2, 0 );
			u32 dstFactor = state.GetValue < u32 >( 3, 0 );
			self->mBlendMode.SetBlend ( srcFactor, dstFactor );
		}
		else {
			
			u32 blendMode = state.GetValue < u32 >( 2, MOAIBlendMode::BLEND_NORMAL );
			self->mBlendMode.SetBlend ( blendMode );
		}
	}
	else {
		self->mBlendMode.SetBlend ( MOAIBlendMode::BLEND_NORMAL );
	}
	
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCullMode
	@text	Sets and enables face culling.
	
	@in		MOAIProp self
	@opt	number cullMode			Default value is MOAIProp.CULL_NONE.
	@out	nil
*/
int MOAIProp::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	self->mCullMode = state.GetValue < int >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDeck
	@text	Sets or clears the deck to be indexed by the prop.
	
	@in		MOAIProp self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIProp::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mDeck.Set ( *self, state.GetLuaObject < MOAIDeck >( 2 ));

	if ( self->mDeck ) {
		self->SetMask ( self->mDeck->GetContentMask ());
	}
	else {
		self->SetMask ( 0 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDepthMask
	@text	Disables or enables depth writing.
	
	@in		MOAIProp self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIProp::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	self->mDepthMask = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
	
	@in		MOAIProp self
	@opt	number depthFunc		Default value is MOAIProp.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIProp::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	self->mDepthTest = state.GetValue < int >( 2, 0 );
	
	return 0;
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
	
	@in		MOAIProp self
	@in		boolean expandForSort	Default value is false.
	@out	nil
*/
int MOAIProp::_setExpandForSort ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mExpandForSort = state.GetValue < bool >( 2, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFrame
	@text	Sets the fitting frame of the prop.
	
	@overload	Clear the fitting frame.
	
		@in		MOAIProp self
		@out	nil
	
	@overload	Set the fitting frame.
	
		@in		MOAIProp self
		@in		number xMin
		@in		number yMin
		@in		number zMin
		@in		number xMax
		@in		number yMax
		@in		number zMax
		@out	nil
*/
int MOAIProp::_setFrame ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	if ( state.CheckParams ( 2, "NNNNNN" )) {

		self->mFrame = state.GetBox ( 2 );
		self->mFitToFrame = true;
	}
	else {
		self->mFitToFrame = false;
	}
	
	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGrid
	@text	Sets or clears the prop's grid indexer. The grid indexer (if any)
			will override the standard indexer.
	
	@in		MOAIProp self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIProp::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2 );
	if ( !grid ) return 0;
	
	self->mGrid.Set ( *self, grid );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGridScale
	@text	Scale applied to deck items before rendering to grid cell.
	
	@in		MOAIProp self
	@opt	number xScale		Default value is 1.
	@opt	number yScale		Default value is 1.
	@out	nil
*/
int MOAIProp::_setGridScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mGridScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mGridScale.mY = state.GetValue < float >( 3, 1.0f );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIProp self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIProp::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mIndex = state.GetValue < u32 >( 2, 1 );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAIProp self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIProp::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2 );
	
	self->SetAttrLink ( PACK_ATTR ( MOAIColor, INHERIT_COLOR ), parent, PACK_ATTR ( MOAIColor, COLOR_TRAIT ));
	self->SetAttrLink ( PACK_ATTR ( MOAITransform, INHERIT_TRANSFORM ), parent, PACK_ATTR ( MOAITransformBase, TRANSFORM_TRAIT ));
	self->SetAttrLink ( PACK_ATTR ( MOAIProp, ATTR_VISIBLE ), parent, PACK_ATTR ( MOAIProp, ATTR_VISIBLE ));
	
	//MOAILog ( state, MOAILogMessages::MOAI_FunctionDeprecated_S, "setParent" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPriority
	@text	Sets or clears the node's priority. Clear the priority
			to have MOAIPartition automatically assign a priority
			to a node when it is added.
	
	@in		MOAIProp self
	@opt	number priority		Default value is nil.
	@out	nil
*/
int MOAIProp::_setPriority ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
		self->mPriority = ( s32 )state.GetValue < int >( 2, 0 );
	}
	else {
		self->mPriority = UNKNOWN_PRIORITY;
		if ( self->mPartition ) {
			self->mPartition->AffirmPriority ( *self );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRemapper
	@text	Set a remapper for this prop to use when drawing deck members.
	
	@in		MOAIProp self
	@opt	MOAIDeckRemapper remapper		Default value is nil.
	@out	nil
*/
int MOAIProp::_setRemapper ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	MOAIDeckRemapper* remapper = state.GetLuaObject < MOAIDeckRemapper >( 2 );
	self->SetDependentMember < MOAIDeckRemapper >( self->mRemapper, remapper );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setShader
	@text	Sets or clears the prop's shader. The prop's shader takes
			precedence over any shader specified by the deck or its
			elements.
	
	@in		MOAIProp self
	@opt	MOAIShader shader	Default value is nil.
	@out	nil
*/
int MOAIProp::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	MOAIShader* shader = state.GetLuaObject < MOAIShader >( 2 );
	self->SetDependentMember < MOAIShader >( self->mShader, shader );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Set or load a texture for this prop. The prop's texture will
			override the deck's texture.
	
	@in		MOAIProp self
	@in		variant texture		A MOAITexture, MOAIMultiTexture, MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITextureBase.QUANTIZE, MOAITextureBase.TRUECOLOR, MOAITextureBase.PREMULTIPLY_ALPHA
	@out	MOAIGfxState texture
*/
int MOAIProp::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	MOAIGfxState* texture = MOAITexture::AffirmTexture ( state, 2 );
	self->mTexture.Set ( *self, texture );

	if ( texture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVTransform
	@text	Sets or clears the prop's UV transform.
	
	@in		MOAIProp self
	@opt	MOAITransformBase transform	Default value is nil.
	@out	nil
*/
int MOAIProp::_setUVTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2 );
	self->SetDependentMember < MOAITransformBase >( self->mUVTransform, transform );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVisible
	@text	Sets or clears the prop's visibility.
	
	@in		MOAIProp self
	@opt	boolean visible		Default value is true.
	@out	nil
*/
int MOAIProp::_setVisible ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mVisible = state.GetValue < bool >( 2, true );

	return 0;
}

//================================================================//
// MOAIProp
//================================================================//

//----------------------------------------------------------------//
bool MOAIProp::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIPropAttr::Check ( attrID )) {
		
		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_INDEX:
				this->mIndex = USFloat::ToIndex ( attrOp.Apply (( float )this->mIndex, op, MOAINode::ATTR_READ_WRITE ));
				return true;
			case ATTR_PARTITION:
				this->SetPartition ( attrOp.Apply < MOAIPartition >( this->GetPartition (), op, MOAINode::ATTR_READ_WRITE ));
				return true;
			case ATTR_SHADER:
				this->mShader.Set ( *this, attrOp.Apply < MOAIShader >( this->mShader, op, MOAINode::ATTR_READ_WRITE ));
				return true;
			case ATTR_BLEND_MODE:
				attrOp.Apply < MOAIBlendMode >( this->mBlendMode, op, MOAINode::ATTR_READ_WRITE );
				return true;
			case ATTR_VISIBLE:
				this->mVisible = USFloat::ToBoolean ( attrOp.Apply ( USFloat::FromBoolean ( this->mVisible ), op, MOAINode::ATTR_READ_WRITE ));
				return true;
			case FRAME_TRAIT:
				attrOp.Apply < USBox >( &this->mFrame, op, MOAINode::ATTR_READ );
				return true;
		}
	}
	
	if ( MOAIColor::ApplyAttrOp ( attrID, attrOp, op )) return true;
	return MOAITransform::ApplyAttrOp ( attrID, attrOp, op );
}

//----------------------------------------------------------------//
void MOAIProp::Draw ( int subPrimID, bool reload ) {
	UNUSED ( subPrimID );

	if ( !this->mVisible ) return;
	if ( !this->mDeck ) return;

	this->LoadGfxState ();
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	if ( this->mUVTransform ) {
		USAffine3D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		gfxDevice.SetUVTransform ( uvMtx );
	}
	else {
		gfxDevice.SetUVTransform ();
	}
	
	if ( this->mGrid ) {
	
		if ( subPrimID == MOAIProp::NO_SUBPRIM_ID ) {
	
			MOAICellCoord c0;
			MOAICellCoord c1;
			
			this->GetGridBoundsInView ( c0, c1 );
			this->mDeck->Draw ( this->GetLocalToWorldMtx (), *this->mGrid, this->mRemapper, this->mGridScale, c0, c1 );
		}
		else {
			this->mDeck->Draw ( this->GetLocalToWorldMtx (), reload, *this->mGrid, this->mRemapper, this->mGridScale, subPrimID );
		}
	}
	else {
		this->mDeck->Draw ( this->GetLocalToWorldMtx (), this->mIndex, this->mRemapper );
	}
	
	// TODO
	//MOAILayoutFrame* parentFrame = MOAICast < MOAILayoutFrame >( this->mParent );
	//if ( parentFrame ) {
	//	gfxDevice.SetScissorRect ();
	//}
}

//----------------------------------------------------------------//
void MOAIProp::DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_MODEL_BOUNDS )) {
		
		debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
		debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
		
		USBox bounds;
		u32 status = this->GetDeckBounds ( bounds );
		if ( status == BOUNDS_OK ) {
			debugLines.DrawRect ( bounds.GetRect ( USBox::PLANE_XY ));
		}
		
		if ( this->mDeck && this->mGrid ) {
	
			debugLines.SetPenColor ( 0x40ffffff );
			debugLines.SetPenWidth ( 2 );
	
			MOAICellCoord c0;
			MOAICellCoord c1;
			
			this->GetGridBoundsInView ( c0, c1 );
			this->mDeck->DrawDebug ( this->GetLocalToWorldMtx (), *this->mGrid, this->mRemapper, this->mGridScale, c0, c1 );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_WORLD_BOUNDS )) {
		debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
		debugLines.DrawRect ( this->GetBounds ().GetRect ( USBox::PLANE_XY ));
	}
	
	debugLines.SetPenColor ( 0x40ffffff );
	debugLines.SetPenWidth ( 2 );
	
	if ( this->mDeck ) {
		this->mDeck->DrawDebug ( this->GetLocalToWorldMtx (), this->mIndex, this->mRemapper );
	}
	
	if ( debugLines.IsVisible ( MOAIDebugLines::PARTITION_CELLS ) || debugLines.IsVisible ( MOAIDebugLines::PARTITION_PADDED_CELLS )) {
		
		debugLines.SetWorldMtx ();
		
		USRect cellRect;
		USRect paddedRect;
		
		if ( this->GetCellRect ( &cellRect, &paddedRect )) {
			
			if ( cellRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( MOAIDebugLines::PARTITION_CELLS )) {
					debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
					debugLines.DrawRect ( cellRect );
				}
			}
			
			if ( paddedRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( MOAIDebugLines::PARTITION_PADDED_CELLS )) {
					debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
					debugLines.DrawRect ( paddedRect );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIProp::ExpandForSort ( MOAIPartitionResultBuffer& buffer ) {

	if ( this->mExpandForSort && this->mGrid ) {
		
		// add a sub-prim for each visible grid cell
		const USAffine3D& mtx = this->GetLocalToWorldMtx ();
		
		MOAIGrid* grid = this->mGrid;
		
		MOAICellCoord c0;
		MOAICellCoord c1;
		
		this->GetGridBoundsInView ( c0, c1 );
		float zLoc = this->GetWorldZLoc ();

		for ( int y = c0.mY; y <= c1.mY; ++y ) {
			for ( int x = c0.mX; x <= c1.mX; ++x ) {
				
				u32 idx = grid->GetTile ( x, y );
				if ( !idx || ( idx & MOAITileFlags::HIDDEN )) continue;
				
				MOAICellCoord coord ( x, y );
				int subPrimID = grid->GetCellAddr ( coord );
				
				USVec2D loc = grid->GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
				mtx.Transform ( loc );
				
				buffer.PushResult ( *this, subPrimID, this->GetPriority (), loc.mX, loc.mY, zLoc );
			}
		}
	}
	else {
		buffer.PushResult ( *this, NO_SUBPRIM_ID, this->mPriority, this->GetWorldXLoc (), this->GetWorldYLoc (), this->GetWorldZLoc ());
	}
}

//----------------------------------------------------------------//
void MOAIProp::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {

	if ( !this->mDeck ) return;
		
	sampler.SetSourcePrim ( this );
	
	if ( this->mGrid ) {
		
		USRect localRect = sampler.GetLocalRect ();
		
		MOAICellCoord c0;
		MOAICellCoord c1;
		
		USRect deckBounds = this->mDeck->GetBounds ().GetRect( USBox::PLANE_XY );

		this->mGrid->GetBoundsInRect ( localRect, c0, c1, deckBounds );
		this->mDeck->GatherSurfaces ( *this->mGrid, this->mRemapper, this->mGridScale, c0, c1, sampler );
	}
	else {
		this->mDeck->GatherSurfaces ( this->mIndex, this->mRemapper, sampler );
	}
}

//----------------------------------------------------------------//
bool MOAIProp::GetCellRect ( USRect* cellRect, USRect* paddedRect ) {

	if ( !( cellRect || paddedRect )) return false;
	
	if ( this->mLayer ) {
	
		USVec3D center;
		this->mBounds.GetCenter ( center );
		
		MOAICellCoord coord = this->mLayer->mGridSpace.GetCellCoord ( center.mX, center.mY );
		USRect rect = this->mLayer->mGridSpace.GetCellRect ( coord );
		
		if ( cellRect ) {
			*cellRect = rect;
		}
		
		if ( paddedRect ) {
			rect.Inflate ( this->mLayer->mCellSize * 0.5f );
			*paddedRect = rect;
		}
		return true;
	}
	
	if ( cellRect ) {
		cellRect->Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	if ( paddedRect ) {
		paddedRect->Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	return false;
}

//----------------------------------------------------------------//
void MOAIProp::GetCollisionShape ( MOAICollisionShape& shape ) {
	UNUSED ( shape );
}

//----------------------------------------------------------------//
u32 MOAIProp::GetDeckBounds ( USBox& bounds ) {
	
	u32 status = BOUNDS_EMPTY;
	
	if ( this->mGrid ) {
		
		USRect rect = this->mGrid->GetBounds ();

		bounds.Init ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax, 0.0f, 0.0f );
		status = this->mGrid->GetRepeat () ? BOUNDS_GLOBAL : BOUNDS_OK;
	}
	else if ( this->mDeck ) {
	
		bounds = this->mDeck->GetBounds ( this->mIndex, this->mRemapper );
		status = BOUNDS_OK;
	}
	
	if ( status == BOUNDS_EMPTY ) {
		status = this->GetFrame ( bounds );
	}
	return status;
}

//----------------------------------------------------------------//
u32 MOAIProp::GetFrame ( USBox& bounds ) {

	const USBox* frameTrait = this->GetLinkedValue < USBox >( MOAIPropAttr::Pack ( INHERIT_FRAME ));
	if ( frameTrait ) {
		bounds = *frameTrait;
		return BOUNDS_OK;
	}
	
	if ( this->mFitToFrame ) {
		bounds = this->mFrame;
		return BOUNDS_OK;
	}
	
	return BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAIProp::GetGridBoundsInView ( MOAICellCoord& c0, MOAICellCoord& c1 ) {

	const USFrustum& frustum = MOAIGfxDevice::Get ().GetViewVolume ();
	
	USRect viewRect;
	if ( frustum.GetXYSectRect ( this->GetWorldToLocalMtx (), viewRect )) {
	
		// TODO: need to take into account perspective and truncate rect based on horizon
		// TODO: consider bringing back poly to tile scanline converter...

		USRect deckBounds = this->mDeck->GetBounds ().GetRect ( USBox::PLANE_XY );

		this->mGrid->GetBoundsInRect ( viewRect, c0, c1, deckBounds );
	}
}

//----------------------------------------------------------------//
u32 MOAIProp::GetPropBounds ( USBox& bounds ) {
	
	if ( this->mGrid && this->mGrid->GetRepeat ()) {
		return BOUNDS_GLOBAL;
	}

	u32 status = this->GetFrame ( bounds );
	if ( status == BOUNDS_EMPTY ) {
		return this->GetDeckBounds ( bounds );
	}
	return status;
}

//----------------------------------------------------------------//
MOAIPartition* MOAIProp::GetPartitionTrait () {

	return this->mPartition;
}

//----------------------------------------------------------------//
bool MOAIProp::Inside ( USVec3D vec, float pad ) {

	const USAffine3D& worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );

	USBox bounds;

	u32 status = this->GetDeckBounds ( bounds );
	
	if ( status == BOUNDS_GLOBAL ) return true;
	if ( status == BOUNDS_EMPTY ) return false;
	
	bounds.Bless ();
	bounds.Inflate ( pad );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIProp::LoadGfxState () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	MOAIGfxState* shader = this->mShader;
	MOAIGfxState* texture = this->mTexture;

	if ( this->mDeck ) {
		shader = shader ? shader : this->mDeck->GetShader ();
		texture = texture ? texture : this->mDeck->GetTexture ();
	}

	gfxDevice.SetGfxState ( shader );
	gfxDevice.SetGfxState ( texture );

	gfxDevice.SetPenColor ( this->mColor );
	gfxDevice.SetCullFunc ( this->mCullMode );
	gfxDevice.SetDepthFunc ( this->mDepthTest );
	gfxDevice.SetDepthMask ( this->mDepthMask );
	gfxDevice.SetBlendMode ( this->mBlendMode );
	
	// TODO
	//MOAILayoutFrame* parent = MOAICast < MOAILayoutFrame >( this->mParent );
	//if ( parent ) {
	//	USRect scissorRect = parent->GetScissorRect ();			
	//	gfxDevice.SetScissorRect ( scissorRect );
	//}
	//else {
		gfxDevice.SetScissorRect ();
	//}
}

//----------------------------------------------------------------//
MOAIProp::MOAIProp () :
	mPartition ( 0 ),
	mCell ( 0 ),
	mLayer ( 0 ),
	mNextResult ( 0 ),
	mMask ( 0xffffffff ),
	mPriority ( UNKNOWN_PRIORITY ),
	mIndex( 1 ),
	mGridScale ( 1.0f, 1.0f ),
	mFitToFrame ( false ),
	mCullMode ( 0 ),
	mDepthTest ( 0 ),
	mDepthMask ( true ),
	mVisible ( true ),
	mExpandForSort ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
		RTTI_EXTEND ( MOAIColor )
		RTTI_EXTEND ( MOAIRenderable )
	RTTI_END
	
	this->mLinkInCell.Data ( this );
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIProp::~MOAIProp () {

	if ( this->mCell ) {
		this->mCell->RemoveProp ( *this );
	}
	
	this->mDeck.Set ( *this, 0 );
	this->mRemapper.Set ( *this, 0 );
	this->mGrid.Set ( *this, 0 );
	this->mShader.Set ( *this, 0 );
	this->mTexture.Set ( *this, 0 );
	this->mUVTransform.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIProp::OnDepNodeUpdate () {
	
	MOAIColor::OnDepNodeUpdate ();
	
	USBox deckBounds;
	u32 deckBoundsStatus = this->GetDeckBounds ( deckBounds );
	
	USBox propBounds;
	u32 propBoundsStatus = this->GetPropBounds ( propBounds );
	
	USVec3D offset ( 0.0f, 0.0f, 0.0f );
	USVec3D stretch ( 1.0f, 1.0f, 1.0f );
	
	if (( deckBoundsStatus == BOUNDS_OK ) && ( propBoundsStatus == BOUNDS_OK )) {
		deckBounds.GetFitting ( propBounds, offset, stretch );
	}
	
	// inherit parent and offset transforms (and compute the inverse)
	this->BuildTransforms ( offset.mX, offset.mY, offset.mZ, stretch.mX, stretch.mY, stretch.mZ );
	
	// update the prop location in the partition
	// use the local frame; world transform will match it to target frame
	switch ( propBoundsStatus ) {
		case BOUNDS_EMPTY:
		case BOUNDS_GLOBAL: {
			this->UpdateBounds ( propBoundsStatus );
			break;
		}
		case BOUNDS_OK: {
			deckBounds.Transform ( this->mLocalToWorldMtx );
			this->UpdateBounds ( deckBounds, propBoundsStatus );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaClass ( state );
	MOAIColor::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_INDEX", MOAIPropAttr::Pack ( ATTR_INDEX ));
	state.SetField ( -1, "ATTR_PARTITION", MOAIPropAttr::Pack ( ATTR_PARTITION ));
	state.SetField ( -1, "ATTR_SHADER", MOAIPropAttr::Pack ( ATTR_SHADER ));
	state.SetField ( -1, "ATTR_BLEND_MODE", MOAIPropAttr::Pack ( ATTR_BLEND_MODE ));
	state.SetField ( -1, "ATTR_VISIBLE", MOAIPropAttr::Pack ( ATTR_VISIBLE ));
	
	state.SetField ( -1, "INHERIT_FRAME", MOAIPropAttr::Pack ( INHERIT_FRAME ));
	state.SetField ( -1, "FRAME_TRAIT", MOAIPropAttr::Pack ( FRAME_TRAIT ));
	
	state.SetField ( -1, "BLEND_ADD", ( u32 )MOAIBlendMode::BLEND_ADD );
	state.SetField ( -1, "BLEND_MULTIPLY", ( u32 )MOAIBlendMode::BLEND_MULTIPLY );
	state.SetField ( -1, "BLEND_NORMAL", ( u32 )MOAIBlendMode::BLEND_NORMAL );
	
	state.SetField ( -1, "GL_ONE", ( u32 )GL_ONE );
	state.SetField ( -1, "GL_ZERO", ( u32 )GL_ZERO );
	state.SetField ( -1, "GL_DST_ALPHA", ( u32 )GL_DST_ALPHA );
	state.SetField ( -1, "GL_DST_COLOR", ( u32 )GL_DST_COLOR );
	state.SetField ( -1, "GL_SRC_COLOR", ( u32 )GL_SRC_COLOR );
	state.SetField ( -1, "GL_ONE_MINUS_DST_ALPHA", ( u32 )GL_ONE_MINUS_DST_ALPHA );
	state.SetField ( -1, "GL_ONE_MINUS_DST_COLOR", ( u32 )GL_ONE_MINUS_DST_COLOR );
	state.SetField ( -1, "GL_ONE_MINUS_SRC_ALPHA", ( u32 )GL_ONE_MINUS_SRC_ALPHA );
	state.SetField ( -1, "GL_ONE_MINUS_SRC_COLOR", ( u32 )GL_ONE_MINUS_SRC_COLOR );
	state.SetField ( -1, "GL_SRC_ALPHA", ( u32 )GL_SRC_ALPHA );
	state.SetField ( -1, "GL_SRC_ALPHA_SATURATE", ( u32 )GL_SRC_ALPHA_SATURATE );
	
	state.SetField ( -1, "DEPTH_TEST_DISABLE", ( u32 )0 );
	state.SetField ( -1, "DEPTH_TEST_NEVER", ( u32 )GL_NEVER );
	state.SetField ( -1, "DEPTH_TEST_LESS", ( u32 )GL_LESS );
	state.SetField ( -1, "DEPTH_TEST_EQUAL", ( u32 )GL_EQUAL );
	state.SetField ( -1, "DEPTH_TEST_LESS_EQUAL", ( u32 )GL_LEQUAL );
	state.SetField ( -1, "DEPTH_TEST_GREATER", ( u32 )GL_GREATER );
	state.SetField ( -1, "DEPTH_TEST_NOTEQUAL", ( u32 )GL_NOTEQUAL );
	state.SetField ( -1, "DEPTH_TEST_GREATER_EQUAL", ( u32 )GL_GEQUAL );
	state.SetField ( -1, "DEPTH_TEST_ALWAYS", ( u32 )GL_ALWAYS );
	
	state.SetField ( -1, "CULL_NONE", ( u32 )0 );
	state.SetField ( -1, "CULL_ALL", ( u32 )GL_FRONT_AND_BACK );
	state.SetField ( -1, "CULL_BACK", ( u32 )GL_BACK );
	state.SetField ( -1, "CULL_FRONT", ( u32 )GL_FRONT );
}

//----------------------------------------------------------------//
void MOAIProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );
	MOAIColor::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getBounds",			_getBounds },
		{ "getGrid",			_getGrid },
		{ "getIndex",			_getIndex },
		{ "getPriority",		_getPriority },
		{ "inside",				_inside },
		{ "setBlendMode",		_setBlendMode },
		{ "setCullMode",		_setCullMode },
		{ "setDeck",			_setDeck },
		{ "setDepthMask",		_setDepthMask },
		{ "setDepthTest",		_setDepthTest },
		{ "setExpandForSort",	_setExpandForSort },
		{ "setFrame",			_setFrame },
		{ "setGrid",			_setGrid },
		{ "setGridScale",		_setGridScale },
		{ "setIndex",			_setIndex },
		{ "setParent",			_setParent },
		{ "setPriority",		_setPriority },
		{ "setRemapper",		_setRemapper },
		{ "setShader",			_setShader },
		{ "setTexture",			_setTexture },
		{ "setUVTransform",		_setUVTransform },
		{ "setVisible",			_setVisible },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIProp::Render () {

	this->Draw ( MOAIProp::NO_SUBPRIM_ID, true );
}

//----------------------------------------------------------------//
void MOAIProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	this->mDeck.Set ( *this, serializer.MemberIDToObject < MOAIDeck >( state.GetField < uintptr >( -1, "mDeck", 0 )));
	this->mGrid.Set ( *this, serializer.MemberIDToObject < MOAIGrid >( state.GetField < uintptr >( -1, "mGrid", 0 )));
}

//----------------------------------------------------------------//
void MOAIProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	state.SetField ( -1, "mDeck", serializer.AffirmMemberID ( this->mDeck ));
	state.SetField ( -1, "mGrid", serializer.AffirmMemberID ( this->mGrid ));
}

//----------------------------------------------------------------//
void MOAIProp::SetPartition ( MOAIPartition* partition ) {

	if ( partition != this->mPartition ) {
		if ( partition ) {
			partition->InsertProp ( *this );
		}
		else if ( this->mPartition ) {
			this->mPartition->RemoveProp ( *this );
		}
	}
}

//----------------------------------------------------------------//
void MOAIProp::UpdateBounds ( u32 status ) {

	USBox bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );

	if ( status == BOUNDS_OK ) {
		status = BOUNDS_EMPTY;
	}
	this->UpdateBounds ( bounds, status );
}

//----------------------------------------------------------------//
void MOAIProp::UpdateBounds ( const USBox& bounds, u32 status ) {

	this->mBounds = bounds;
	this->mBounds.Bless ();

	if ( this->mPartition ) {
		this->mPartition->UpdateProp ( *this, status );
	}
}
