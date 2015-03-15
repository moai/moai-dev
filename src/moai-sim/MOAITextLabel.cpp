// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAINodeMgr.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAITextDesigner.h>
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
	state.Push ( self->mDesigner.GetHAlign ());
	state.Push ( self->mDesigner.GetVAlign ());
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
	state.Push ( self->mDesigner.GetGlyphScale ());
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
	
	lua_pushnumber ( state, self->mDesigner.GetLineSpacing ());
	return 1;
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

	ZLRect rect = self->mDesigner.GetFrame ();
	state.Push ( rect );

	return 4;
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
		hasRect = self->mLayout.GetBounds ( rect );
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
	self->mDesigner.ReserveCurves ( total );

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

	self->mDesigner.SetHAlign ( state.GetValue < u32 >( 2, MOAITextDesigner::LEFT_JUSTIFY ));
	self->mDesigner.SetVAlign ( state.GetValue < u32 >( 3, MOAITextDesigner::TOP_JUSTIFY ));
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITextLabel::_setAutoFlip ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	self->mAutoFlip = state.GetValue < bool >( 2, false );
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITextLabel::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextLabel, "U" )

	if ( state.CheckParams ( 2, "NNNNNN", false )) {

		ZLBox bounds = state.GetBox ( 2 );
		bounds.Bless ();
		
		ZLRect frame;
		frame = bounds.GetRect ( ZLBox::PLANE_XY );
		
		self->mDesigner.SetFrame ( frame );
		self->mDesigner.SetLimitWidth ( true );
		self->mDesigner.SetLimitHeight ( true );
	}
	else {
		self->mDesigner.SetLimitWidth ( false );
		self->mDesigner.SetLimitHeight ( false );
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

		self->mDesigner.SetCurve ( index, curve );
		self->ScheduleLayout ();
	}
	else {
		self->mDesigner.ClearCurves ();
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
	self->mDesigner.SetGlyphScale ( state.GetValue < float >( 2, 1.0f ));
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
	
	self->mDesigner.SetHLineSnap ( hLineSnap );
	self->mDesigner.SetVLineSnap ( vLineSnap );
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
	
	self->mDesigner.SetLineSpacing ( lineSpacing );
	self->ScheduleLayout ();
	
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
	self->mDesigner.SetFrame ( rect );

	self->mDesigner.SetLimitWidth ( true );
	self->mDesigner.SetLimitHeight ( true );

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

	self->mDesigner.SetLimitWidth ( state.GetValue < bool >( 2, false ));
	self->mDesigner.SetLimitHeight ( state.GetValue < bool >( 3, false ));

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
/**	@lua	setSpeed
	@text	Sets the base spool speed used when creating a spooling MOAIAction with the spool() function.

	@in		MOAITextLabel self
	@in		number speed				The base spooling speed.
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

	self->mDesigner.SetWordBreak ( state.GetValue < u32 >( 2, MOAITextDesigner::WORD_BREAK_NONE ));
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

	self->mDesigner.SetYFlip ( state.GetValue < bool >( 2, false ));
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

	self->Start ( MOAISim::Get ().GetActionMgr (), false );

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

	float yScale = this->mDesigner.GetYFlip () ? -1.0f : 1.0f;

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
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

		this->LoadGfxState ();
		this->LoadVertexTransform ();
		this->LoadUVTransform ();
	

		if ( !this->mShader ) {
			// TODO: this should really come from MOAIFont, which should really be a
			// specialized implementation of MOAIDeck...
			gfxDevice.SetShaderPreset ( MOAIShaderMgr::FONT_SNAPPING_SHADER );
		}

		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
		this->mLayout.Draw ( this->mReveal );
	}
}

//----------------------------------------------------------------//
void MOAITextLabel::DrawDebug ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );

	MOAIGraphicsProp::DrawDebug ( subPrimID, lod );

	if ( !this->IsVisible ( lod )) return;
	if ( this->IsClear ()) return;

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	ZLMatrix4x4 worldDrawingMtx = this->GetWorldDrawingMtx ();
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, worldDrawingMtx );
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX )) {
	
		ZLRect bounds;
		if ( this->mLayout.GetBounds ( bounds )) {
		
			ZLRect frame = this->mDesigner.GetFrame ();
		
			if ( this->mDesigner.GetLimitWidth ()) {
				float xOffset = this->mLayout.mXOffset;
				bounds.mXMin = frame.mXMin - xOffset;
				bounds.mXMax = frame.mXMax - xOffset;
			}
			
			if ( this->mDesigner.GetLimitHeight ()) {
				float yOffset = this->mLayout.mYOffset;
				bounds.mYMin = frame.mYMin - yOffset;
				bounds.mYMax = frame.mYMax - yOffset;
			}
		
			draw.DrawRectOutline ( bounds );
		}
	}
	
	this->mLayout.DrawDebug ();
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
			
			ZLMatrix4x4 viewProj = camera->GetWorldToWndMtx ( *viewport );
		
			ZLVec3D upVec = worldDrawingMtx.GetYAxis ();
			viewProj.TransformVec ( upVec );
			
			if ( upVec.mY < 0.0f ) {
				ZLMatrix4x4 scale;
				scale.Scale ( -1.0f, -1.0f, 1.0f );
				worldDrawingMtx.Prepend ( scale );
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
	mAutoFlip ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGraphicsProp )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mStyleCache.SetOwner ( this );
	this->mDesigner.SetOwner ( this );

	//this->SetMask ( MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
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
	
	ZLRect textFrame = this->mDesigner.GetFrame ();
	bool limitWidth = this->mDesigner.GetLimitWidth ();
	bool limitHeight = this->mDesigner.GetLimitHeight ();
	
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
	
	this->mSpool += ( this->mSpeed * step );
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

	ZLVec2D offset ( 0.0f, 0.0f );
	this->mDesigner.Layout ( this->mLayout, this->mStyleCache, this->mStyleMap, this->mText.c_str (), this->mCurrentPageIdx, offset, &this->mMore, &this->mNextPageIdx );
}

//----------------------------------------------------------------//
void MOAITextLabel::RefreshStyleGlyphs () {

	this->mStyleMap.RefreshStyleGlyphs ( this->mText.c_str ());
}

//----------------------------------------------------------------//
void MOAITextLabel::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGraphicsProp::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );

	state.SetField ( -1, "WORD_BREAK_NONE", ( u32 )MOAITextDesigner::WORD_BREAK_NONE );
	state.SetField ( -1, "WORD_BREAK_CHAR", ( u32 )MOAITextDesigner::WORD_BREAK_CHAR );

	state.SetField ( -1, "BASELINE_JUSTIFY",	( u32 )MOAITextDesigner::BASELINE_JUSTIFY );
	state.SetField ( -1, "BOTTOM_JUSTIFY",		( u32 )MOAITextDesigner::BOTTOM_JUSTIFY );
	state.SetField ( -1, "LEFT_JUSTIFY",		( u32 )MOAITextDesigner::LEFT_JUSTIFY );
	state.SetField ( -1, "CENTER_JUSTIFY",		( u32 )MOAITextDesigner::CENTER_JUSTIFY );
	state.SetField ( -1, "RIGHT_JUSTIFY",		( u32 )MOAITextDesigner::RIGHT_JUSTIFY );
	state.SetField ( -1, "TOP_JUSTIFY",			( u32 )MOAITextDesigner::TOP_JUSTIFY );
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
		{ "getRect",				_getRect },
		{ "getStyle",				_getStyle },
		{ "getText",				_getText },
		{ "getTextBounds",			_getTextBounds },
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
		{ "setReveal",				_setReveal },
		{ "setRect",				_setRect },
		{ "setRectLimits",			_setRectLimits },
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
	
	this->mReveal = REVEAL_ALL;
	this->mSpool = 0.0f;
	
	this->mCurrentPageIdx = 0;
	this->mNextPageIdx = 0;
	
	this->ScheduleLayout ();
}
