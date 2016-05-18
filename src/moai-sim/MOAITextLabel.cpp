// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAINodeMgr.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAITextLayoutRules.h>
#include <moai-sim/MOAITextLabel.h>
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
/**	@lua	clearHighlights
	@text	Removes all highlights currently associated with the text box.

	@in		MOAITextLabel self
	@out	nil
*/
int MOAITextLabel::_clearHighlights ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	self->mLayout.ClearHighlights ();
	self->ScheduleLayout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getAlignment
	@text	Returns the alignment of the text

	@in		MOAITextLabel self
	@out	number hAlign			horizontal alignment
	@out	number vAlign			vertical alignment
*/
int MOAITextLabel::_getAlignment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	state.Push ( self->mLayoutRules.GetHAlign ());
	state.Push ( self->mLayoutRules.GetVAlign ());
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getGlyphScale
	@text	Returns the current glyph scale.

	@in		MOAITextLabel self
	@out	number glyphScale
*/
int MOAITextLabel::_getGlyphScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	state.Push ( self->mLayoutRules.GetGlyphScale ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getLineSpacing
	@text	Returns the spacing between lines (in pixels).

	@in		MOAITextLabel self
	@out	number lineScale		The size of the spacing in pixels.
*/
int MOAITextLabel::_getLineSpacing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	lua_pushnumber ( state, self->mLayoutRules.GetLineSpacing ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getOverrunRules
	@text	Returns the overrun rules.

	@in		MOAITextLabel self
	@out	number firstOverrunRule
	@out	number overrunRule
*/
int MOAITextLabel::_getOverrunRules ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	state.Push ( self->mLayoutRules.GetFirstOverrunRule ());
	state.Push ( self->mLayoutRules.GetOverrunRule ());
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getRect
	@text	Returns the two-dimensional boundary of the text box (if any).

	@in		MOAITextLabel self
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAITextLabel::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	ZLRect rect = self->mLayoutRules.GetFrame ();
	state.Push ( rect );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getSizingRules
	@text	Returns the sizing rules.

	@in		MOAITextLabel self
	@out	number hLayoutSizingRule
	@out	number vLayoutSizingRule
	@out	number lineSizingRule
*/
int MOAITextLabel::_getSizingRules ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	state.Push ( self->mLayoutRules.GetHLayoutSizingRule ());
	state.Push ( self->mLayoutRules.GetVLayoutSizingRule ());
	state.Push ( self->mLayoutRules.GetLineSizingRule ());
	
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getStyle
	@text	Returns the style associated with a name or, if no name
			is given, returns the default style.

	@overload
	
		@in		MOAITextLabel self
		@out	MOAITextStyle defaultStyle
	
	@overload
	
		@in		MOAITextLabel self
		@in		string styleName
		@out	MOAITextStyle style
*/
int MOAITextLabel::_getStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	MOAITextStyle* style = 0;

	cc8* styleName = state.GetValue < cc8* >( 2, 0 );
	style = self->mStyleCache.GetStyle ( styleName );

	if ( style ) {
		style->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getText
	@text	Return the text string.

	@in		MOAITextLabel self
	@out	string text				Text string.
*/
int MOAITextLabel::_getText ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	if ( self->mText ) {
		lua_pushstring ( state, self->mText );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getTextBounds
	@text	Returns the bounding rectange of a given substring on a
			single line in the local space of the text box. If 'index' and
			'size' are not passed, the bounds for all visible text will be
			returned.

	@in		MOAITextLabel self
	@opt	number index		Index of the first character in the substring.
	@opt	number size			Length of the substring.
	@out	number xMin			Edge of rect or 'nil' is no match found.
	@out	number yMin			Edge of rect or 'nil' is no match found.
	@out	number xMax			Edge of rect or 'nil' is no match found.
	@out	number yMax			Edge of rect or 'nil' is no match found.
*/
int MOAITextLabel::_getTextBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	ZLRect rect;
	bool hasRect = false;
	
	self->Refresh ();
	
	if ( state.CheckParams ( 2, "NN", false )) {
	
		u32 index	= state.GetValue < u32 >( 2, 1 ) - 1;
		u32 size	= state.GetValue < u32 >( 3, 0 );
		
		hasRect = self->mLayout.GetBoundsForRange ( index, size, rect );
	}
	else {
		rect = self->mLayout.GetLayoutBounds ();
		hasRect = true;
	}
	
	if ( hasRect ) {
	
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
/**	@name	hasOverrun
    @text	Returns whether a token was truncated at the end of the text layout.
 
    @in		MOAITextBox self
    @out	boolean overrun			Only true if a token is split across layout pages.
 */
int MOAITextLabel::_hasOverrun ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	bool overrun = self->mOverrun;
	state.Push ( overrun );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	more
	@text	Returns whether there are additional pages of text below the cursor position that are not visible on the screen.

	@in		MOAITextLabel self
	@out	boolean isMore				If there is additional text below the cursor that is not visible on the screen due to clipping.
*/
int MOAITextLabel::_more ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	lua_pushboolean ( L, self->mMore );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	nextPage
	@text	Advances to the next page of text (if any) or wraps to the start of the text (if at end).

	@in		MOAITextLabel self
	@opt	boolean reveal		Default is true
	@out	nil
*/
int MOAITextLabel::_nextPage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	bool reveal = state.GetValue < bool >( 2, true );
	self->NextPage ( reveal );
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveCurves
	@text	Reserves a set of IDs for animation curves to be binding to this text object.  See setCurves.

	@in		MOAITextLabel self
	@in		number nCurves
	@out	nil
*/
int MOAITextLabel::_reserveCurves ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mLayoutRules.ReserveCurves ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	revealAll
	@text	Displays as much text as will fit in the text box.

	@in		MOAITextLabel self
	@out	nil
*/
int MOAITextLabel::_revealAll ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	self->mReveal = REVEAL_ALL;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setAlignment
	@text	Sets the horizontal and/or vertical alignment of the text in the text box.

	@in		MOAITextLabel self
	@in		number hAlignment				Can be one of LEFT_JUSTIFY, CENTER_JUSTIFY or RIGHT_JUSTIFY.
	@in		number vAlignment				Can be one of TOP_JUSTIFY, CENTER_JUSTIFY, BOTTOM_JUSTIFY or BASELINE_JUSTIFY.
	@out	nil
*/
int MOAITextLabel::_setAlignment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "UN" )

	self->mLayoutRules.SetHAlign ( state.GetValue < u32 >( 2, MOAITextLayoutRules::LEFT_JUSTIFY ));
	self->mLayoutRules.SetVAlign ( state.GetValue < u32 >( 3, MOAITextLayoutRules::TOP_JUSTIFY ));
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setAutoFlip
	@text	When autoflip is enabled, the label will be evaluated in
			screen space during rendering, and flipped vertically to
			remain 'upright' in the view.

	@in		MOAITextLabel self
	@in		boolean autoflip		Default value is false.
	@out	nil
*/
int MOAITextLabel::_setAutoFlip ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	self->mAutoFlip = state.GetValue < bool >( 2, false );
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBounds
	@text	Sets or removes a bounding rectangle for the text, specified
			as the XY planes of the given bounding box. Toggles
			the rect limits accordingly.

	@overload	Set a the text label's bounding rectagle and enable the limits.

		@in		MOAITextLabel self
		@in		number xMin
		@in		number yMin
		@in		number zMin
		@in		number xMax
		@in		number yMax
		@in		number zMax
		@out	nil
	
	@overload	Clear the text label's bounding rectangle and disable the limits.

		@in		MOAITextLabel self
		@out	nil
*/
int MOAITextLabel::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	if ( state.CheckParams ( 2, "NNNNNN", false )) {

		ZLBox bounds = state.GetBox ( 2 );
		bounds.Bless ();
		
		ZLRect frame;
		frame = bounds.GetRect ( ZLBox::PLANE_XY );
		
		self->mLayoutRules.SetFrame ( frame );
		self->mLayoutRules.SetLimitWidth ( true );
		self->mLayoutRules.SetLimitHeight ( true );
	}
	else {
		self->mLayoutRules.SetLimitWidth ( false );
		self->mLayoutRules.SetLimitHeight ( false );
	}
	
	self->ScheduleLayout ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCurve
	@text	Binds an animation curve to the text, where the Y value of the curve indicates the text offset, or clears the curves.

	@overload

		@in		MOAITextLabel self
		@in		number curveID				The ID of the curve within this text object.
		@in		MOAIAnimCurve curve			The MOAIAnimCurve to bind to.
		@out	nil
	
	@overload
		
		@in		MOAITextLabel self
		@out	nil
*/
int MOAITextLabel::_setCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	if ( state.GetTop () > 1 ) {

		u32 index = state.GetValue < u32 >( 2, 1 ) - 1;
	
		MOAIAnimCurve* curve = state.GetLuaObject < MOAIAnimCurve >( 3, true );
		if ( !curve ) return 0;

		self->mLayoutRules.SetCurve ( index, curve );
		self->ScheduleLayout ();
	}
	else {
		self->mLayoutRules.ClearCurves ();
		self->ScheduleLayout ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGlyphScale
	@text	Sets the glyph scale. This is a scalar applied to glyphs
			as they are positioned in the text box.

	@in		MOAITextLabel self
	@opt	number glyphScale		Default value is 1.
	@out	number glyphScale
*/
int MOAITextLabel::_setGlyphScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	self->mLayoutRules.SetGlyphScale ( state.GetValue < float >( 2, 1.0f ));
	self->ScheduleLayout ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setHighlight
	@text	Set or clear the highlight color of a sub string in the text.
			Only affects text displayed on the current page. Highlight
			will automatically clear when layout or page changes.

	@overload

		@in		MOAITextLabel self
		@in		number index		Index of the first character in the substring.
		@in		number size			Length of the substring.
		@in		number r
		@in		number g
		@in		number b
		@opt	number a			Default value is 1.
		@out	nil
	
	@overload
		
		@in		MOAITextLabel self
		@in		number index		Index of the first character in the substring.
		@in		number size			Length of the substring.
		@out	nil
*/
int MOAITextLabel::_setHighlight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "UNN" )
	
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
/**	@lua	setLineSnap
	@text	Sets the snapping boundary for lines of text. Only applied during layout and in the
			text label's local space.

	@in		MOAITextLabel self
	@opt	number hLineSnap
	@opt	number vLineSnap			Default value is hLineSnap.
	@out	nil
*/
int MOAITextLabel::_setLineSnap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	float hLineSnap = state.GetValue < float >( 2, 0.0f );
	float vLineSnap = state.GetValue < float >( 3, hLineSnap );
	
	self->mLayoutRules.SetHLineSnap ( hLineSnap );
	self->mLayoutRules.SetVLineSnap ( vLineSnap );
	self->ScheduleLayout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setLineSpacing
	@text	Sets additional space between lines in text units. '0' uses
			the default spacing.

	@in		MOAITextLabel self
	@in		number lineSpacing		Default value is 0.
	@out	nil
*/
int MOAITextLabel::_setLineSpacing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )
	
	float lineSpacing = state.GetValue < float >( 2, 0.0f );
	
	self->mLayoutRules.SetLineSpacing ( lineSpacing );
	self->ScheduleLayout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setOverrunRule
	@text	Control behavior of text shaper when a token needs to be wrapped.
			An alternate rule may be set for the first token on a line.

			OVERRUN_MOVE_WORD will cause the entire token to be moved to the next line.
			OVERRUN_SPLIT_WORD will split the token across lines.
			OVERRUN_TRUNCATE_WORD will discard the remaining characters in the token.
			OVERRUN_ABORT_LAYOUT will back up to the start of the token and stop the layout.
			
			OVERRUN_MOVE_WORD is not permitted as the rule for the line's first token. If the
			first token doesn't fit on current line, it will not fit on the next line and so on.

	@in		MOAITextLabel self
	@in		number firstOverrunRule		One of OVERRUN_SPLIT_WORD, OVERRUN_TRUNCATE_WORD, OVERRUN_ABORT_LAYOUT. Default value is OVERRUN_SPLIT_WORD.
	@in		number overrunRule			One of OVERRUN_MOVE_WORD, OVERRUN_SPLIT_WORD, OVERRUN_TRUNCATE_WORD, OVERRUN_ABORT_LAYOUT. Default value is OVERRUN_MOVE_WORD.
	@out	nil
*/
int MOAITextLabel::_setOverrunRules ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	self->mLayoutRules.SetFirstOverrunRule ( state.GetValue < u32 >( 2, MOAITextLayoutRules::OVERRUN_SPLIT_WORD ));
	self->mLayoutRules.SetOverrunRule ( state.GetValue < u32 >( 3, MOAITextLayoutRules::OVERRUN_MOVE_WORD ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Sets the rectangular area for this text box.

	@in		MOAITextLabel self
	@in		number x1					The X coordinate of the rect's upper-left point.
	@in		number y1					The Y coordinate of the rect's upper-left point.
	@in		number x2					The X coordinate of the rect's lower-right point.
	@in		number y2					The Y coordinate of the rect's lower-right point.
	@out	nil
*/
int MOAITextLabel::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "UNNNN" )

	ZLRect rect = state.GetRect < float >( 2 );
	rect.Bless ();
	self->mLayoutRules.SetFrame ( rect );

	self->mLayoutRules.SetLimitWidth ( true );
	self->mLayoutRules.SetLimitHeight ( true );

	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRectLimits
	@text	Toggles width/height constraints based on the rect.

	@in		MOAITextLabel self
	@opt	boolean limitWidth		Limit text to the rect's width. Default value is 'false'.
	@opt	boolean limitHeight		Limit text to the rect's height. Default value is 'false'.
	@out	nil
*/
int MOAITextLabel::_setRectLimits ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	self->mLayoutRules.SetLimitWidth ( state.GetValue < bool >( 2, false ));
	self->mLayoutRules.SetLimitHeight ( state.GetValue < bool >( 3, false ));

	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setReveal
	@text	Sets the number of renderable characters to be shown. 
			Can range from 0 to any value; values greater than the
			number of renderable characters in the current text will
			be ignored.

	@in		MOAITextLabel self
	@in		number reveal				The number of renderable characters (i.e. not whitespace) to be shown.
	@out	nil
*/
int MOAITextLabel::_setReveal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "UN" )

	self->mReveal = state.GetValue < u32 >( 2, self->mReveal );
	self->mSpool = ( float )self->mReveal;

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSizingRule
	@text	The sizing rules are used to determine the boundaries of each line for
			layout and alignment purposes. The H and V layout sizing rules determine whether
			the layout is based on the tight-fitting glyph boundaries or the (possibly) looser
			fitting 'logical' bounds (i.e. the bounds specified by the type face, allowing for
			full ascent and descent). The line sizing rule determines whether lines are spaced
			from logical or glyph bounds. Note that these rules do not affect the spacing of
			glyphs as they are laid out within a line, but they do affect the spacing of lines.

	@in		MOAITextLabel self
	@in		number hLayoutSizingRule		One of LOGICAL_SIZE or GLYPH_SIZE. Default is LOGICAL_SIZE.
	@in		number vLayoutSizingRule		One of LOGICAL_SIZE or GLYPH_SIZE. Default is LOGICAL_SIZE.
	@in		number lineSizingRule			One of LOGICAL_SIZE or GLYPH_SIZE. Default is LOGICAL_SIZE.
	@out	nil
*/
int MOAITextLabel::_setSizingRules ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	self->mLayoutRules.SetHLayoutSizingRule ( state.GetValue < u32 >( 2, MOAITextLayoutRules::LOGICAL_SIZE ));
	self->mLayoutRules.SetVLayoutSizingRule ( state.GetValue < u32 >( 3, MOAITextLayoutRules::LOGICAL_SIZE ));
	self->mLayoutRules.SetLineSizingRule ( state.GetValue < u32 >( 3, MOAITextLayoutRules::LOGICAL_SIZE ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSpeed
	@text	Sets the base spool speed used when creating a spooling MOAIAction with the spool() function.

	@in		MOAITextLabel self
	@in		number speed		The base spooling speed.
	@out	nil
*/
int MOAITextLabel::_setSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "UN" )
	
	self->mSpeed = state.GetValue < float >( 2, self->mSpeed );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setText
	@text	Sets the text string to be displayed by this textbox.

	@in		MOAITextLabel self
	@in		string newStr				The new text string to be displayed.
	@out	nil
*/
int MOAITextLabel::_setText ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "US" )

	cc8* text = state.GetValue < cc8* >( 2, "" );
	self->SetText ( text );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setStyle
	@text	Attaches a style to the textbox and associates a name with it.
			If no name is given, sets the default style.

	@overload
	
		@in		MOAITextLabel self
		@in		MOAITextStyle defaultStyle
		@out	nil
	
	@overload
	
		@in		MOAITextLabel self
		@in		string styleName
		@in		MOAITextStyle style
		@out	nil
*/
int MOAITextLabel::_setStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	cc8* styleName = state.GetValue < cc8* >( 2, "" );
	
	if ( strlen ( styleName )) {
	
		MOAITextStyle* style = state.GetLuaObject < MOAITextStyle >( 3, true );
		self->mStyleCache.SetStyle ( styleName, style );
	}
	else {
	
		MOAITextStyle* style = state.GetLuaObject < MOAITextStyle >( 2, true );
		self->mStyleCache.SetStyle ( style );
	}
	
	self->ScheduleLayout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setWordBreak
	@text	Sets the rule for breaking words across lines.

	@in		MOAITextLabel self
	@opt	number rule				One of MOAITextLabel.WORD_BREAK_NONE, MOAITextLabel.WORD_BREAK_CHAR.
									Default is MOAITextLabel.WORD_BREAK_NONE.
	@out	nil
*/
int MOAITextLabel::_setWordBreak ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	//self->mLayoutRules.SetWordBreakRule ( state.GetValue < u32 >( 2, MOAITextLayoutRules::WORD_BREAK_NONE ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setYFlip
	@text	Sets the rendering direction for the text. Default assumes
			a window style screen space (positive Y moves down the screen). Set
			to true to render text for world style coordinate systems (positive
			Y moves up the screen).

	@in		MOAITextLabel self
	@in		boolean yFlip				Whether the vertical rendering direction should be inverted.
	@out	nil
*/
int MOAITextLabel::_setYFlip ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "UB" )

	self->mLayoutRules.SetYFlip ( state.GetValue < bool >( 2, false ));
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	spool
	@text	Creates a new MOAIAction which when run has the effect of increasing
			the amount of characters revealed from 0 to the length of the string
			currently set.  The spool action is automatically added to the root
			of the action tree, but may be reparented or stopped by the developer.
			This function also automatically sets the current number of revealed
			characters to 0 (i.e. MOAITextLabel:setReveal(0)).

	@in		MOAITextLabel self
	@out	MOAIAction action			The new MOAIAction which spools the text when run.
*/
int MOAITextLabel::_spool ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	self->mReveal = state.GetValue < u32 >( 2, 0 );
	self->mSpool = ( float )self->mReveal;

	self->Start ( 0, false );

	return 1;
}

//================================================================//
// DOXYGEN
//================================================================//

#ifdef DOXYGEN

	//----------------------------------------------------------------//
	/**	@lua	affirmStyle
		@text	Returns the textbox's default style. If no default style
				exists, creates an empty style, sets it as the default and
				returns it.

		@in		MOAITextLabel self
		@out	MOAITextStyle style
	*/
	int MOAITextLabel::_affirmStyle ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@lua	setFont
		@text	Sets the font to be used by the textbox's default style.
				If no default style exists, a default style is created.

		@in		MOAITextLabel self
		@in		MOAIFont font
		@out	nil
	*/
	int MOAITextLabel::_setFont ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@lua	setTextSize
		@text	Sets the size to be used by the textbox's default style.
				If no default style exists, a default style is created.

		@in		MOAITextLabel self
		@in		number points			The point size to be used by the default style.
		@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
		@out	nil
	*/
	int MOAITextLabel::_setTextSize ( lua_State* L ) {
	}

#endif

//================================================================//
// MOAITextLabel
//================================================================//

const float MOAITextLabel::DEFAULT_SPOOL_SPEED = 24.0f;

//----------------------------------------------------------------//
void MOAITextLabel::BuildLocalToWorldMtx ( ZLAffine3D& localToWorldMtx ) {

	this->MOAITransform::BuildLocalToWorldMtx ( localToWorldMtx );

	float yScale = this->mLayoutRules.GetYFlip () ? -1.0f : 1.0f;

	ZLAffine3D mtx;
	mtx.ScRoTr ( 1.0f, yScale, 1.0f, 0.0f, 0.0f, 0.0f, this->mLayout.mXOffset, this->mLayout.mYOffset, 0.0f );
	localToWorldMtx.Prepend ( mtx );
}

//----------------------------------------------------------------//
void MOAITextLabel::Draw ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	
	if ( !this->IsVisible ( lod )) return;
	if ( this->IsClear ()) return;
	
	if ( this->mReveal ) {
		
		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

		this->LoadGfxState ();
		this->LoadVertexTransform ();
		this->LoadUVTransform ();
	
		gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_VIEW_PROJ_MTX ));
		gfxMgr.mVertexCache.SetUVTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::UV_MTX ));
		
		MOAIShader* shader = this->mMaterialBatch ? this->mMaterialBatch->RawGetShader ( 0 ) : 0;
		bool useSpriteShaders = !shader;
		
		if ( useSpriteShaders ) {
			shader = MOAIShaderMgr::Get ().GetShader ( MOAIShaderMgr::FONT_SNAPPING_SHADER );
		}
		this->mLayout.Draw ( this->mReveal, shader, useSpriteShaders );
	}
}

