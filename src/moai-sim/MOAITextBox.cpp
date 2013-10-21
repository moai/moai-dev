// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAINodeMgr.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextDesigner.h>
#include <moai-sim/MOAITextBox.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleParser.h>

// TODO: font garbage collection
//			- ref count glyphs
//			- glyph lifecycle
//			- glyph page defragmantation
// TODO: bitmap font ripper
// TODO: hit test for characters

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clearHighlights
	@text	Removes all highlights currently associated with the text box.

	@in		MOAITextBox self
	@out	nil
*/
int MOAITextBox::_clearHighlights ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	
	self->mLayout.ClearHighlights ();
	self->ScheduleLayout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAlignment
	@text	Returns the alignment of the text

	@in		MOAITextBox self
	@out	number hAlign			horizontal alignment
	@out	number vAlign			vertical alignment
*/
int MOAITextBox::_getAlignment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	state.Push ( self->mHAlign );
	state.Push ( self->mVAlign );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getGlyphScale
	@text	Returns the current glyph scale.

	@in		MOAITextBox self
	@out	number glyphScale
*/
int MOAITextBox::_getGlyphScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	state.Push ( self->mGlyphScale );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLineSpacing
	@text	Returns the spacing between lines (in pixels).

	@in		MOAITextBox self
	@out	number lineScale		The size of the spacing in pixels.
*/
int MOAITextBox::_getLineSpacing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	
	lua_pushnumber ( state, self->mLineSpacing );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getRect
	@text	Returns the two-dimensional boundary of the text box.

	@in		MOAITextBox self
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAITextBox::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	ZLRect rect = self->mFrame;
	rect.Bless ();
	
	lua_pushnumber ( state, rect.mXMin );
	lua_pushnumber ( state, rect.mYMin );
	lua_pushnumber ( state, rect.mXMax );
	lua_pushnumber ( state, rect.mYMax );

	return 4;
}

