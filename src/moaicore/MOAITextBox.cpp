// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIQuadBrush.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAITextDesigner.h>
#include <moaicore/MOAITextBox.h>
#include <moaicore/MOAITextStyle.h>
#include <moaicore/MOAITextStyler.h>

#define DEFAULT_STYLE_NAME ""

// TODO: font garbage collection
//			- ref count glyphs
//			- glyph lifecycle
//			- glyph page defragmantation
// TODO: bitmap font ripper
// TODO: hit test for characters

//================================================================//
// MOAITextStyleRef
//================================================================//

//----------------------------------------------------------------//
MOAITextStyleRef::MOAITextStyleRef () :
	mStyle ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextStyleRef::~MOAITextStyleRef () {
}

//----------------------------------------------------------------//
bool MOAITextStyleRef::NeedsLayout () const {

	if ( this->mStyle ) {
		return this->mState.NeedsLayout ( *this->mStyle );
	}
	return false;
}

//----------------------------------------------------------------//
void MOAITextStyleRef::UpdateState () {

	if ( this->mStyle ) {
		this->mState = *this->mStyle;
	}
}

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
	
	self->ClearHighlights ();
	self->ScheduleLayout ();
	
	return 0;
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
	@text	Returns the two dimensional boundary of the text box.

	@in		MOAITextBox self
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAITextBox::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	USRect rect = self->mFrame;
	rect.Bless ();
	
	lua_pushnumber ( state, rect.mXMin );
	lua_pushnumber ( state, rect.mYMin );
	lua_pushnumber ( state, rect.mXMax );
	lua_pushnumber ( state, rect.mYMax );

	return 4;
}

