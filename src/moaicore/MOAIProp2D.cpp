// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILayoutFrame.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIProp2D self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIProp2D::_getGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )
	
	if ( self->mGrid ) {
		self->mGrid->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIProp2D self
	@out	number index
*/
int MOAIProp2D::_getIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )

	lua_pushnumber ( state, self->mIndex );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	inside
	@text	Returns true if this prop is under the given world space point.
	
	@in		MOAIProp2D self
	@in		number x
	@in		number y
	@out	boolean isInside
*/
int	MOAIProp2D::_inside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "UNN" )

	USVec2D vec;
	vec.mX	= state.GetValue < float >( 2, 0.0f );
	vec.mY	= state.GetValue < float >( 3, 0.0f );

	bool result = self->Inside ( vec );
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setDeck
	@text	Sets or clears the deck to be indexed by the prop.
	
	@in		MOAIProp2D self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )

	self->mDeck = state.GetLuaObject < MOAIDeck >( 2 );

	if ( self->mDeck ) {
		self->SetMask ( self->mDeck->GetContentMask ());
	}
	else {
		self->SetMask ( 0 );
	}
	
	return 0;
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
	MOAI_LUA_SETUP ( MOAIProp2D, "UNNNN" )

	if ( state.CheckParams ( 2, "NNNN" )) {

		float x0	= state.GetValue < float >( 2, 0.0f );
		float y0	= state.GetValue < float >( 3, 0.0f );
		float x1	= state.GetValue < float >( 4, 0.0f );
		float y1	= state.GetValue < float >( 5, 0.0f );
		
		self->mFrame.Init ( x0, y0, x1, y1 );
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
	
	@in		MOAIProp2D self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2 );
	if ( !grid ) return 0;
	
	self->mGrid = grid;
	
	return 0;
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
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )

	self->mGridScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mGridScale.mY = state.GetValue < float >( 3, 1.0f );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIProp2D self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIProp2D::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )

	self->mIndex = state.GetValue < u32 >( 2, 1 );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRemapper
	@text	Set a remapper for this prop to use when drawing deck members.
	
	@in		MOAIProp2D self
	@opt	MOAIDeckRemapper remapper		Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setRemapper ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )

	MOAIDeckRemapper* remapper = state.GetLuaObject < MOAIDeckRemapper >( 2 );
	self->SetDependentMember < MOAIDeckRemapper >( self->mRemapper, remapper );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRepeat
	@text	Repeats a grid indexer along X or Y. Only used when a grid
			is attached.
	
	@in		MOAIProp2D self
	@opt	boolean repeatX		Default value is true.
	@opt	boolean repeatY		Default value is repeatX.
	@out	nil
*/
int MOAIProp2D::_setRepeat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )

	bool repeatX = state.GetValue < bool >( 2, true );
	bool repeatY = state.GetValue < bool >( 3, repeatX );

	self->mRepeat = 0;
	self->mRepeat |= repeatX ? REPEAT_X : 0;
	self->mRepeat |= repeatY ? REPEAT_Y : 0;

	return 0;
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
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )
	
	MOAIShader* shader = state.GetLuaObject < MOAIShader >( 2 );
	self->SetDependentMember < MOAIShader >( self->mShader, shader );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVTransform
	@text	Sets or clears the prop's UV transform.
	
	@in		MOAIProp2D self
	@opt	MOAITransformBase transform	Default value is nil.
	@out	nil
*/
int MOAIProp2D::_setUVTransform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )

	MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2 );
	self->SetDependentMember < MOAITransformBase >( self->mUVTransform, transform );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVisible
	@text	Sets or clears the prop's visibility.
	
	@in		MOAIProp2D self
	@opt	boolean visible		Default value is true.
	@out	nil
*/
int MOAIProp2D::_setVisible ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp2D, "U" )

	self->mVisible = state.GetValue < bool >( 2, true );

	return 0;
}

//================================================================//
// MOAIProp2D
//================================================================//