//----------------------------------------------------------------//
/**	@name	getString
	@text	Return the text string.

	@in		MOAITextBox self
	@out	string text			Text string.
*/
int MOAITextBox::_getString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	if ( self->mText ) {
		lua_pushstring ( state, self->mText );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getStringBounds
	@text	Returns the bounding rectangle of a given substring on a
			single line in the local space of the text box.

	@in		MOAITextBox self
	@in		number index		Index of the first character in the substring.
	@in		number size			Length of the substring.
	@out	number xMin			Edge of rect or 'nil' is no match found.
	@out	number yMin			Edge of rect or 'nil' is no match found.
	@out	number xMax			Edge of rect or 'nil' is no match found.
	@out	number yMax			Edge of rect or 'nil' is no match found.
*/
int MOAITextBox::_getStringBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UNN" )
	
	u32 index	= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 size	= state.GetValue < u32 >( 3, 0 );
	
	ZLRect rect;
	if ( self->mLayout.GetBoundsForRange ( index, size, rect )) {
		
		rect.Bless ();
		
		lua_pushnumber ( state, rect.mXMin );
		lua_pushnumber ( state, rect.mYMin );
		lua_pushnumber ( state, rect.mXMax );
		lua_pushnumber ( state, rect.mYMax );
		
		return 4;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getStyle
	@text	Returns the style associated with a name or, if no name
			is given, returns the default style.

	@overload
	
		@in		MOAITextBox self
		@out	MOAITextStyle defaultStyle
	
	@overload
	
		@in		MOAITextBox self
		@in		string styleName
		@out	MOAITextStyle style
*/
int MOAITextBox::_getStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	MOAITextStyle* style = 0;

	cc8* styleName = state.GetValue < cc8* >( 2, 0 );
	style = self->mStyler.GetStyle ( styleName );

	if ( style ) {
		style->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	more
	@text	Returns whether there are additional pages of text below the cursor position that are not visible on the screen.

	@in		MOAITextBox self
	@out	boolean isMore				If there is additional text below the cursor that is not visible on the screen due to clipping.
*/
int MOAITextBox::_more ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	
	lua_pushboolean ( L, self->mMore );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	nextPage
	@text	Advances to the next page of text (if any) or wraps to the start of the text (if at end).

	@in		MOAITextBox self
	@opt	boolean reveal		Default is true
	@out	nil
*/
int MOAITextBox::_nextPage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	bool reveal = state.GetValue < bool >( 2, true );
	self->NextPage ( reveal );
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveCurves
	@text	Reserves a set of IDs for animation curves to be binding to this text object.  See setCurves.

	@in		MOAITextBox self
	@in		number nCurves
	@out	nil
*/
int MOAITextBox::_reserveCurves ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveCurves ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	revealAll
	@text	Displays as much text as will fit in the text box.

	@in		MOAITextBox self
	@out	nil
*/
int MOAITextBox::_revealAll ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	
	self->mReveal = REVEAL_ALL;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAlignment
	@text	Sets the horizontal and/or vertical alignment of the text in the text box.

	@in		MOAITextBox self
	@in		number hAlignment				Can be one of LEFT_JUSTIFY, CENTER_JUSTIFY or RIGHT_JUSTIFY.
	@in		number vAlignment				Can be one of LEFT_JUSTIFY, CENTER_JUSTIFY or RIGHT_JUSTIFY.
	@out	nil
*/
int MOAITextBox::_setAlignment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UN" )

	self->mHAlign = state.GetValue < u32 >( 2, MOAITextBox::LEFT_JUSTIFY );
	self->mVAlign = state.GetValue < u32 >( 3, MOAITextBox::LEFT_JUSTIFY );
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCurve
	@text	Binds an animation curve to the text, where the Y value of the curve indicates the text offset, or clears the curves.

	@overload

		@in		MOAITextBox self
		@in		number curveID				The ID of the curve within this text object.
		@in		MOAIAnimCurve curve			The MOAIAnimCurve to bind to.
		@out	nil
	
	@overload
		
		@in		MOAITextBox self
		@out	nil
*/
int MOAITextBox::_setCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	
	if ( state.GetTop () > 1 ) {

		u32 index = state.GetValue < u32 >( 2, 1 ) - 1;
	
		MOAIAnimCurve* curve = state.GetLuaObject < MOAIAnimCurve >( 3, true );
		if ( !curve ) return 0;

		self->SetCurve ( index, curve );
		self->ScheduleLayout ();
	}
	else {
		self->ClearCurves ();
		self->ScheduleLayout ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGlyphScale
	@text	Sets the glyph scale. This is a scalar applied to glyphs
			as they are positioned in the text box.

	@in		MOAITextBox self
	@opt	number glyphScale		Default value is 1.
	@out	number glyphScale
*/
int MOAITextBox::_setGlyphScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	self->mGlyphScale = state.GetValue < float >( 2, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setHighlight
	@text	Set or clear the highlight color of a sub string in the text.
			Only affects text displayed on the current page. Highlight
			will automatically clear when layout or page changes.

	@overload

		@in		MOAITextBox self
		@in		number index		Index of the first character in the substring.
		@in		number size			Length of the substring.
		@in		number r
		@in		number g
		@in		number b
		@opt	number a			Default value is 1.
		@out	nil
	
	@overload
		
		@in		MOAITextBox self
		@in		number index		Index of the first character in the substring.
		@in		number size			Length of the substring.
		@out	nil
*/
int MOAITextBox::_setHighlight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UNN" )
	
	u32 index	= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 size	= state.GetValue < u32 >( 3, 0 );
	
	if ( size ) {
		
		if ( state.GetTop () > 3 ) {
			u32 rgba = state.GetColor32 ( 4, 1.0f, 1.0f, 1.0f, 1.0f );
			self->mLayout.SetHighlight ( index, size, rgba );
		}
		else {
			self->mLayout.SetHighlight ( index, size );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLineSpacing
	@text	Sets additional space between lines in text units. '0' uses
			the default spacing. Values must be positive.

	@in		MOAITextBox self
	@in		number lineSpacing		Default value is 0.
	@out	nil
*/
int MOAITextBox::_setLineSpacing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	
	float lineSpacing = state.GetValue < float >( 2, 0.0f );
	self->mLineSpacing = lineSpacing < 0.0f ? 0.0f : lineSpacing;
	self->ScheduleLayout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Sets the rectangular area for this text box.

	@in		MOAITextBox self
	@in		number x1					The X coordinate of the rect's upper-left point.
	@in		number y1					The Y coordinate of the rect's upper-left point.
	@in		number x2					The X coordinate of the rect's lower-right point.
	@in		number y2					The Y coordinate of the rect's lower-right point.
	@out	nil
*/
int MOAITextBox::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UNNNN" )

	float left		= state.GetValue < float >( 2, 0.0f );
	float top		= state.GetValue < float >( 3, 0.0f );
	float right		= state.GetValue < float >( 4, 0.0f );
	float bottom	= state.GetValue < float >( 5, 0.0f );

	self->SetRect ( left, top, right, bottom );
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setReveal
	@text	Sets the number of renderable characters to be shown. 
			Can range from 0 to any value; values greater than the
			number of renderable characters in the current text will
			be ignored.

	@in		MOAITextBox self
	@in		number reveal				The number of renderable characters (i.e. not whitespace) to be shown.
	@out	nil
*/
int MOAITextBox::_setReveal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UN" )

	self->mReveal = state.GetValue < u32 >( 2, self->mReveal );
	self->mSpool = ( float )self->mReveal;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSnapToViewportScale
	@text	If set to true text positions will snap to integers according to the viewport scale. Default value is true.

	@in		MOAITextBox self
	@in		boolean snap				Whether text positions should snap to viewport scale
	@out	nil
*/
int MOAITextBox::_setSnapToViewportScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UB" )
	
	self->mSnapToViewportScale = state.GetValue < bool >( 2, self->mSnapToViewportScale );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSpeed
	@text	Sets the base spool speed used when creating a spooling MOAIAction with the spool() function.

	@in		MOAITextBox self
	@in		number speed				The base spooling speed.
	@out	nil
*/
int MOAITextBox::_setSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UN" )
	
	self->mSpeed = state.GetValue < float >( 2, self->mSpeed );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setString
	@text	Sets the text string to be displayed by this textbox.

	@in		MOAITextBox self
	@in		string newStr				The new text string to be displayed.
	@out	nil
*/
int MOAITextBox::_setString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "US" )

	cc8* text = state.GetValue < cc8* >( 2, "" );
	self->SetText ( text );
	self->ScheduleLayout ();
	self->mMore = ( text && text [ 0 ]);

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setStyle
	@text	Attaches a style to the textbox and associates a name with it.
			If no name is given, sets the default style.

	@overload
	
		@in		MOAITextBox self
		@in		MOAITextStyle defaultStyle
		@out	nil
	
	@overload
	
		@in		MOAITextBox self
		@in		string styleName
		@in		MOAITextStyle style
		@out	nil
*/
int MOAITextBox::_setStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	cc8* styleName = state.GetValue < cc8* >( 2, "" );
	
	if ( strlen ( styleName )) {
	
		MOAITextStyle* style = state.GetLuaObject < MOAITextStyle >( 3, true );
		self->mStyler.SetStyle ( styleName, style );
	}
	else {
	
		MOAITextStyle* style = state.GetLuaObject < MOAITextStyle >( 2, true );
		self->mStyler.SetStyle ( style );
	}
	
	self->ScheduleLayout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setWordBreak
	@text	Sets the rule for breaking words across lines.

	@in		MOAITextBox self
	@opt	number rule				One of MOAITextBox.WORD_BREAK_NONE, MOAITextBox.WORD_BREAK_CHAR.
									Default is MOAITextBox.WORD_BREAK_NONE.
	@out	nil
*/
int MOAITextBox::_setWordBreak ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	self->mWordBreak = state.GetValue < u32 >( 2, MOAITextBox::WORD_BREAK_NONE );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setYFlip
	@text	Sets the rendering direction for the text. Default assumes
			a window style screen space (positive Y moves down the screen). Set
			to true to render text for world style coordinate systems (positive
			Y moves up the screen).

	@in		MOAITextBox self
	@in		boolean yFlip				Whether the vertical rendering direction should be inverted.
	@out	nil
*/
int MOAITextBox::_setYFlip ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UB" )

	self->mYFlip = state.GetValue < bool >( 2, self->mYFlip );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	spool
	@text	Creates a new MOAIAction which when run has the effect of increasing
			the amount of characters revealed from 0 to the length of the string
			currently set.  The spool action is automatically added to the root
			of the action tree, but may be reparented or stopped by the developer.
			This function also automatically sets the current number of revealed
			characters to 0 (i.e. MOAITextBox:setReveal(0)).

	@in		MOAITextBox self
	@in		number yFlip				Whether the vertical rendering direction should be inverted.
	@out	MOAIAction action			The new MOAIAction which spools the text when run.
*/
int MOAITextBox::_spool ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	self->mReveal = state.GetValue < u32 >( 2, 0 );
	self->mSpool = ( float )self->mReveal;

	self->Start ();

	return 1;
}

//================================================================//
// DOXYGEN
//================================================================//

#ifdef DOXYGEN

	//----------------------------------------------------------------//
	/**	@name	affirmStyle
		@text	Returns the textbox's default style. If no default style
				exists, creates an empty style, sets it as the default and
				returns it.

		@in		MOAITextBox self
		@out	MOAITextStyle style
	*/
	int MOAITextBox::_affirmStyle ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@name	setFont
		@text	Sets the font to be used by the textbox's default style.
				If no default style exists, a default style is created.

		@in		MOAITextBox self
		@in		MOAIFont font
		@out	nil
	*/
	int MOAITextBox::_setFont ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@name	setTextSize
		@text	Sets the size to be used by the textbox's default style.
				If no default style exists, a default style is created.

		@in		MOAITextBox self
		@in		number points			The point size to be used by the default style.
		@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
		@out	nil
	*/
	int MOAITextBox::_setTextSize ( lua_State* L ) {
	}

#endif

//================================================================//
// MOAITextBox
//================================================================//

const float MOAITextBox::DEFAULT_SPOOL_SPEED = 24.0f;

//----------------------------------------------------------------//
void MOAITextBox::ClearCurves () {

	for ( u32 i = 0; i < this->mCurves.Size (); ++i ) {
		this->LuaRelease ( this->mCurves [ i ]);
	}
	this->mCurves.Clear ();
}

//----------------------------------------------------------------//
void MOAITextBox::Draw ( int subPrimID ) {
	UNUSED ( subPrimID ); 
	
	if ( !this->IsVisible () ) return;
	
	if ( this->mReveal ) {
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

		if ( this->mUVTransform ) {
			ZLAffine3D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
			gfxDevice.SetUVTransform ( uvMtx );
		}
		else {
			gfxDevice.SetUVTransform ();
		}
		
		this->LoadGfxState ();

		if ( !this->mShader ) {
			// TODO: this should really come from MOAIFont, which should really be a
			// specialized implementation of MOAIDeck...
			gfxDevice.SetShaderPreset ( MOAIShaderMgr::FONT_SHADER );
		}

		gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, this->GetLocalToWorldMtx ());
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
		this->mLayout.Draw ( this->mReveal );
	}
}

//----------------------------------------------------------------//
void MOAITextBox::DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID ); 

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, this->GetLocalToWorldMtx ());
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX )) {
		draw.DrawRectOutline ( this->mFrame );
	}
	
	this->mLayout.DrawDebug ();
}

