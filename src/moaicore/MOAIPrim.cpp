// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIFrame.h>
#include <moaicore/MOAIPrim.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( priority ) getPriority ( self )</tt>\n
\n
	Returns current rendering priority of prim.
	@param self (in)
	@param priority (out)
*/
int MOAIPrim::_getPriority ( lua_State* L ) {
	LUA_SETUP ( MOAIPrim, "U" )
	
	lua_pushnumber ( state, self->mPriority );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setFrame ( self, left, top, right, bottom )</tt>\n
\n
	Directly sets prim's frame rectangle.
	@param self (in)
	@param left (in)
	@param top (in)
	@param right (in)
	@param bottom (in)
*/
int MOAIPrim::_setFrame ( lua_State* L ) {
	LUA_SETUP ( MOAIPrim, "UNNNN" )

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
	MOAIPrim.FRAME_FROM_SOURCE - Frame is taken from content source.
	MOAIPrim.FRAME_FROM_PARENT - Frame is taken from parent layout.
	MOAIPrim.FRAME_FROM_DEFINE - Frame is taken from self.
	@param self (in)
	@param source (in)
*/
int MOAIPrim::_setFrameSource ( lua_State* L ) {
	LUA_SETUP ( MOAIPrim, "UN" )

	self->mFrameSource = state.GetValue < u32 >( 2, self->mFrameSource );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setPriority ( self, priority )</tt>\n
\n
	Sets the primitive's render priority.
	@param self (in)
	@param priority (in)
*/
int MOAIPrim::_setPriority ( lua_State* L ) {
	LUA_SETUP ( MOAIPrim, "UN" )

	self->mPriority	= state.GetValue < int >( 2, 0 );

	return 0;
}

//================================================================//
// MOAIPrim
//================================================================//

//----------------------------------------------------------------//
void MOAIPrim::Draw () {
}

//----------------------------------------------------------------//
void MOAIPrim::DrawDebug () {
}

//----------------------------------------------------------------//
void MOAIPrim::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
u32 MOAIPrim::GetLocalFrame ( USRect& frame ) {
	UNUSED ( frame );

	return FRAME_EMPTY;
}

//----------------------------------------------------------------//
MOAIOverlapPrim2D* MOAIPrim::IsOverlapPrim2D () {
	return 0;
}

//----------------------------------------------------------------//
MOAIPrim::MOAIPrim () :
	mFrameSource ( FRAME_FROM_PARENT ),
	mPriority ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform2D )
	RTTI_END
	
	this->SetQueryMask ( 0 );
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIPrim::~MOAIPrim () {
}

//----------------------------------------------------------------//
void MOAIPrim::OnDepNodeUpdate () {
	
	USRect localFrame;
	localFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	u32 frameStatus = this->GetLocalFrame ( localFrame );
	
	if ( localFrame.Area () == 0.0f ) {
		frameStatus = FRAME_EMPTY;
	}
	
	// this here is where we do the logic to fit to the target frame
	// simply... we have our local frame
	// target frame is selected from parent (MOAIPrim or MOAIFrame)
	// then, if *scaling* to frame, just compute the matrix and prepend
	// if *stretching* to frame, compute the translation matrix and store the stretch factor
	// not all prims can stretch, btw... but that is their problem
	// the stretched/scaled prim should align with its parent frame only when using the identity transform
	// any additional transforms are applied *after* the fit; better for animated widgets, etc.
	
	// this is sort of a pain in the butt, but should be worth it
	// well, I hope it's worth it...
	
	this->mStretch.Init ( 1.0f, 1.0f );
	USVec2D offset ( 0.0f, 0.0f );
	
	// default to match the local frame from source
	USRect targetFrame = localFrame;
	
	// select the frame
	if (( this->mFrameSource == FRAME_FROM_PARENT ) && this->mParent ) {
		
		MOAIFrame* parentFrame = USCast < MOAIFrame >( this->mParent );
		
		if ( parentFrame ) {
			targetFrame = parentFrame->GetFrame ();
		}
	}
	else if ( this->mFrameSource == FRAME_FROM_DEFINE ) {
		targetFrame = this->mFrame;
	}

	// and check if the target frame is empty, too
	if ( targetFrame.Area () == 0.0f ) {
		frameStatus = FRAME_EMPTY;
	}

	// compute the scale and offset (if any)
	if ( frameStatus != FRAME_EMPTY ) {
	
		float targetWidth = targetFrame.Width ();
		float targetHeight = targetFrame.Height ();
		
		this->mStretch.mX = targetWidth / localFrame.Width ();
		this->mStretch.mY = targetHeight / localFrame.Height ();
		
		offset.mX = targetFrame.mXMin - ( localFrame.mXMin * this->mStretch.mX );
		offset.mY = targetFrame.mYMin - ( localFrame.mYMin * this->mStretch.mY );
	}
	
	// inherit parent and offset transforms (and compute the inverse)
	this->BuildTransforms ( offset.mX, offset.mY );
	
	// update the prim location in the partition
	// use the local frame; world transform will match it to target frame
	switch ( frameStatus ) {
		case FRAME_EMPTY: {
			this->SetWorldBoundsEmpty ();
			break;
		}
		case FRAME_GLOBAL: {
			this->SetWorldBoundsGlobal ();
			break;
		}
		case FRAME_OK: {
			USRect bounds = localFrame;
			this->mLocalToWorldMtx.Transform ( bounds );
			this->SetWorldBounds ( bounds );
			break;
		}
	}
	
	// and draw the partition cells if debug lines enabled
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	if ( debugLines.IsVisible ( MOAIDebugLines::PARTITION_CELLS ) || debugLines.IsVisible ( MOAIDebugLines::PARTITION_PADDED_CELLS )) {
	
		USRect cellRect;
		USRect paddedRect;
	
		if ( this->GetCellRect ( &cellRect, &paddedRect )) {

			if ( debugLines.Bind ( MOAIDebugLines::PARTITION_CELLS )) {
				debugLines.SetPenSpace ( MOAIDebugLines::PARTITION_CELLS );
				debugLines.DrawRect ( cellRect );
			}
			
			if ( debugLines.Bind ( MOAIDebugLines::PARTITION_PADDED_CELLS )) {
				debugLines.SetPenSpace ( MOAIDebugLines::PARTITION_PADDED_CELLS );
				debugLines.DrawRect ( paddedRect );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPrim::RegisterLuaClass ( USLuaState& state ) {
	
	MOAITransform2D::RegisterLuaClass ( state );
	
	state.SetField ( -1, "FRAME_FROM_SOURCE", ( u32 )FRAME_FROM_SOURCE );
	state.SetField ( -1, "FRAME_FROM_PARENT", ( u32 )FRAME_FROM_PARENT );
	state.SetField ( -1, "FRAME_FROM_DEFINE", ( u32 )FRAME_FROM_DEFINE );
	
	state.SetField ( -1, "SCALE_TO_FRAME", ( u32 )SCALE_TO_FRAME );
	state.SetField ( -1, "STRETCH_TO_FRAME", ( u32 )STRETCH_TO_FRAME );
}

//----------------------------------------------------------------//
void MOAIPrim::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAITransform2D::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "getPriority",		_getPriority },
		{ "setFrame",			_setFrame },
		{ "setFrameSource",		_setFrameSource },
		{ "setPriority",		_setPriority },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIPrim::ToString () {

	return "";
}