//----------------------------------------------------------------//
void MOAITextLabel::DrawDebug ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );

	MOAIGraphicsProp::DrawDebug ( subPrimID, lod );

	if ( !this->IsVisible ( lod )) return;
	if ( this->IsClear ()) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	ZLMatrix4x4 worldDrawingMtx = this->GetWorldDrawingMtx ();
	
	gfxMgr.mGfxState.SetMtx ( MOAIGfxGlobalsCache::WORLD_MTX, worldDrawingMtx );
	gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_VIEW_PROJ_MTX ));
	
	this->mLayout.DrawDebug ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	draw.Bind ();
	
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	if (( this->mLayout.mLayoutBounds.Area () > 0.0f ) && debugLines.Bind ( MOAIDebugLines::TEXT_BOX_LAYOUT_BOUNDS )) {
		
		draw.DrawRectOutline ( this->mLayout.mLayoutBounds );
	}
	
	if (( this->mLayout.mGlyphBounds.Area () > 0.0f ) && debugLines.Bind ( MOAIDebugLines::TEXT_BOX_GLYPH_BOUNDS )) {
		
		draw.DrawRectOutline ( this->mLayout.mGlyphBounds );
	}
	
	ZLRect frame = this->mLayoutRules.GetFrame ();
	
	if ( frame.Area () > 0.0f ) {
	
		frame.Offset ( -this->mLayout.mXOffset, -this->mLayout.mYOffset );
		
		if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX )) {
		
			draw.DrawRectOutline ( frame );
		}
		
		if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX_LIMITS )) {
			
			if ( this->mLayoutRules.GetLimitHeight ()) {
				draw.DrawLine ( frame.mXMin, frame.mYMin, frame.mXMax, frame.mYMin );
				draw.DrawLine ( frame.mXMin, frame.mYMax, frame.mXMax, frame.mYMax );
			}
			
			if ( this->mLayoutRules.GetLimitWidth ()) {
				draw.DrawLine ( frame.mXMin, frame.mYMin, frame.mXMin, frame.mYMax );
				draw.DrawLine ( frame.mXMax, frame.mYMin, frame.mXMax, frame.mYMax );
			}
		}
	}
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAITextLabel::GetWorldDrawingMtx () {

	ZLMatrix4x4 worldDrawingMtx = MOAIGraphicsProp::GetWorldDrawingMtx ();
	
	if ( this->mAutoFlip ) {
		
		MOAIRenderMgr& renderMgr = MOAIRenderMgr::Get ();
		
		MOAICamera* camera = renderMgr.GetCamera ();
		if ( camera ) {
		
			// TODO: this is a bunch of getting and re-calculating drawing matrices
			// would be better to cache these in a drawing intent that gets passed
			// down from the renderer
		
			MOAIViewport* viewport = renderMgr.GetViewport ();
			assert ( viewport );
			ZLMatrix4x4 viewProj = camera->GetViewProjMtx ( *viewport );

			ZLVec3D upVec = worldDrawingMtx.GetYAxis ();

			viewProj.TransformVec ( upVec );

			// For text flipping when orbiting around the map. Tilting should not affect this
			if ( upVec.mY > 0.0f ) {

				ZLMatrix4x4 flip;
				flip.Scale ( -1.0f, -1.0f, 1.0f );
				
				// if there's no x-axis constraint, flip inside the glyph rect
				if ( !this->mLayoutRules.GetLimitWidth ()) {
					float xOffset = this->mLayout.mGlyphBounds.mXMin + this->mLayout.mGlyphBounds.mXMax;
					flip.m [ ZLMatrix4x4::C3_R0 ] = xOffset;
				}
				
				// if there's no y-axis constraint, flip inside the glyph rect
				if ( !this->mLayoutRules.GetLimitHeight ()) {
					float yOffset = this->mLayout.mGlyphBounds.mYMin + this->mLayout.mGlyphBounds.mYMax;
					flip.m [ ZLMatrix4x4::C3_R1 ] = yOffset;
				}
				worldDrawingMtx.Prepend ( flip );
			}
		}
	}
	
	return worldDrawingMtx;
}