//----------------------------------------------------------------//
bool MOAIProp2D::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	if ( MOAIProp2DAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );

		if ( attrID == ATTR_INDEX ) {
			this->mIndex = attrOp.Op ( this->mIndex );
			return true;
		}
	}
	
	if ( MOAIColor::ApplyAttrOp ( attrID, attrOp )) return true;
	return MOAITransform::ApplyAttrOp ( attrID, attrOp );
}

//----------------------------------------------------------------//
bool MOAIProp2D::BindDeck () {

	if ( this->mDeck ) {
		return this->mDeck->Bind ();
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIProp2D::Draw () {

	if ( !this->mVisible ) return;
	if ( !this->BindDeck ()) return;

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	if ( this->mUVTransform ) {
		USAffine2D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		drawbuffer.SetUVTransform ( uvMtx );
	}

	this->LoadShader ();
	
	if ( this->mGrid ) {
	
		USCellCoord c0;
		USCellCoord c1;
		
		this->GetBoundsInView ( c0, c1 );
		this->mDeck->Draw ( this->GetLocalToWorldMtx (), *this->mGrid, this->mRemapper, this->mGridScale, c0, c1 );
	}
	else {
		this->mDeck->Draw ( this->GetLocalToWorldMtx (), this->mIndex, this->mRemapper );
	}
	
	// TODO
	//MOAILayoutFrame* parentFrame = USCast < MOAILayoutFrame >( this->mParent );
	//if ( parentFrame ) {
	//	drawbuffer.SetScissorRect ();
	//}
}

//----------------------------------------------------------------//
void MOAIProp2D::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	if ( this->mDeck ) {
		if ( debugLines.Bind ( MOAIDebugLines::PROP_MODEL_BOUNDS )) {
			
			debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
			debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
			
			USRect bounds = this->mDeck->GetBounds ( this->mIndex, this->mRemapper );
			debugLines.DrawRect ( bounds );
			
			if ( this->mGrid ) {
		
				debugLines.SetPenColor ( 0x40ffffff );
				debugLines.SetPenWidth ( 2 );
		
				USCellCoord c0;
				USCellCoord c1;
				
				this->GetBoundsInView ( c0, c1 );
				this->mDeck->DrawDebug ( this->GetLocalToWorldMtx (), *this->mGrid, this->mRemapper, this->mGridScale, c0, c1 );
			}
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::PROP_WORLD_BOUNDS )) {
		debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
		debugLines.DrawRect ( this->GetBounds ());
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
					debugLines.SetPenSpace ( MOAIDebugLines::PARTITION_CELLS );
					debugLines.DrawRect ( cellRect );
				}
			}
			
			if ( paddedRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( MOAIDebugLines::PARTITION_PADDED_CELLS )) {
					debugLines.SetPenSpace ( MOAIDebugLines::PARTITION_PADDED_CELLS );
					debugLines.DrawRect ( paddedRect );
				}
			}
		}
	}
}
//----------------------------------------------------------------//
void MOAIProp2D::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {

	if ( !this->mDeck ) return;
		
	sampler.SetSourcePrim ( this );
	
	if ( this->mGrid ) {
		
		USRect localRect = sampler.GetLocalRect ();
		
		USCellCoord c0;
		USCellCoord c1;
		
		this->GetBoundsInRect ( localRect, c0, c1 );
		this->mDeck->GatherSurfaces ( *this->mGrid, this->mRemapper, this->mGridScale, c0, c1, sampler );
	}
	else {
		this->mDeck->GatherSurfaces ( this->mIndex, this->mRemapper, sampler );
	}
}