//----------------------------------------------------------------//
u32 MOAITextBox::GetPropBounds ( ZLBox& bounds ) {

	bounds.Init ( this->mFrame.mXMin, this->mFrame.mYMax, this->mFrame.mXMax, this->mFrame.mYMin, 0.0f, 0.0f );
	return MOAIProp::BOUNDS_OK;
}

//----------------------------------------------------------------//
bool MOAITextBox::IsDone () {

	if ( this->IsActive ()) {
		this->Layout ();
		return ( this->mReveal >= this->mLayout.CountSprites ());
	}
	return true;
}

//----------------------------------------------------------------//
void MOAITextBox::Layout () {

	if ( !this->mText ) {
		this->mStyler.ResetStyleMap ();
		this->ResetLayout ();
	}
	else if ( this->mNeedsLayout ) {
		
		this->mStyler.BuildStyleMap ( this->mText.c_str ());
		
		this->ResetLayout ();
		
		MOAITextDesigner designer;
		designer.Init ( this->mLayout, this->mStyler, this->mText.c_str (), this->mCurrentPageIdx );
		
		ZLVec2D pen ( this->mFrame.mXMin, this->mFrame.mYMin );
		designer.SetLoc ( pen );
		
		designer.SetWidth ( this->mFrame.Width ());
		designer.SetHeight ( this->mFrame.Height ());
		
		designer.SetLimitWidth ( true );
		designer.SetLimitHeight ( true );
		
		designer.SetHAlign ( this->mHAlign );
		designer.SetVAlign ( this->mVAlign );
		
		designer.SetWordBreak ( this->mWordBreak );
		designer.SetGlyphScale ( this->mGlyphScale );
		designer.SetLineSpacing ( this->mLineSpacing );
		
		designer.SetCurves ( this->mCurves, this->mCurves.Size ());
		designer.BuildLayout ();
		
		this->mNextPageIdx = designer.GetIndex ();
		this->mMore = designer.More ();
		
		this->mLayout.ApplyHighlights ();
	}
	
	this->mNeedsLayout = false;
}