//----------------------------------------------------------------//
/**	@name	getStringBounds
	@text	Returns the bounding rectange of a given substring on a
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
	
	USRect rect;
	if ( self->GetBoundsForRange ( index, size, rect )) {
		
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
	style = self->GetStyle ( styleName );

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
	@in		enum hAlignment				Can be one of LEFT_JUSTIFY, CENTER_JUSTIFY or RIGHT_JUSTIFY.
	@in		enum vAlignment				Can be one of LEFT_JUSTIFY, CENTER_JUSTIFY or RIGHT_JUSTIFY.
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
	
	@overload
		
		@in		MOAITextBox self
		@in		number index		Index of the first character in the substring.
		@in		number size			Length of the substring.
*/
int MOAITextBox::_setHighlight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UNN" )
	
	u32 index	= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 size	= state.GetValue < u32 >( 3, 0 );
	
	if ( size ) {
		
		if ( state.GetTop () > 3 ) {
			u32 rgba = state.GetColor32 ( 4, 1.0f, 1.0f, 1.0f, 1.0f );
			self->SetHighlight ( index, size, rgba );
		}
		else {
			self->SetHighlight ( index, size );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLineSpacing
	@text	Sets additional space between lines in text units. '0' uses
			the default spacing. Valus must be positive.

	@in		MOAITextBox self
	@in		number lineSpacing		Default value is 0.
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
	
	self->ResetStyleMap ();
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
		self->SetStyle ( styleName, style );
	}
	else {
	
		MOAITextStyle* style = state.GetLuaObject < MOAITextStyle >( 2, true );
		self->SetStyle ( style );
	}
	
	self->ResetStyleMap ();
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
	@in		number yFlip				Whether the vertical rendering direction should be inverted.
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
MOAITextStyle* MOAITextBox::AddAnonymousStyle ( MOAITextStyle* source ) {

	MOAITextStyle* style = new MOAITextStyle ();
	style->Init ( *source );
	
	MOAITextStyleRef styleRef;
	styleRef.mStyle = style;
	styleRef.UpdateState ();
	
	this->RetainStyle ( style );
	this->mAnonymousStyles.Push ( styleRef );
	
	return style;
}

//----------------------------------------------------------------//
void MOAITextBox::AddHighlight ( u32 base, u32 top, u32 color ) {

	// make room for the new highlight
	this->ClearHighlight ( base, top );

	MOAITextHighlight* highlight = new MOAITextHighlight ();
	
	highlight->mBase	= base;
	highlight->mTop		= top;
	highlight->mColor	= color;
	highlight->mPrev	= 0;
	highlight->mNext	= 0;
	
	if ( !this->mHighlights ) {
		this->mHighlights = highlight;
		return;
	}
	
	MOAITextHighlight* best = 0;
	MOAITextHighlight* cursor = this->mHighlights;
	for ( ; cursor; cursor = cursor->mNext ) {
		if ( cursor->mBase >= base ) break;
		best = cursor;
	}
	
	if ( best ) {
	
		highlight->mNext = best->mNext;
		highlight->mPrev = best;
	
		if ( best->mNext ) {
			best->mNext->mPrev = highlight;
		}
		best->mNext = highlight;
	}
	else {
	
		highlight->mNext = this->mHighlights;
		this->mHighlights = highlight;
		
		if ( highlight->mNext ) {
			highlight->mNext->mPrev = highlight;
		}
	}
}

//----------------------------------------------------------------//
void MOAITextBox::ApplyHighlights () {

	u32 totalSprites = this->mSprites.GetTop ();
	u32 spriteIdx = 0;
	MOAITextHighlight* highlight = this->mHighlights;
	
	while (( spriteIdx < totalSprites ) && highlight ) {
		MOAITextSprite& sprite = this->mSprites [ spriteIdx ];
		
		if ( sprite.mIdx >= highlight->mTop ) {
			highlight = highlight->mNext;
		}
		else {
		
			if ( sprite.mIdx >= highlight->mBase ) {
				sprite.mRGBA = highlight->mColor;
				sprite.mMask |= MOAITextSprite::MASK_COLOR;
			}
			spriteIdx++;
		}
	}
}

//----------------------------------------------------------------//
void MOAITextBox::ClearHighlight ( u32 base, u32 top ) {

	MOAITextHighlight* highlight = this->mHighlights;
	for ( ; highlight; highlight = highlight->mNext ) {
		
		if ( highlight->mTop <= base ) continue;
		if ( highlight->mBase >= top ) continue;
		
		// cursor is inside of span
		if (( highlight->mBase >= base ) && ( highlight->mTop <= top )) {
			highlight->mBase = highlight->mTop;
			continue;
		}
		
		// cursor contains span
		if (( highlight->mBase < base ) && ( highlight->mTop > top )) {
			
			MOAITextHighlight* split = new MOAITextHighlight ();
			
			split->mColor	= highlight->mColor;
			split->mBase	= top;
			split->mTop		= highlight->mTop;
			
			split->mPrev = highlight;
			split->mNext = highlight->mNext;
			
			highlight->mTop = base;
			
			if ( highlight->mNext ) {
				highlight->mNext->mPrev = split;
			}
			highlight->mNext = split;
			break;
		}
		
		if (( highlight->mBase < base ) && ( base < highlight->mTop )) {
			highlight->mTop = base;
			continue;
		}
		
		if (( highlight->mBase < top ) && ( top < highlight->mTop )) {
			highlight->mBase = top;
			continue;
		}
	}
}

//----------------------------------------------------------------//
void MOAITextBox::ClearHighlights () {

	MOAITextHighlight* cursor = this->mHighlights;
	this->mHighlights = 0;
	
	while ( cursor ) {
		MOAITextHighlight* highlight = cursor;
		cursor = cursor->mNext;
		delete highlight;
	}
}

//----------------------------------------------------------------//
bool MOAITextBox::CheckStylesChanged () {

	bool status = false;

	// TODO: think about keeping list of currently active styles instead of iterating through everything
	
	u32 totalAnonymous = this->mAnonymousStyles.GetTop ();
	for ( u32 i = 0; i < totalAnonymous; i++ ) {
		MOAITextStyleRef& styleRef = this->mAnonymousStyles [ i ];
		if ( styleRef.NeedsLayout ()) {
			styleRef.UpdateState ();
			status = true;
		}
	}

	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		MOAITextStyleRef& styleRef = styleSetIt->second;
		if ( styleRef.NeedsLayout ()) {
			styleRef.UpdateState ();
			status = true;
		}
	}

	return status;
}

