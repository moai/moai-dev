// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILayoutFrame.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( grid ) getGrid ( self )</tt>\n
\n
	Returns the grid object associated with this tilemap.
	@param self (in)
	@param grid (out)
*/
int MOAIProp2D::_getGrid ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UU" )
	
	if ( self->mGrid ) {
		self->mGrid->PushLuaInstance ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( addr ) getContentAddr ( self )</tt>\n
\n
	Gets the current content address.
	@param self (in)
	@param addr (out)
*/
int MOAIProp2D::_getIndex ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "U" )

	lua_pushnumber ( state, self->mIndex );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>inside (mX, mY)</tt>\n
	\n
	Returns true if a point is inside of this sprite.
	@param mX The X coordinate of the point to be checked.
	@param mY The Y coordinate of the point to be checked.
	@return True if the point is inside, otherwise false.
*/
int	MOAIProp2D::_inside ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UNN" )

	USVec2D vec;
	vec.mX	= state.GetValue < float >( 2, 0.0f );
	vec.mY	= state.GetValue < float >( 3, 0.0f );

	bool result = self->Inside ( vec );
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>setGfxSource ( self, lib )</tt>\n
	\n
	Associates a library with the prop.
	@param self (in)
	@param lib (in) Library to be used.
*/
int MOAIProp2D::_setDeck ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UU" )

	self->mDeck = state.GetLuaData < MOAIDeck >( 2 );

	if ( self->mDeck ) {
		self->SetMask ( self->mDeck->GetContentMask ());
	}
	else {
		self->SetMask ( 0 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setFrame ( self, left, top, right, bottom )</tt>\n
\n
	Directly sets prop's frame rectangle.
	@param self (in)
	@param left (in)
	@param top (in)
	@param right (in)
	@param bottom (in)
*/
int MOAIProp2D::_setFrame ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UNNNN" )

	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float x1	= state.GetValue < float >( 4, 0.0f );
	float y1	= state.GetValue < float >( 5, 0.0f );
	
	self->mFrame.Init ( x0, y0, x1, y1 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setFrameSource ( self, source )</tt>\n
\n
	Sets source for frame rectangle.
	MOAIProp2D.FRAME_FROM_SOURCE - Frame is taken from content source.
	MOAIProp2D.FRAME_FROM_PARENT - Frame is taken from parent layout.
	MOAIProp2D.FRAME_FROM_DEFINE - Frame is taken from self.
	@param self (in)
	@param source (in)
*/
int MOAIProp2D::_setFrameSource ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UN" )

	self->mFrameSource = state.GetValue < u32 >( 2, self->mFrameSource );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setGrid ( self, grid )</tt>\n
\n
	Sets the grid for the tilemap. The grid provides the array of tilecodes
	used by the tilemap for looking up content from the content source.
	@param self (in)
	@param self (grid)
*/
int MOAIProp2D::_setGrid ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UU" )
	
	MOAIGrid* grid = state.GetLuaData < MOAIGrid >( 2 );
	if ( !grid ) return 0;
	
	self->mGrid = grid;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setContentAddr ( self, addr )</tt>\n
\n
	Sets the index for the asset to display in the content library.
	@param self (in)
	@param addr (in)
*/
int MOAIProp2D::_setIndex ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UN" )

	self->mIndex = state.GetValue < u32 >( 2, 0 );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setRepeat ( self, repeat )</tt>\n
\n
	Sets the tilemap to wrap (repeat continuously when rendered or queried against)
	or clamp (render once, like a sprite).
	@param self (in)
	@param repeatX (in)
	@param repeatY (in)
*/
int MOAIProp2D::_setRepeat ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UB" )

	bool repeatX = state.GetValue < bool >( 2, (( self->mRepeat & REPEAT_X ) == REPEAT_X ));
	bool repeatY = state.GetValue < bool >( 3, repeatX );

	self->mRepeat = 0;
	self->mRepeat |= repeatX ? REPEAT_X : 0;
	self->mRepeat |= repeatY ? REPEAT_Y : 0;

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setShader ( self, shader )</tt>\n
\n
	Sets the shader to use for this primitive.
	@param self (in)
	@param shader (in)
*/
int MOAIProp2D::_setShader ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UU" )

	if ( self->mShader ) {
		self->ClearDependency ( *self->mShader );
	}

	self->mShader = state.GetLuaData < MOAIShader >( 2 );
	self->SetDependency ( *self->mShader );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setUVTransform ( self, transform )</tt>\n
\n
	Sets a transform to be applied to the sprite source's UV coordinated
	prior to rendering.
	@param self (in)
	@param transform (in)
*/
int MOAIProp2D::_setUVTransform ( lua_State* L ) {
	LUA_SETUP ( MOAIProp2D, "UU" )
	
	MOAITransform* transform = state.GetLuaData < MOAITransform >( 2 );
	if ( !transform ) return 0;

	self->mUVTransform = transform;

	return 0;
}

//================================================================//
// MOAIProp2D
//================================================================//

//----------------------------------------------------------------//
void MOAIProp2D::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch( attrID ) {
		case ATTR_INDEX:
			this->mIndex = attrOp.Op ( this->mIndex );
			break;
		default:
			MOAITransform::ApplyAttrOp ( attrID, attrOp );
	}
}

//----------------------------------------------------------------//
u32 MOAIProp2D::CountAttributes () {

	return MOAIProp2D::TOTAL_ATTR;
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

	if ( !this->BindDeck ()) return;

	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();

	if ( this->mUVTransform ) {
		USAffine2D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
		drawbuffer.SetUVTransform ( uvMtx );
	}

	this->LoadShader ();
	
	if ( this->mGrid ) {
	
		USTileCoord c0;
		USTileCoord c1;
		
		this->GetBoundsInView ( c0, c1 );
		this->mDeck->Draw ( this->GetLocalToWorldMtx (), *this->mGrid, c0, c1 );
	}
	else {
		this->mDeck->Draw ( this->GetLocalToWorldMtx (), this->mIndex );
	}
	
	MOAILayoutFrame* parentFrame = USCast < MOAILayoutFrame >( this->mParent );
	if ( parentFrame ) {
		drawbuffer.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
void MOAIProp2D::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	if ( this->mDeck ) {
		if ( debugLines.Bind ( MOAIDebugLines::SPRITE_MODEL_BOUNDS )) {
			
			debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
			debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
			
			USRect bounds = this->mDeck->GetBounds ( this->mIndex );
			debugLines.DrawRect ( bounds );
			
			if ( this->mGrid ) {
		
				debugLines.SetPenColor ( 0x40ffffff );
				debugLines.SetPenWidth ( 2 );
		
				USTileCoord c0;
				USTileCoord c1;
				
				this->GetBoundsInView ( c0, c1 );
				this->mDeck->DrawDebug ( this->GetLocalToWorldMtx (), *this->mGrid, c0, c1 );
			}
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::SPRITE_WORLD_BOUNDS )) {
		debugLines.SetPenSpace ( MOAIDebugLines::WORLD_SPACE );
		debugLines.DrawRect ( this->GetBounds ( ));
	}
	
	debugLines.SetPenColor ( 0x40ffffff );
	debugLines.SetPenWidth ( 2 );
	
	if ( this->mDeck ) {
		this->mDeck->DrawDebug ( this->GetLocalToWorldMtx (), this->mIndex );
	}
	
	if ( debugLines.IsVisible ( MOAIDebugLines::PARTITION_CELLS ) || debugLines.IsVisible ( MOAIDebugLines::PARTITION_PADDED_CELLS )) {
		
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
		
		USTileCoord c0;
		USTileCoord c1;
		
		this->GetBoundsInRect ( localRect, c0, c1 );
		this->mDeck->GatherSurfaces ( *this->mGrid, c0, c1, sampler );
	}
	else {
		this->mDeck->GatherSurfaces ( this->mIndex, sampler );
	}
}

//----------------------------------------------------------------//
void MOAIProp2D::GetBoundsInRect ( const USRect& rect, USTileCoord& c0, USTileCoord& c1 ) {

	if ( this->mGrid ) {

		c0 = this->mGrid->GetTileCoord ( rect.mXMin, rect.mYMin );
		c1 = this->mGrid->GetTileCoord ( rect.mXMax, rect.mYMax );
		
		if ( !( this->mRepeat & REPEAT_X )) {
			this->mGrid->ClampX ( c0 );
			this->mGrid->ClampX ( c1 );
		}
		
		if ( !( this->mRepeat & REPEAT_Y )) {
			this->mGrid->ClampY ( c0 );
			this->mGrid->ClampY ( c1 );
		}
	}
	else {
		
		c0.Init ( 0, 0 );
		c1.Init ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIProp2D::GetBoundsInView ( USTileCoord& c0, USTileCoord& c1 ) {

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
u32 MOAIProp2D::GetLocalFrame ( USRect& frame ) {
	
	if ( this->mGrid ) {
	
		frame = this->mGrid->GetBounds ();
		return this->mRepeat ? BOUNDS_GLOBAL : BOUNDS_OK;
	}
	else if ( this->mDeck ) {
	
		frame = this->mDeck->GetBounds ( this->mIndex );
		return BOUNDS_OK;
	}
	
	return BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
bool MOAIProp2D::Inside ( USVec2D vec ) {

	if ( !this->mDeck ) return false;
	
	const USAffine2D& worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );
	
	return this->mDeck->Contains ( this->mIndex, vec );
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
	}
	else {
		drawbuffer.SetPenColor ( 1.0f, 1.0f, 1.0f, 1.0f );
		drawbuffer.SetBlendMode ( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	}
	
	MOAILayoutFrame* parent = USCast < MOAILayoutFrame >( this->mParent );
	if ( parent ) {
		USRect scissorRect = parent->GetScissorRect ();			
		drawbuffer.SetScissorRect ( scissorRect );
	}
	else {
		drawbuffer.SetScissorRect ();
	}
}

//----------------------------------------------------------------//
MOAIProp2D::MOAIProp2D () :
	mIndex( 0 ),
	mRepeat ( 0 ),
	mFrameSource ( FRAME_FROM_PARENT ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
	RTTI_END
	
	this->SetMask ( 0 );
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIProp2D::~MOAIProp2D () {
}

//----------------------------------------------------------------//
void MOAIProp2D::OnDepNodeUpdate () {
	
	USRect localFrame;
	localFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	u32 frameStatus = this->GetLocalFrame ( localFrame );
	
	if ( localFrame.Area () == 0.0f ) {
		frameStatus = BOUNDS_EMPTY;
	}
	
	// this here is where we do the logic to fit to the target frame
	// simply... we have our local frame
	// target frame is selected from parent (MOAIProp2D or MOAILayoutFrame)
	// then, if *scaling* to frame, just compute the matrix and prepend
	// if *stretching* to frame, compute the translation matrix and store the stretch factor
	// not all prims can stretch, btw... but that is their problem
	// the stretched/scaled prop should align with its parent frame only when using the identity transform
	// any additional transforms are applied *after* the fit; better for animated widgets, etc.
	
	// this is sort of a pain in the butt, but should be worth it
	// well, I hope it's worth it...
	
	USVec2D offset ( 0.0f, 0.0f );
	USVec2D stretch ( 1.0f, 1.0f );
	
	// default to match the local frame from source
	USRect targetFrame = localFrame;
	
	// select the frame
	if (( this->mFrameSource == FRAME_FROM_PARENT ) && this->mParent ) {
		
		MOAILayoutFrame* parentFrame = USCast < MOAILayoutFrame >( this->mParent );
		
		if ( parentFrame ) {
			targetFrame = parentFrame->GetFrame ();
		}
	}
	else if ( this->mFrameSource == FRAME_FROM_DEFINE ) {
		targetFrame = this->mFrame;
	}

	// and check if the target frame is empty, too
	if ( targetFrame.Area () == 0.0f ) {
		frameStatus = BOUNDS_EMPTY;
	}

	// compute the scale and offset (if any)
	if ( frameStatus != BOUNDS_EMPTY ) {
	
		float targetWidth = targetFrame.Width ();
		float targetHeight = targetFrame.Height ();
		
		stretch.mX = targetWidth / localFrame.Width ();
		stretch.mY = targetHeight / localFrame.Height ();
		
		offset.mX = targetFrame.mXMin - ( localFrame.mXMin * stretch.mX );
		offset.mY = targetFrame.mYMin - ( localFrame.mYMin * stretch.mY );
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
			USRect bounds = localFrame;
			this->mLocalToWorldMtx.Transform ( bounds );
			this->UpdateBounds ( bounds, frameStatus );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIProp2D::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIProp::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_INDEX", ( u32 )ATTR_INDEX );
	
	state.SetField ( -1, "FRAME_FROM_SOURCE", ( u32 )FRAME_FROM_SOURCE );
	state.SetField ( -1, "FRAME_FROM_PARENT", ( u32 )FRAME_FROM_PARENT );
	state.SetField ( -1, "FRAME_FROM_DEFINE", ( u32 )FRAME_FROM_DEFINE );
	
	state.SetField ( -1, "SCALE_TO_FRAME", ( u32 )SCALE_TO_FRAME );
	state.SetField ( -1, "STRETCH_TO_FRAME", ( u32 )STRETCH_TO_FRAME );
}

//----------------------------------------------------------------//
void MOAIProp2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "getGrid",			_getGrid },
		{ "getIndex",			_getIndex },
		{ "inside",				_inside },
		{ "setDeck",			_setDeck },
		{ "setFrame",			_setFrame },
		{ "setFrameSource",		_setFrameSource },
		{ "setGrid",			_setGrid },
		{ "setIndex",			_setIndex },
		{ "setRepeat",			_setRepeat },
		{ "setShader",			_setShader },
		{ "setUVTransform",		_setUVTransform },
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