//----------------------------------------------------------------//
void MOAIProp2D::GetBoundsInRect ( const USRect& rect, USCellCoord& c0, USCellCoord& c1 ) {

	if ( this->mGrid ) {

		c0 = this->mGrid->GetCellCoord ( rect.mXMin, rect.mYMin );
		c1 = this->mGrid->GetCellCoord ( rect.mXMax, rect.mYMax );
		
		if ( !( this->mRepeat & REPEAT_X )) {
			c0 = this->mGrid->ClampX ( c0 );
			c1 = this->mGrid->ClampX ( c1 );
		}
		
		if ( !( this->mRepeat & REPEAT_Y )) {
			c0 = this->mGrid->ClampY ( c0 );
			c1 = this->mGrid->ClampY ( c1 );
		}
	}
	else {
		
		c0.Init ( 0, 0 );
		c1.Init ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIProp2D::GetBoundsInView ( USCellCoord& c0, USCellCoord& c1 ) {

	const USAffine2D& invWorldMtx = this->GetWorldToLocalMtx ();

	// view quad in world space
	USViewQuad viewQuad;
	viewQuad.Init ();
	viewQuad.Transform ( invWorldMtx );
	
	USRect viewRect = viewQuad.mBounds;
	viewRect.Bless ();
	
	this->GetBoundsInRect ( viewRect, c0, c1 );
}

//----------------------------------------------------------------//
USColorVec MOAIProp2D::GetColorTrait () {

	return this->mColor;
}

//----------------------------------------------------------------//
USRect* MOAIProp2D::GetFrameTrait () {

	return &this->mFrame;
}

//----------------------------------------------------------------//
MOAIShader* MOAIProp2D::GetShaderTrait () {

	return this->mShader;
}

//----------------------------------------------------------------//
u32 MOAIProp2D::GetLocalFrame ( USRect& frame ) {
	
	if ( this->mGrid ) {
	
		frame = this->mGrid->GetBounds ();
		return this->mRepeat ? BOUNDS_GLOBAL : BOUNDS_OK;
	}
	else if ( this->mDeck ) {
	
		frame = this->mDeck->GetBounds ( this->mIndex, this->mRemapper );
		return BOUNDS_OK;
	}
	
	return BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
bool MOAIProp2D::GetVisibleTrait () {

	return this->mVisible;
}

//----------------------------------------------------------------//
bool MOAIProp2D::Inside ( USVec2D vec ) {

	const USAffine2D& worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );

	if ( this->mFitToFrame ) {
		return this->mFrame.Contains ( vec ); 
	}
	
	USRect rect;
	u32 status = this->GetLocalFrame ( rect );
	
	if ( status == BOUNDS_GLOBAL ) return true;
	if ( status == BOUNDS_EMPTY ) return false;
	
	return rect.Contains ( vec );
}

//----------------------------------------------------------------//
MOAIOverlapPrim2D* MOAIProp2D::IsOverlapPrim2D () {
	return 0;
}

//----------------------------------------------------------------//
void MOAIProp2D::LoadShader () {

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	if ( this->mShader ) {
		this->mShader->Bind ();
		
		USColorVec color = drawbuffer.GetPenColor ();
		color.Modulate ( this->mColor );
		drawbuffer.SetPenColor ( color );
	}
	else {
		drawbuffer.SetPenColor ( this->mColor );
		drawbuffer.SetBlendMode ( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	}
	
	// TODO
	//MOAILayoutFrame* parent = USCast < MOAILayoutFrame >( this->mParent );
	//if ( parent ) {
	//	USRect scissorRect = parent->GetScissorRect ();			
	//	drawbuffer.SetScissorRect ( scissorRect );
	//}
	//else {
		drawbuffer.SetScissorRect ();
	//}
}

//----------------------------------------------------------------//
MOAIProp2D::MOAIProp2D () :
	mIndex( 1 ),
	mRepeat ( 0 ),
	mGridScale ( 1.0f, 1.0f ),
	mFitToFrame ( false ),
	mVisible ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
		RTTI_EXTEND ( MOAIColor )
	RTTI_END
	
	this->mColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIProp2D::~MOAIProp2D () {
}

//----------------------------------------------------------------//
void MOAIProp2D::OnDepNodeUpdate () {
	
	this->mColor = *this;
	
	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	u32 frameStatus = this->GetLocalFrame ( rect );
	
	if ( rect.Area () == 0.0f ) {
		frameStatus = BOUNDS_EMPTY;
	}
	
	USVec2D offset ( 0.0f, 0.0f );
	USVec2D stretch ( 1.0f, 1.0f );
	
	// select the frame
	USRect frame = this->mFrame;
	
	if ( this->mTraitSource ) {
	
		if ( this->mTraitMask & INHERIT_COLOR ) {
			this->mColor.Modulate ( this->mTraitSource->GetColorTrait ());
		}
		
		if ( this->mTraitMask & INHERIT_FRAME ) {
			
			USRect* frameTrait = this->mTraitSource->GetFrameTrait ();
			if ( frameTrait ) {
				frame = *frameTrait;
			}
		}
		
		if ( this->mTraitMask & INHERIT_PARTITION ) {
			MOAIPartition* partition = this->mTraitSource->GetPartitionTrait ();
			this->SetPartition ( partition );
		}
		
		if ( this->mTraitMask & INHERIT_SHADER ) {
			this->mShader = this->mTraitSource->GetShaderTrait ();
		}
		
		if ( this->mTraitMask & INHERIT_VISIBLE ) {
			this->mVisible = this->mTraitSource->GetVisibleTrait ();
		}
	}
	
	if (( this->mTraitMask & INHERIT_FRAME ) || this->mFitToFrame ) {

		// and check if the target frame is empty, too
		if ( frame.Area () == 0.0f ) {
			frameStatus = BOUNDS_EMPTY;
		}

		// compute the scale and offset (if any)
		if ( frameStatus != BOUNDS_EMPTY ) {
			
			stretch.mX = frame.Width () / rect.Width ();
			stretch.mY = frame.Height () / rect.Height ();
			
			offset.mX = frame.mXMin - ( rect.mXMin * stretch.mX );
			offset.mY = frame.mYMin - ( rect.mYMin * stretch.mY );
		}
	}
	
	// inherit parent and offset transforms (and compute the inverse)
	this->BuildTransforms ( offset.mX, offset.mY, stretch.mX, stretch.mY );
	
	// update the prop location in the partition
	// use the local frame; world transform will match it to target frame
	switch ( frameStatus ) {
		case BOUNDS_EMPTY:
		case BOUNDS_GLOBAL: {
			this->UpdateBounds ( frameStatus );
			break;
		}
		case BOUNDS_OK: {
			this->mLocalToWorldMtx.Transform ( rect );
			this->UpdateBounds ( rect, frameStatus );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIProp2D::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIProp::RegisterLuaClass ( state );
	MOAIColor::RegisterLuaClass ( state );
	
	state.SetField ( -1, "INHERIT_COLOR", ( u32 )INHERIT_COLOR );
	state.SetField ( -1, "INHERIT_FRAME", ( u32 )INHERIT_FRAME );
	state.SetField ( -1, "INHERIT_PARTITION", ( u32 )INHERIT_PARTITION );
	
	state.SetField ( -1, "ATTR_INDEX", MOAIProp2DAttr::Pack ( ATTR_INDEX ));
}

//----------------------------------------------------------------//
void MOAIProp2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );
	MOAIColor::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getGrid",			_getGrid },
		{ "getIndex",			_getIndex },
		{ "inside",				_inside },
		{ "setDeck",			_setDeck },
		{ "setFrame",			_setFrame },
		{ "setGrid",			_setGrid },
		{ "setGridScale",		_setGridScale },
		{ "setIndex",			_setIndex },
		{ "setRemapper",		_setRemapper },
		{ "setRepeat",			_setRepeat },
		{ "setShader",			_setShader },
		{ "setUVTransform",		_setUVTransform },
		{ "setVisible",			_setVisible },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIProp2D::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	
	this->mDeck = serializer.GetRefField < MOAIDeck >( state, -1, "mDeck" );
	this->mGrid = serializer.GetRefField < MOAIGrid >( state, -1, "mGrid" );
}

//----------------------------------------------------------------//
void MOAIProp2D::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	
	serializer.SetRefField ( state, -1, "mDeck", this->mDeck );
	serializer.SetRefField ( state, -1, "mGrid", this->mGrid );
}

//----------------------------------------------------------------//
STLString MOAIProp2D::ToString () {

	return "";
}