//----------------------------------------------------------------//
bool MOAITextLabel::IsDone () {

	if ( this->IsActive ()) {
		this->RefreshLayout ();
		return ( this->mReveal >= this->mLayout.CountSprites ());
	}
	return true;
}

//----------------------------------------------------------------//
MOAITextLabel::MOAITextLabel () :
	mNeedsLayout ( false ),
	mSpool ( 0.0f ),
	mSpeed ( DEFAULT_SPOOL_SPEED ),
	mReveal ( REVEAL_ALL ),
	mCurrentPageIdx ( 0 ),
	mNextPageIdx ( 0 ),
	mMore ( false ),
	mOverrun ( false ),
	mAutoFlip ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGraphicsProp )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mStyleCache.SetOwner ( this );
	this->mLayoutRules.SetOwner ( this );

	this->mBlendMode.SetBlend ( ZGL_BLEND_FACTOR_SRC_ALPHA, ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
}

//----------------------------------------------------------------//
MOAITextLabel::~MOAITextLabel () {

	this->mLayout.ClearHighlights ();
	this->ResetLayout ();
}

//----------------------------------------------------------------//
bool MOAITextLabel::More () {
	
	this->Refresh ();
	
	if ( this->mReveal < this->mLayout.CountSprites ()) {
		return true;
	}
	return this->mMore;
}