//----------------------------------------------------------------//
MOAITextBox::MOAITextBox () :
	mLineSpacing ( 0.0f ),
	mHAlign ( LEFT_JUSTIFY ),
	mVAlign ( LEFT_JUSTIFY ),
	mSpool ( 0.0f ),
	mSpeed ( DEFAULT_SPOOL_SPEED ),
	mReveal ( REVEAL_ALL ),
	mYFlip ( false ),
	mGlyphScale ( 1.0f ),
	mSnapToViewportScale ( true ),
	mCurrentPageIdx ( 0 ),
	mNextPageIdx ( 0 ),
	mNeedsLayout ( false ),
	mMore ( false ),
	mWordBreak ( WORD_BREAK_NONE ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f ); 
	this->SetMask ( MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
	this->mBlendMode.SetBlend ( ZGL_BLEND_FACTOR_SRC_ALPHA, ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
	
	this->SetText ( "" );
}

//----------------------------------------------------------------//
MOAITextBox::~MOAITextBox () {

	this->ClearCurves ();
	this->mLayout.ClearHighlights ();
	this->ResetLayout ();
}

//----------------------------------------------------------------//
bool MOAITextBox::More () {
	
	this->Layout ();
	
	if ( this->mReveal < this->mLayout.CountSprites ()) {
		return true;
	}
	return this->mMore;
}

//----------------------------------------------------------------//
void MOAITextBox::NextPage ( bool reveal ) {
	
	if ( this->mMore ) {
		this->mCurrentPageIdx = this->mNextPageIdx;
	}
	else {
		this->mCurrentPageIdx = 0;
		this->mNextPageIdx = 0;
	}
	
	this->mReveal = reveal ? REVEAL_ALL : 0;
	this->mSpool = 0.0f;
}

//----------------------------------------------------------------//
void MOAITextBox::OnDepNodeUpdate () {

	MOAIProp::OnDepNodeUpdate ();

	if ( this->mStyler.CheckStylesChanged ()) {
		this->mNeedsLayout = true;
		this->mStyler.RefreshStyleGlyphs ( this->mText.c_str ());
	}
	
	this->Layout ();

	if ( this->mYFlip ) {
		
		ZLAffine3D mtx;
		
		mtx.ScRoTr ( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, ( this->mFrame.mYMin + this->mFrame.mYMax ), 0.0f );
		this->mLocalToWorldMtx.Prepend ( mtx );
		this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
	}
}

//----------------------------------------------------------------//
void MOAITextBox::OnUpdate ( float step ) {
	
	this->mSpool += ( this->mSpeed * step );
	this->mReveal = ( u32 )this->mSpool;
}

//----------------------------------------------------------------//
void MOAITextBox::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIProp::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );

	state.SetField ( -1, "LEFT_JUSTIFY", ( u32 )LEFT_JUSTIFY );
	state.SetField ( -1, "CENTER_JUSTIFY", ( u32 )CENTER_JUSTIFY );
	state.SetField ( -1, "RIGHT_JUSTIFY", ( u32 )RIGHT_JUSTIFY );
	
	state.SetField ( -1, "WORD_BREAK_NONE", ( u32 )WORD_BREAK_NONE );
	state.SetField ( -1, "WORD_BREAK_CHAR", ( u32 )WORD_BREAK_CHAR );
}

