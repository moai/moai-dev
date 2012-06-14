// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILayoutFrame.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	layout
	@text	Perform the layout algorithm to layout hierarchy.

	@in		MOAILayoutFrame self
	@out	nil
*/
int MOAILayoutFrame::_layout ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayoutFrame, "U" )

	self->Layout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFit
	@text	Sets the fitting mode for each axis.

	@in		MOAILayoutFrame self
	@in		enum xFit				The fitting mode for the X axis; can be FIT_EXPAND, FIT_CONTENT or FIT_ABSOLUTE.
	@in		enum yFit				The fitting mode for the Y axis; can be FIT_EXPAND, FIT_CONTENT or FIT_ABSOLUTE.
	@out	nil
*/
int	MOAILayoutFrame::_setFit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayoutFrame, "U" )
	
	self->mFit [ X_AXIS ]	= state.GetValue < u32 >( 2, self->mFit [ X_AXIS ]);
	self->mFit [ Y_AXIS ]	= state.GetValue < u32 >( 3, self->mFit [ Y_AXIS ]);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setIdealSize
	@text	Sets the size hint used for fitting.

	@in		MOAILayoutFrame self
	@in		enum xSize				The size hinting for the X axis.
	@in		enum ySize				The size hinting for the Y axis.
	@out	nil
*/
int	MOAILayoutFrame::_setIdealSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayoutFrame, "U" )

	self->mIdealSize [ X_AXIS ]		= state.GetValue < float >( 2, self->mIdealSize [ X_AXIS ]);
	self->mIdealSize [ Y_AXIS ]		= state.GetValue < float >( 3, self->mIdealSize [ Y_AXIS ]);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLayout
	@text	Sets the layout mode for each axis.  Each argument can be one of LAYOUT_ALIGN_{MIN,CENTER,MAX} combined with LAYOUT_JUSTIFY_{MIN,CENTER,MAX}.  In this case, MIN, CENTER and MAX refer to the border along each axis; for the X axis MIN would be left and MAX would be right.

	@in		MOAILayoutFrame self
	@in		enum xLayout			The alignment and justification mode for the X axis.
	@in		enum yLayout			The alignment and justification mode for the Y axis.
	@out	nil
*/
int	MOAILayoutFrame::_setLayout ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayoutFrame, "U" )
	
	self->mLayout [ X_AXIS ]	= state.GetValue < u32 >( 2, self->mLayout [ X_AXIS ]);
	self->mLayout [ Y_AXIS ]	= state.GetValue < u32 >( 3, self->mLayout [ Y_AXIS ]);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMargin
	@text	Sets the interior margins of the layout.  This is the space between the edge of the layout and the content.

	@in		MOAILayoutFrame self
	@in		number left				The margin between the left edge of the layout and the content.
	@in		number top				The margin between the top edge of the layout and the content.
	@in		number right			The margin between the right edge of the layout and the content.
	@in		number bottom			The margin between the bottom edge of the layout and the content.
	@out	nil
*/
int MOAILayoutFrame::_setMargin ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayoutFrame, "U" )

	self->mMargin [ X_AXIS ][ MIN ]	= state.GetValue < float >( 2, self->mMargin [ X_AXIS ][ MIN ]);
	self->mMargin [ Y_AXIS ][ MIN ]	= state.GetValue < float >( 3, self->mMargin [ Y_AXIS ][ MIN ]);
	self->mMargin [ X_AXIS ][ MAX ]	= state.GetValue < float >( 4, self->mMargin [ X_AXIS ][ MAX ]);
	self->mMargin [ Y_AXIS ][ MAX ]	= state.GetValue < float >( 5, self->mMargin [ Y_AXIS ][ MAX ]);

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setWeight
	@text	Sets the stretch weighting for stretchable layouts (i.e. FIT_EXPAND).  This is the proportional stretch relative to each other, so any number space can be used; 1, 2, 5 is the same as 0.5, 1, 2.5.

	@in		MOAILayoutFrame self
	@in		number xWeight			The weighting for the X axis.
	@in		number yWeight			The weighting for the Y axis.
	@out	nil
*/
int	MOAILayoutFrame::_setWeight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayoutFrame, "U" )
	
	self->mWeight [ X_AXIS ]	= state.GetValue < float >( 2, self->mWeight [ X_AXIS ]);
	self->mWeight [ Y_AXIS ]	= state.GetValue < float >( 3, self->mWeight [ Y_AXIS ]);
	
	return 0;
}