//----------------------------------------------------------------//
void MOAITextLabel::NextPage ( bool reveal ) {
	
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
void MOAITextLabel::OnDepNodeUpdate () {

	this->Refresh ();
	MOAIGraphicsProp::OnDepNodeUpdate ();
}

//----------------------------------------------------------------//
u32 MOAITextLabel::OnGetModelBounds ( ZLBox& bounds ) {

	this->Refresh ();

	ZLRect textBounds; // the tight fitting bounds of the text (if any: may be empty)
	bool hasBounds = this->mLayout.GetBounds ( textBounds );
	
	ZLRect textFrame = this->mLayoutRules.GetFrame ();
	bool limitWidth = this->mLayoutRules.GetLimitWidth ();
	bool limitHeight = this->mLayoutRules.GetLimitHeight ();
	
	if ( hasBounds ) {
	
		if ( limitWidth ) {
			textBounds.mXMin = textFrame.mXMin;
			textBounds.mXMax = textFrame.mXMax;
		}
		
		if ( limitHeight ) {
			textBounds.mYMin = textFrame.mYMin;
			textBounds.mYMax = textFrame.mYMax;
		}
	
		bounds.Init ( textBounds.mXMin, textBounds.mYMax, textBounds.mXMax, textBounds.mYMin, 0.0f, 0.0f );
		return MOAIProp::BOUNDS_OK;
	}
	else {
	
		// if the text bounds are empty, then *both* frame axis must be in use for the rect to be valid
		if ( limitWidth && limitHeight ) {
			bounds.Init ( textFrame.mXMin, textFrame.mYMax, textFrame.mXMax, textFrame.mYMin, 0.0f, 0.0f );
			return MOAIProp::BOUNDS_OK;
		}
	}
	
	return MOAIProp::BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAITextLabel::OnUpdate ( double step ) {
	
	this->mSpool += ( float )( this->mSpeed * step );
	this->mReveal = ( u32 )this->mSpool;
}

//----------------------------------------------------------------//
void MOAITextLabel::Refresh () {

	if ( this->mStyleCache.CheckStylesChanged ()) {
		this->mNeedsLayout = true;
		this->RefreshStyleGlyphs ();
	}

	if ( this->mNeedsLayout ) {
		this->RefreshLayout ();
		this->mNeedsLayout = false;
	}
}

//----------------------------------------------------------------//
void MOAITextLabel::RefreshLayout () {

	this->mLayout.Reset ();
	this->mStyleCache.ClearAnonymousStyles ();

	this->mStyleMap.BuildStyleMap ( this->mStyleCache, this->mText.c_str ());

	this->mLayoutRules.Layout ( this->mLayout, this->mStyleCache, this->mStyleMap, this->mText.c_str (), this->mCurrentPageIdx, &this->mMore, &this->mNextPageIdx, &this->mOverrun );
}

//----------------------------------------------------------------//
void MOAITextLabel::RefreshStyleGlyphs () {

	this->mStyleMap.RefreshStyleGlyphs ( this->mText.c_str ());
}

//----------------------------------------------------------------//
void MOAITextLabel::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGraphicsProp::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );

	state.SetField ( -1, "OVERRUN_MOVE_WORD",		( u32 )MOAITextLayoutRules::OVERRUN_MOVE_WORD );
	state.SetField ( -1, "OVERRUN_SPLIT_WORD",		( u32 )MOAITextLayoutRules::OVERRUN_SPLIT_WORD );
	state.SetField ( -1, "OVERRUN_TRUNCATE_WORD",	( u32 )MOAITextLayoutRules::OVERRUN_TRUNCATE_WORD );
	state.SetField ( -1, "OVERRUN_ABORT_LAYOUT",	( u32 )MOAITextLayoutRules::OVERRUN_ABORT_LAYOUT );

	state.SetField ( -1, "GLYPH_SIZE",				( u32 )MOAITextLayoutRules::GLYPH_SIZE );
	state.SetField ( -1, "LOGICAL_SIZE",			( u32 )MOAITextLayoutRules::LOGICAL_SIZE );

	state.SetField ( -1, "BASELINE_JUSTIFY",		( u32 )MOAITextLayoutRules::BASELINE_JUSTIFY );
	state.SetField ( -1, "BOTTOM_JUSTIFY",			( u32 )MOAITextLayoutRules::BOTTOM_JUSTIFY );
	state.SetField ( -1, "LEFT_JUSTIFY",			( u32 )MOAITextLayoutRules::LEFT_JUSTIFY );
	state.SetField ( -1, "CENTER_JUSTIFY",			( u32 )MOAITextLayoutRules::CENTER_JUSTIFY );
	state.SetField ( -1, "RIGHT_JUSTIFY",			( u32 )MOAITextLayoutRules::RIGHT_JUSTIFY );
	state.SetField ( -1, "TOP_JUSTIFY",				( u32 )MOAITextLayoutRules::TOP_JUSTIFY );
}