//----------------------------------------------------------------//
void MOAITextBox::ClearCurves () {

	for ( u32 i = 0; i < this->mCurves.Size (); ++i ) {
		this->LuaRelease ( this->mCurves [ i ]);
	}
	this->mCurves.Clear ();
}

//----------------------------------------------------------------//
void MOAITextBox::CompactHighlights () {

	MOAITextHighlight* cursor = this->mHighlights;
	while ( cursor ) {
	
		MOAITextHighlight* highlight = cursor;
		cursor = highlight->mNext;
	
		// prune empty highlights
		if ( highlight->mBase == highlight->mTop ) {
			this->RemoveHighlight ( *highlight );
			delete highlight;
			continue;
		}
		
		// gobble up any adjacent (or overlapping) same-colored highlights
		// break if we hit a gap or another color
		MOAITextHighlight* cursor2 = highlight->mNext;
		while ( cursor2 ) {
			MOAITextHighlight* highlight2 = cursor2;
			cursor2 = highlight2->mNext;
			
			if ( highlight2->mBase != highlight2->mTop ) {
			
				if ( highlight->mColor != highlight2->mColor ) break;
				if ( highlight->mTop < highlight2->mBase ) break;
				
				if ( highlight->mTop < highlight2->mTop ) {
					highlight->mTop = highlight2->mTop;
				}
			}
			
			this->RemoveHighlight ( *highlight2 );
			delete highlight2;
		}
		
		cursor = highlight->mNext;
	}
}