//================================================================//
// MOAILayoutFrame
//================================================================//

//----------------------------------------------------------------//
void MOAILayoutFrame::ComputeContentSize ( u32 axis ) {

	// local copy of fitting mode
	u32 fit = this->mFit [ axis ];

	// grab the parent and its fitting mode
	MOAILayoutFrame* parent = this->GetParentWidget ();
	float parentSize = 0.0f;
	
	if ( parent ) {
		if (( fit == FIT_EXPAND ) && ( parent->mFit [ axis ] == FIT_CONTENT )) {
			fit = FIT_CONTENT;
		}
		else {
			parentSize = parent->GetInnerSize ( axis );
		}
	}

	// clear out the size
	this->mSize [ axis ] = 0;

	// if absolute, use ideal size
	if ( fit == FIT_ABSOLUTE ) {
		this->mSize [ axis ] = this->mIdealSize [ axis ];
	}

	// resolve parent fitting before computing size of children
	if ( fit == FIT_EXPAND ) {
		this->mSize [ axis ] = parentSize;
	}

	bool justified = (( this->mLayout [ axis ] & LAYOUT_JUSTIFY ) != 0 );
	float contentSize = 0.0f;

	// tally the size of the children
	ChildLink* childIt = this->mChildren.Head ();
	for ( ; childIt; childIt = childIt->Next ()) {
		MOAILayoutFrame* child = childIt->Data ();
		child->ComputeContentSize ( axis );
		
		float childSize = child->mSize [ axis ];
		
		if ( justified ) {
			contentSize += childSize;
		}
		else if ( childSize > contentSize ) {
			contentSize = childSize;
		}
	}
	
	this->mContentSize [ axis ] = contentSize;
	
	if ( fit == FIT_CONTENT ) {
		if ( this->mChildren.Count ()) {
			this->mSize [ axis ] = contentSize;
		}
		else {
			this->mSize [ axis ] = this->mIdealSize [ axis ];
		}
	}
}

//----------------------------------------------------------------//
void MOAILayoutFrame::Fit ( u32 axis ) {
	
	float size = this->GetInnerSize ( axis );
	float contentSize = this->mContentSize [ axis ];
	
	if ( size == contentSize ) return;
	contentSize = 0.0f;
	
	ChildLink* childIt;
	
	float fixSize = 0.0f;
	float flexCount = 0.0f;
	float totalWeight = 0.0f;
	
	childIt = this->mChildren.Head ();
	for ( ; childIt; childIt = childIt->Next ()) {
		MOAILayoutFrame* child = childIt->Data ();
		
		float idealSize = child->mIdealSize [ axis ];
		
		if ( child->mFit [ axis ] == FIT_EXPAND ) {
			flexCount += 1.0f;
			totalWeight += child->mWeight [ axis ];
		}
		else {
			fixSize += idealSize;
		}
	}
	
	float grow = size - fixSize;
	
	if ( grow >= 0.0f ) {
		
		// expand!
		childIt = this->mChildren.Head ();
		for ( ; childIt; childIt = childIt->Next ()) {
			MOAILayoutFrame* child = childIt->Data ();
			
			if ( child->mFit [ axis ] == FIT_EXPAND ) {
				float weight = child->mWeight [ axis ] / totalWeight;
				child->mSize [ axis ] = grow * weight;
			}
			contentSize += child->mSize [ axis ];
		}
	}
	else {
		
		// shrink!
		float scale = size / fixSize;
		
		childIt = this->mChildren.Head ();
		for ( ; childIt; childIt = childIt->Next ()) {
			MOAILayoutFrame* child = childIt->Data ();
			
			if ( child->mFit [ axis ] == FIT_EXPAND ) {
				child->mSize [ axis ] = 0.0f;
			}
			else {
				child->mSize [ axis ] = child->mSize [ axis ] * scale;
			}
			contentSize += child->mSize [ axis ];
		}
	}
	
	this->mContentSize [ axis ] = contentSize;
}

//----------------------------------------------------------------//
USRect MOAILayoutFrame::GetFrame () {

	USRect frame;
	
	frame.mXMin = this->mMargin [ X_AXIS ][ MIN ];
	frame.mYMin = this->mMargin [ Y_AXIS ][ MIN ];
	
	frame.mXMax = this->mSize [ X_AXIS ] - this->mMargin [ X_AXIS ][ MAX ];
	frame.mYMax = this->mSize [ Y_AXIS ] - this->mMargin [ Y_AXIS ][ MAX ];
	
	return frame;
}