//----------------------------------------------------------------//
void MOAITextBox::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clearHighlights",		_clearHighlights },
		{ "getAlignment",			_getAlignment },
		{ "getGlyphScale",			_getGlyphScale },
		{ "getLineSpacing",			_getLineSpacing },
		{ "getRect",				_getRect },
		{ "getString",				_getString },
		{ "getStringBounds",		_getStringBounds },
		{ "getStyle",				_getStyle },
		{ "more",					_more },
		{ "nextPage",				_nextPage },
		{ "reserveCurves",			_reserveCurves },
		{ "revealAll",				_revealAll },
		{ "setAlignment",			_setAlignment },
		{ "setCurve",				_setCurve },
		{ "setGlyphScale",			_setGlyphScale },
		{ "setHighlight",			_setHighlight },
		{ "setLineSpacing",			_setLineSpacing },
		{ "setRect",				_setRect },
		{ "setReveal",				_setReveal },
		{ "setSnapToViewportScale",	_setSnapToViewportScale },
		{ "setSpeed",				_setSpeed },
		{ "setString",				_setString },
		{ "setStyle",				_setStyle },
		{ "setWordBreak",			_setWordBreak },
		{ "setYFlip",				_setYFlip },
		{ "spool",					_spool },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITextBox::ReserveCurves ( u32 total ) {

	this->ClearCurves ();
	
	this->mCurves.Init ( total );
	this->mCurves.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAITextBox::ResetLayout () {

	this->mMore = false;
	this->mLayout.Reset ();
}

//----------------------------------------------------------------//
void MOAITextBox::ScheduleLayout () {

	this->mNeedsLayout = true;
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAITextBox::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAITextBox::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAITextBox::SetCurve ( u32 idx, MOAIAnimCurve* curve ) {

	if ( idx > this->mCurves.Size ()) return;
	if ( this->mCurves [ idx ] == curve ) return;

	this->LuaRetain ( curve );
	this->LuaRelease ( this->mCurves [ idx ]);

	this->mCurves [ idx ] = curve;
}

//----------------------------------------------------------------//
void MOAITextBox::SetRect ( float left, float top, float right, float bottom ) {

	this->mFrame.Init ( left, top, right, bottom );
}

//----------------------------------------------------------------//
void MOAITextBox::SetText ( cc8* text ) {

	this->mText = text;
	
	this->mReveal = REVEAL_ALL;
	this->mSpool = 0.0f;
	
	this->mCurrentPageIdx = 0;
	this->mNextPageIdx = 0;
	
	this->mStyler.ResetStyleMap ();
	this->mLayout.ClearHighlights ();
}