//----------------------------------------------------------------//
void MOAITextLabel::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIGraphicsProp::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clearHighlights",		_clearHighlights },
		{ "getAlignment",			_getAlignment },
		{ "getGlyphScale",			_getGlyphScale },
		{ "getLineSpacing",			_getLineSpacing },
		{ "getOverrunRules",		_getOverrunRules },
		{ "getRect",				_getRect },
		{ "getSizingRules",			_getSizingRules },
		{ "getStyle",				_getStyle },
		{ "getText",				_getText },
		{ "getTextBounds",			_getTextBounds },
		{ "hasOverrun",				_hasOverrun },
		{ "more",					_more },
		{ "nextPage",				_nextPage },
		{ "reserveCurves",			_reserveCurves },
		{ "revealAll",				_revealAll },
		{ "setAlignment",			_setAlignment },
		{ "setAutoFlip",			_setAutoFlip },
		{ "setBounds",				_setBounds },
		{ "setCurve",				_setCurve },
		{ "setGlyphScale",			_setGlyphScale },
		{ "setLineSnap",			_setLineSnap },
		{ "setLineSpacing",			_setLineSpacing },
		{ "setHighlight",			_setHighlight },
		{ "setOverrunRules",		_setOverrunRules },
		{ "setRect",				_setRect },
		{ "setRectLimits",			_setRectLimits },
		{ "setReveal",				_setReveal },
		{ "setSizingRules",			_setSizingRules },
		{ "setSpeed",				_setSpeed },
		{ "setStyle",				_setStyle },
		{ "setText",				_setText },
		{ "setWordBreak",			_setWordBreak },
		{ "setYFlip",				_setYFlip },
		{ "spool",					_spool },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITextLabel::ResetLayout () {

	this->mMore = false;
	this->mOverrun = false;
	this->mLayout.Reset ();
}

//----------------------------------------------------------------//
void MOAITextLabel::ScheduleLayout () {

	this->mNeedsLayout = true;
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAITextLabel::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {	
	MOAIGraphicsProp::SerializeIn ( state, serializer );
	MOAIAction::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAITextLabel::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIGraphicsProp::SerializeOut ( state, serializer );
	MOAIAction::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAITextLabel::SetText ( cc8* text ) {

	this->mText = text;
	this->mMore = ( text && text [ 0 ]);
	this->mOverrun = this->mMore;
	
	this->mReveal = REVEAL_ALL;
	this->mSpool = 0.0f;
	
	this->mCurrentPageIdx = 0;
	this->mNextPageIdx = 0;
	
	this->ScheduleLayout ();
}