//----------------------------------------------------------------//
float MOAILayoutFrame::GetInnerSize ( u32 axis ) {

	float innerSize = this->mSize [ axis ] - ( this->mMargin [ axis ][ MIN ] + this->mMargin [ axis ][ MAX ]);
	return innerSize < 0.0f ? 0.0f : innerSize;
}

//----------------------------------------------------------------//
MOAILayoutFrame* MOAILayoutFrame::GetParentWidget () {

	// TODO:
	//MOAITraits* source = this->GetTraitSource ( INHERIT_FRAME );
	//if ( source ) {
	//	return source->AsType < MOAILayoutFrame >();
	//}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAILayoutFrame::GetPerpAxis ( u32 axis ) {

	return (( axis + 1 ) & 0x00000001 );
}

//----------------------------------------------------------------//
USRect MOAILayoutFrame::GetScissorRect () {

	USRect scissorRect = this->GetFrame ();
	
	USMatrix4x4 mtx;
	
	// TODO:
	mtx.Init ( this->mLocalToWorldMtx );
	//mtx.Append ( MOAIGfxDevice::Get ().GetWorldToWndMtx ( 1.0f, 1.0f ));
	mtx.Transform ( scissorRect );
	
	return scissorRect;
}

//----------------------------------------------------------------//
float MOAILayoutFrame::GetSize ( u32 axis ) {

	return this->mSize [ axis ];
}

//----------------------------------------------------------------//
void MOAILayoutFrame::Layout () {
		
	// compute current width
	this->ComputeContentSize ( X_AXIS );
	this->ComputeContentSize ( Y_AXIS );
		
	this->Layout ( X_AXIS );
	this->Layout ( Y_AXIS );
}

//----------------------------------------------------------------//
void MOAILayoutFrame::Layout ( u32 axis ) {

	float size = this->GetInnerSize ( axis );

	float minMargin = this->mMargin [ axis ][ MIN ];
	float maxMargin = this->mMargin [ axis ][ MAX ];

	if ( size == 0.0f ) {
		
		float margin = minMargin + maxMargin;
	
		float loc = 0;
		if ( margin > 0.0f ) {
			loc = minMargin * ( this->mSize [ axis ] / margin );
		}
		
		ChildLink* childIt = this->mChildren.Head ();
		for ( ; childIt; childIt = childIt->Next ()) {
			MOAILayoutFrame* child = childIt->Data ();
			
			child->SetLocByAxis ( axis, loc );
			child->mSize [ axis ] = 0.0f;
			child->Layout ( axis );
		}
	}
	else if ( this->mLayout [ axis ] & LAYOUT_ALIGN ) {
		
		float offset = 0.0f;
		float scale = 0.0f;
		
		switch ( this->mLayout [ axis ]) {

			case LAYOUT_ALIGN_MIN: {
				offset = minMargin;
				scale = 0.0f;
				break;
			}
			case LAYOUT_ALIGN_MAX: {
				offset = minMargin + size;
				scale = 1.0f;
				break;
			}
			case LAYOUT_ALIGN_CENTER: {
				offset = minMargin + ( size * 0.5f );
				scale = 0.5f;
				break;
			}
		}
		
		ChildLink* childIt = this->mChildren.Head ();
		for ( ; childIt; childIt = childIt->Next ()) {
			MOAILayoutFrame* child = childIt->Data ();
			
			if ( child->mSize [ axis ] > size ) {
				child->mSize [ axis ] = size;
			}
			
			child->SetLocByAxis ( axis, offset - ( child->mSize [ axis ] * scale ));
			child->Layout ( axis );
		}
	}
	else if ( this->mLayout [ axis ] & LAYOUT_JUSTIFY ) {
				
		this->Fit ( axis );
		
		float cursor = 0.0f;
		float empty = size - this->mContentSize [ axis ];
		
		switch ( this->mLayout [ axis ]) {

			case LAYOUT_JUSTIFY_MIN: {
				cursor = minMargin;
				break;
			}
			case LAYOUT_JUSTIFY_MAX: {
				cursor = minMargin + empty;
				break;
			}
			case LAYOUT_JUSTIFY_CENTER: {
				cursor = minMargin + ( empty * 0.5f );
				break;
			}
		}
		
		ChildLink* childIt = this->mChildren.Head ();
		for ( ; childIt; childIt = childIt->Next ()) {
			MOAILayoutFrame* child = childIt->Data ();
			
			child->SetLocByAxis ( axis, cursor );
			cursor += child->mSize [ axis ];
			child->Layout ( axis );
		}
	}
}