//----------------------------------------------------------------//
void MOAITextBox::Draw ( int subPrimID ) {
	UNUSED ( subPrimID ); 
	
	if ( !( this->mFlags & FLAGS_VISIBLE )) return;
	
	if ( this->mReveal ) {
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

		if ( this->mUVTransform ) {
			USAffine3D uvMtx = this->mUVTransform->GetLocalToWorldMtx ();
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
		
		MOAIQuadBrush::BindVertexFormat ( gfxDevice );

		USColorVec baseColor = gfxDevice.GetPenColor ();
		USColorVec blendColor;
		u32 rgba0 = 0xffffffff;
		u32 rgba1 = 0xffffffff;

		u32 size = this->mSprites.GetTop ();
		for ( u32 i = 0; ( i < size ) && ( i < this->mReveal ); ++i ) {
			const MOAITextSprite& sprite = this->mSprites [ i ];
			
			rgba0 = sprite.mMask & MOAITextSprite::MASK_COLOR ? sprite.mRGBA : sprite.mStyle->mColor;
			
			if ( rgba0 != rgba1 ) {
				rgba1 = rgba0;
				
				blendColor.SetRGBA ( rgba0 );
				blendColor.Modulate ( baseColor );
				gfxDevice.SetPenColor ( blendColor );
			}
			sprite.mGlyph->Draw ( *sprite.mTexture, sprite.mX, sprite.mY, sprite.mScale );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextBox::DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID ); 

	this->Layout ();

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
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX_BASELINES )) {
		
		u32 totalLines = this->mLines.GetTop ();
		for ( u32 i = 0; i < totalLines; ++i ) {
			MOAITextLine& line = this->mLines [ i ];
			float y = line.mRect.mYMin + line.mAscent;
			draw.DrawLine ( line.mRect.mXMin, y, line.mRect.mXMax, y );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX_LAYOUT )) {
		
		u32 totalLines = this->mLines.GetTop ();
		for ( u32 i = 0; i < totalLines; ++i ) {
			MOAITextLine& line = this->mLines [ i ];
			draw.DrawRectOutline ( line.mRect );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextBox::FindSpriteSpan ( u32 idx, u32 size, u32& spanIdx, u32& spanSize ) {

	spanSize = 0;

	u32 top = this->mSprites.GetTop ();
	if ( !top ) return;
	if ( this->mSprites [ 0 ].mIdx >= ( idx + size )) return;
	if ( this->mSprites [ top - 1 ].mIdx < idx ) return;
	
	for ( u32 i = 0; i < top; ++i ) {
		MOAITextSprite& sprite = this->mSprites [ i ];
	
		if ( sprite.mIdx >= idx ) {
			spanIdx = i;
			spanSize = 1;
			break;
		}
	}
	
	if ( spanSize ) {
		
		u32 max = idx + size;
		u32 i = spanIdx + 1;
		u32 count = 0;
		
		for ( ; i < top; ++i, ++count ) {
			MOAITextSprite& sprite = this->mSprites [ i ];
			if ( sprite.mIdx >= max ) break;
		}
		spanSize = count + 1;
	}
}

//----------------------------------------------------------------//
bool MOAITextBox::GetBoundsForRange ( u32 idx, u32 size, USRect& rect ) {

	if ( !size ) return false;
	this->Layout ();

	bool result = false;

	u32 spanIdx;
	u32 spanSize;
	
	this->FindSpriteSpan ( idx, size, spanIdx, spanSize );
	if ( !spanSize ) return false;

	u32 end = spanIdx + spanSize;
	for ( u32 i = spanIdx; i < end; ++i ) {
		MOAITextSprite& sprite = this->mSprites [ i ];
		MOAIGlyph& glyph = *sprite.mGlyph;

		if ( glyph.mWidth > 0.0f ) {

			USRect glyphRect = glyph.GetRect ( sprite.mX, sprite.mY );

			if ( result ) {
				rect.Grow ( glyphRect );
			}
			else {
				rect = glyphRect;
				result = true;
			}
		}
	}
	return result;
}

//----------------------------------------------------------------//
u32 MOAITextBox::GetPropBounds ( USBox& bounds ) {

	bounds.Init ( this->mFrame.mXMin, this->mFrame.mYMax, this->mFrame.mXMax, this->mFrame.mYMin, 0.0f, 0.0f );
	return MOAIProp::BOUNDS_OK;
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextBox::GetStyle () {

	return this->GetStyle ( DEFAULT_STYLE_NAME );
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextBox::GetStyle ( cc8* styleName ) {

	if ( styleName && this->mStyleSet.contains ( styleName )) {
		return this->mStyleSet [ styleName ].mStyle;
	}
	return this->mStyleSet [ DEFAULT_STYLE_NAME ].mStyle;
}

//----------------------------------------------------------------//
bool MOAITextBox::IsDone () {

	if ( this->IsActive ()) {
		this->Layout ();
		return ( this->mReveal >= this->mSprites.GetTop ());
	}
	return true;
}

//----------------------------------------------------------------//
void MOAITextBox::Layout () {

	if ( !this->mText ) {
		this->ResetStyleMap ();
		this->ResetLayout ();
	}
	else if ( this->mNeedsLayout ) {
		
		if ( !this->mStyleMap.GetTop ()) {
			MOAITextStyler styler;
			styler.BuildStyleMap ( *this );
		}
		
		this->ResetLayout ();
		
		MOAITextDesigner designer;
		designer.Init ( *this );
		designer.BuildLayout ();
	
		this->ApplyHighlights ();
	}
	
	this->mNeedsLayout = false;
}

//----------------------------------------------------------------//
MOAITextBox::MOAITextBox () :
	mLineSpacing ( 0.0f ),
	mText ( "" ),
	mTextLength ( 0 ),
	mHAlign ( LEFT_JUSTIFY ),
	mVAlign ( LEFT_JUSTIFY ),
	mSpool ( 0.0f ),
	mSpeed ( DEFAULT_SPOOL_SPEED ),
	mReveal ( REVEAL_ALL ),
	mYFlip ( false ),
	mGlyphScale ( 1.0f ),
	mCurrentPageIdx ( 0 ),
	mNextPageIdx ( 0 ),
	mNeedsLayout ( false ),
	mMore ( false ),
	mHighlights ( 0 ),
	mWordBreak ( WORD_BREAK_NONE ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f ); 
	this->SetMask ( MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
	this->mBlendMode.SetBlend ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

//----------------------------------------------------------------//
MOAITextBox::~MOAITextBox () {

	this->ClearCurves ();
	this->ClearHighlights ();
	
	// TODO: this is a known bug - releasing the dep links here
	// will cause plenty o' crashing
	// the case seems to be when the text box has ben garbage
	// collected but is still in the node manager's update list
	// the lua ref stuff is destroyed by the __gc method
	// but it's needed by the links
	// am wondering if the links are also being orphaned or
	// compromised by the gc
	
	this->ResetLayout ();
	this->ResetStyleMap ();
	this->ResetStyleSet ();
}

//----------------------------------------------------------------//
bool MOAITextBox::More () {
	
	this->Layout ();
	
	if ( this->mReveal < this->mSprites.GetTop ()) {
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

	if ( this->CheckStylesChanged ()) {
		this->mNeedsLayout = true;
		this->RefreshStyleGlyphs ();
	}
	this->Layout ();

	if ( this->mYFlip ) {
		
		USAffine3D mtx;
		
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
void MOAITextBox::PushLine ( u32 start, u32 size, const USRect& rect, float ascent ) {

	MOAITextLine textLine;
	
	textLine.mStart			= start;
	textLine.mSize			= size;
	textLine.mRect			= rect;
	textLine.mAscent		= ascent;
	
	this->mLines.Push ( textLine );
}

//----------------------------------------------------------------//
void MOAITextBox::PushSprite ( u32 idx, MOAIGlyph& glyph, MOAITextStyle& style, float x, float y, float scale ) {
	
	MOAITextSprite textSprite;
	
	textSprite.mIdx			= idx;
	textSprite.mGlyph		= &glyph;
	textSprite.mStyle		= &style;
	textSprite.mX			= x;
	textSprite.mY			= y;
	textSprite.mScale		= scale;
	
	textSprite.mRGBA		= style.mColor;
	textSprite.mTexture		= style.mFont->GetGlyphTexture ( glyph );
	textSprite.mMask		= 0;

	this->mSprites.Push ( textSprite );
}

//----------------------------------------------------------------//
void MOAITextBox::PushStyleSpan ( int base, int top, MOAITextStyle& style ) {

	MOAITextStyleSpan span;
	
	span.mBase		= base;
	span.mTop		= top;
	span.mStyle		= &style;

	this->mStyleMap.Push ( span );
}

//----------------------------------------------------------------//
void MOAITextBox::RefreshStyleGlyphs () {

	u32 totalSpans = this->mStyleMap.GetTop ();
	if ( !totalSpans ) return;
	
	for ( u32 i = 0; i < totalSpans; ++i ) {
		MOAITextStyleSpan& span = this->mStyleMap [ i ];
		
		int idx = span.mBase;
		while ( idx < span.mTop ) {
			u32 c = u8_nextchar ( this->mText, &idx );
			span.mStyle->AffirmGlyph ( c );
		}
	}
	
	// TODO: think about keeping list of currently active styles instead of iterating through everything
	
	u32 totalAnonymous = this->mAnonymousStyles.GetTop ();
	for ( u32 i = 0; i < totalAnonymous; i++ ) {
		MOAITextStyleRef& styleRef = this->mAnonymousStyles [ i ];
		styleRef.mStyle->mFont->ProcessGlyphs ();
	}

	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		MOAITextStyleRef& styleRef = styleSetIt->second;
		styleRef.mStyle->mFont->ProcessGlyphs ();
	}
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
		{ "getGlyphScale",			_getGlyphScale },
		{ "getLineSpacing",			_getLineSpacing },
		{ "getRect",				_getRect },
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
void MOAITextBox::ReleaseStyle ( MOAITextStyle* style ) {

	if ( style ) {
		this->ClearNodeLink ( *style );
	}
	this->LuaRelease ( style );
}

//----------------------------------------------------------------//
void MOAITextBox::RemoveHighlight ( MOAITextHighlight& highlight ) {

	MOAITextHighlight* prev = highlight.mPrev;
	MOAITextHighlight* next = highlight.mNext;
	
	if ( prev ) {
		prev->mNext = next;
	}
	else {
		this->mHighlights = next;	
	}
	
	if ( next ) {
		next->mPrev = prev;
	}
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
	this->mLines.Reset ();
	this->mSprites.Reset ();
}

//----------------------------------------------------------------//
void MOAITextBox::ResetHighlights () {

	u32 top = this->mSprites.GetTop ();
	for ( u32 i = 0; i > top; ++i ) {
		MOAITextSprite& sprite = this->mSprites [ i ];
		sprite.mMask = sprite.mMask & ~MOAITextSprite::MASK_COLOR;
	}
}

//----------------------------------------------------------------//
void MOAITextBox::ResetStyleMap () {

	u32 totalAnonymous = this->mAnonymousStyles.GetTop ();
	for ( u32 i = 0; i < totalAnonymous; i++ ) {
		this->ReleaseStyle ( this->mAnonymousStyles [ i ].mStyle );
	}
	this->mAnonymousStyles.Reset ();
	this->mStyleMap.Reset ();
}

//----------------------------------------------------------------//
void MOAITextBox::ResetStyleSet () {
	
	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		this->ReleaseStyle ( styleSetIt->second.mStyle );
	}
	this->mStyleSet.clear ();
}

//----------------------------------------------------------------//
void MOAITextBox::RetainStyle ( MOAITextStyle* style ) {

	if ( style ) {
		this->SetNodeLink ( *style );
	}
	this->LuaRetain ( style );
}

//----------------------------------------------------------------//
void MOAITextBox::ScheduleLayout () {

	this->mNeedsLayout = true;
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAITextBox::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIProp::SerializeIn ( state, serializer );
	MOAIAction::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAITextBox::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIProp::SerializeOut ( state, serializer );
	MOAIAction::SerializeOut ( state, serializer );
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
void MOAITextBox::SetHighlight ( u32 idx, u32 size ) {

	if ( !size ) return;

	this->ClearHighlight ( idx, idx + size );
	this->CompactHighlights ();
	this->ResetHighlights ();
	this->ApplyHighlights ();
}

//----------------------------------------------------------------//
void MOAITextBox::SetHighlight ( u32 idx, u32 size, u32 color ) {

	if ( !size ) return;

	this->AddHighlight ( idx, idx + size, color );
	this->CompactHighlights ();
	this->ResetHighlights ();
	this->ApplyHighlights ();
}

//----------------------------------------------------------------//
void MOAITextBox::SetRect ( float left, float top, float right, float bottom ) {

	this->mFrame.Init ( left, top, right, bottom );
}

//----------------------------------------------------------------//
void MOAITextBox::SetStyle ( MOAITextStyle* style ) {

	this->SetStyle ( DEFAULT_STYLE_NAME, style );
}

//----------------------------------------------------------------//
void MOAITextBox::SetStyle ( cc8* styleName, MOAITextStyle* style ) {

	styleName = styleName ? styleName : DEFAULT_STYLE_NAME;
		
	MOAITextStyle* prevStyle = 0;
	
	if ( this->mStyleSet.contains ( styleName )) {
		prevStyle = this->mStyleSet [ styleName ].mStyle;
		if ( prevStyle == style ) return;
	}
	
	this->RetainStyle ( style );
	this->ReleaseStyle ( prevStyle );
	
	if ( style ) {
		MOAITextStyleRef& styleRef = this->mStyleSet [ styleName ];
		styleRef.mStyle = style;
		styleRef.UpdateState ();
	}
	else {
		if ( this->mStyleSet.contains ( styleName )) {
			this->mStyleSet.erase ( styleName );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextBox::SetText ( cc8* text ) {

	this->mText = text;
	this->mTextLength = ( u32 )this->mText.length ();
	
	this->mReveal = REVEAL_ALL;
	this->mSpool = 0.0f;
	
	this->mCurrentPageIdx = 0;
	this->mNextPageIdx = 0;
	
	this->ResetStyleMap ();
	this->ClearHighlights ();
}