//----------------------------------------------------------------//
MOAILayoutFrame::MOAILayoutFrame () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
	
	this->mMargin [ X_AXIS ][ MIN ] = 0.0f;
	this->mMargin [ Y_AXIS ][ MIN ] = 0.0f;
	this->mMargin [ X_AXIS ][ MAX ] = 0.0f;
	this->mMargin [ Y_AXIS ][ MAX ] = 0.0f;
	
	this->mLayout [ X_AXIS ] = LAYOUT_ALIGN_MIN;
	this->mLayout [ Y_AXIS ] = LAYOUT_ALIGN_MIN;
	
	this->mFit [ X_AXIS ] = FIT_ABSOLUTE;
	this->mFit [ Y_AXIS ] = FIT_ABSOLUTE;
	
	this->mWeight [ X_AXIS ] = 1.0f;
	this->mWeight [ Y_AXIS ] = 1.0f;
	
	this->mIdealSize [ X_AXIS ] = 0.0f;
	this->mIdealSize [ Y_AXIS ] = 0.0f;
	
	this->mLinkInChildren.Data ( this );
}

//----------------------------------------------------------------//
MOAILayoutFrame::~MOAILayoutFrame () {

	this->RemoveChildren ();
}

//----------------------------------------------------------------//
void MOAILayoutFrame::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaClass ( state );
	
	state.SetField ( -1, "LAYOUT_ALIGN_MIN", ( u32 )LAYOUT_ALIGN_MIN );
	state.SetField ( -1, "LAYOUT_ALIGN_MAX", ( u32 )LAYOUT_ALIGN_MAX );
	state.SetField ( -1, "LAYOUT_ALIGN_CENTER", ( u32 )LAYOUT_ALIGN_CENTER );
	
	state.SetField ( -1, "LAYOUT_JUSTIFY_MIN", ( u32 )LAYOUT_JUSTIFY_MIN );
	state.SetField ( -1, "LAYOUT_JUSTIFY_MAX", ( u32 )LAYOUT_JUSTIFY_MAX );
	state.SetField ( -1, "LAYOUT_JUSTIFY_CENTER", ( u32 )LAYOUT_JUSTIFY_CENTER );
	
	state.SetField ( -1, "FIT_EXPAND", ( u32 )FIT_EXPAND );
	state.SetField ( -1, "FIT_ABSOLUTE", ( u32 )FIT_ABSOLUTE );
	state.SetField ( -1, "FIT_CONTENT", ( u32 )FIT_CONTENT );
}

//----------------------------------------------------------------//
void MOAILayoutFrame::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "layout",				_layout },
		{ "setFit",				_setFit },
		{ "setIdealSize",		_setIdealSize },
		{ "setLayout",			_setLayout },
		{ "setMargin",			_setMargin },
		{ "setWeight",			_setWeight },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILayoutFrame::RemoveChildren () {

	ChildLink* childIt = this->mChildren.Head ();
	for ( ; childIt; childIt = childIt->Next ()) {
		MOAILayoutFrame* child = childIt->Data ();
		
		child->SetParent ( 0 );
	}
}

//----------------------------------------------------------------//
void MOAILayoutFrame::SetLocByAxis ( u32 axis, float loc ) {

	if ( axis == Y_AXIS ) {
		this->mLoc.mY = loc;
	}
	else {
		this->mLoc.mX = loc;
	}
}

//----------------------------------------------------------------//
void MOAILayoutFrame::SetParent ( MOAITransformBase* parent ) {
	UNUSED ( parent );

	// TODO
	//this->mLinkInChildren.Remove ();
	//
	//this->MOAITransform::SetParent ( parent );
	//
	//if ( parent ) {
	//	MOAILayoutFrame* parentWidget = parent->AsType < MOAILayoutFrame >();
	//	if ( parentWidget ) {
	//		parentWidget->mChildren.PushBack ( this->mLinkInChildren );
	//	}
	//}
}
