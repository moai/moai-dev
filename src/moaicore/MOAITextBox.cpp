// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
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

/*

	Raw text -> styler -> layout -> page

	From a string, we generate a text layout. A layout is a data structure optimized for
	fast arrangement into pages. The layout is an index of all words in the original text properly
	sized and arranged along a horizontal line. The layout may be shared by multiple pages.
	Each page is a 'window' into the layout (and corresponding string). Ideally, the page
	should aggregate any graphics state changes required to render the text.
	
	Here's the process:
	
	1. Load the text into a styler.
		a. Parse the text into words and make a table of styles used.
			i. Each word should reference its style.
			ii. For words requiring multiple styles, break them into compound words.
			iii. Words should not any following words that are breaks so as to not separate during page layout.
		b. Populate the fonts used with the glyphs required by the text.
		c. For each font, load and resolve glyph sizes (but do not rasterize glyphs).
		d. Using the glyph sizes, scan the word table and assign a location and width to each word.
	
	2. Load the text into a page.
		a. Break the text into lines. Each line gets an offset depending on justification.
		b. Scan the text in the page and rasterize glyphs.
		c. Update page's base text offset and character count.

	A text style controls the font, size, color and transform used for a piece of text. A particle
	system may also be attached to the style. In this case, the styled text will be spawned into the
	particle system instead of rendered. The style may be configured to spawn particle text in either
	local or world space. To make this work, the glyph cache for a particular font size must implement
	the deck interface.
*/

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clearCurves
	@text	Removes all of the animation curves currently bound to this text object.

	@in		MOAITextBox self
	@out	nil
*/
int MOAITextBox::_clearCurves ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	self->ClearCurves ();
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getLineSize
	@text	Returns the size of a line (in pixels).

	@in		MOAITextBox self
	@out	number lineScale		The size of the line in pixels.
*/
int MOAITextBox::_getLineSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	
	//if ( self->mFont ) {
	//	lua_pushnumber ( state, self->mPoints * self->mFont->GetLineSpacing ());
	//	return 1;
	//}
	return 0;
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
	
	//u32 index	= state.GetValue < u32 >( 2, 1 ) - 1;
	//u32 size	= state.GetValue < u32 >( 3, 0 );
	//
	//if ( size ) {
	//	self->Layout ();
	//	
	//	USRect rect;
	//	if ( self->mLayout.GetBoundsForRange ( index, size, rect )) {
	//		
	//		rect.Bless ();
	//		
	//		lua_pushnumber ( state, rect.mXMin );
	//		lua_pushnumber ( state, rect.mYMin );
	//		lua_pushnumber ( state, rect.mXMax );
	//		lua_pushnumber ( state, rect.mYMax );
	//		
	//		return 4;
	//	}
	//}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
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
	
	bool more = self->More ();
	lua_pushboolean ( L, more );
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
	@text	Binds an animation curve to the text, where the Y value of the curve indicates the text offset.

	@in		MOAITextBox self
	@in		number curveID				The ID of the curve within this text object.
	@in		MOAIAnimCurve curve			The MOAIAnimCurve to bind to.
	@out	nil
*/
int MOAITextBox::_setCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UNU" )
	
	u32 index = state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAIAnimCurve* curve = state.GetLuaObject < MOAIAnimCurve >( 3 );
	if ( !curve ) return 0;

	self->SetCurve ( index, curve );
	self->ScheduleLayout ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLineSpacing
	@text	Sets the scale of spacing between lines. '1' uses the fon't default spacing.

	@in		MOAITextBox self
	@in		number lineSpacing		Default value is 1.
*/
int MOAITextBox::_setLineSpacing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )
	
	self->mLineSpacing = state.GetValue < float >( 2, 1.0f );
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
	@text	Sets the number of renderable characters to be shown.  Can range from 0 to any value; values greater than the number of renderable characters in the current text will be ignored.

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

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setStringColor
	@text	Sets the color of a sub string in the text. Only affects
			text displayed on the current page.

	@in		MOAITextBox self
	@in		number index		Index of the first character in the substring.
	@in		number size			Length of the substring.
	@in		number r
	@in		number g
	@in		number b
	@opt	number a			Default value is 1.
*/
int MOAITextBox::_setStringColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "UNNNNN" )
	
	//u32 index	= state.GetValue < u32 >( 2, 1 ) - 1;
	//u32 size	= state.GetValue < u32 >( 3, 0 );
	//
	//if ( size ) {
	//	self->Layout ();
	//
	//	float r		= state.GetValue < float >( 4, 1.0f );
	//	float g		= state.GetValue < float >( 5, 1.0f );
	//	float b		= state.GetValue < float >( 6, 1.0f );
	//	float a		= state.GetValue < float >( 7, 1.0f );
	//	
	//	u32 rgba = USColor::PackRGBA ( r, g, b, a );
	//	self->mLayout.SetColorForRange ( index, size, rgba );
	//}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITextBox::_setStyle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextBox, "U" )

	cc8* styleName = state.GetValue < cc8* >( 2, "" );
	
	if ( strlen ( styleName )) {
	
		MOAITextStyle* style = state.GetLuaObject < MOAITextStyle >( 3 );
		self->SetStyle ( styleName, style );
	}
	else {
	
		MOAITextStyle* style = state.GetLuaObject < MOAITextStyle >( 2 );
		self->SetStyle ( style );
	}
	
	self->ResetStyleMap ();
	self->ScheduleLayout ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setYFlip
	@text	Sets the rendering direction for the text.  Default assumes a window style screen space (positive Y moves down the screen).  Set to true to render text for world style coordinate systems (positive Y moves up the screen).

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
	@text	Creates a new MOAIAction which when run has the effect of increasing the amount of characters revealed from 0 to the length of the string currently set.  The spool action is automatically added to the root of the action tree, but may be reparented or stopped by the developer.  This function also automatically sets the current number of revealed characters to 0 (i.e. MOAITextBox:setReveal(0)).

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
void MOAITextBox::Draw ( int subPrimID, bool reload ) {
	UNUSED ( subPrimID ); 
	UNUSED ( reload );
	
	if ( this->mReveal ) {
	
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

		this->LoadGfxState ();

		if ( !this->mShader ) {
			// TODO: this should really come from MOAIFont, which should really be a
			// specialized implementation of MOAIDeck...
			gfxDevice.SetShaderPreset ( MOAIShaderMgr::FONT_SHADER );
		}

		gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, this->GetLocalToWorldMtx ());
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
		
		MOAIQuadBrush::BindVertexFormat ( gfxDevice );

		USColorVec baseColor = gfxDevice.GetPenColor ();
		USColorVec blendColor;
		u32 rgba = 0xffffffff;

		u32 size = this->mSprites.GetTop ();
		for ( u32 i = 0; ( i < size ) && ( i < this->mReveal ); ++i ) {
			const MOAITextSprite& sprite = this->mSprites [ i ];
			
			if ( sprite.mRGBA != rgba ) {
				rgba = sprite.mRGBA;
				
				blendColor.SetRGBA ( rgba );
				blendColor.Modulate ( baseColor );
				gfxDevice.SetPenColor ( blendColor );
			}
			sprite.mGlyph->Draw ( sprite.mX, sprite.mY );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextBox::DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID ); 

	this->Layout ();

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
	debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX )) {
		debugLines.DrawRect ( this->mFrame );
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX_BASELINES )) {
		
		u32 totalLines = this->mLines.GetTop ();
		for ( u32 i = 0; i < totalLines; ++i ) {
			MOAITextLine& line = this->mLines [ i ];
			
			float y = line.mRect.mYMin + line.mAscent;
			debugLines.DrawLine ( line.mRect.mXMin, y, line.mRect.mXMax, y );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX_LAYOUT )) {
		
		u32 totalLines = this->mLines.GetTop ();
		for ( u32 i = 0; i < totalLines; ++i ) {
			MOAITextLine& line = this->mLines [ i ];
			debugLines.DrawRect ( line.mRect );
		}
	}
}

//----------------------------------------------------------------//
u32 MOAITextBox::GetDeckBounds ( USBox& bounds ) {

	bounds.Init ( this->mFrame.mXMin, this->mFrame.mYMax, this->mFrame.mXMax, this->mFrame.mYMin, 0.0f, 0.0f );
	return MOAIProp::BOUNDS_OK;
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextBox::GetStyle () {

	return this->mDefaultStyle;
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextBox::GetStyle ( cc8* styleName ) {

	if ( styleName && strlen ( styleName )) {
		if ( this->mStyleSet.contains ( styleName )) {
			return this->mStyleSet [ styleName ];
		}
	}
	return this->mDefaultStyle;
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
			
			u32 totalActiveStyles = this->mActiveStyles.GetTop ();
			for ( u32 i = 0; i < totalActiveStyles; ++i ) {
				MOAITextStyleState& styleState = this->mActiveStyles [ i ];
				this->LuaRetain ( styleState.mFont );
				styleState.mFont->UpdateGlyphs ( MOAIGlyph::METRICS_AND_BITMAP );
			}
		}
		
		this->ResetLayout ();
		
		MOAITextDesigner designer;
		designer.BuildLayout ( *this );
	}
	
	this->mNeedsLayout = false;
}

//----------------------------------------------------------------//
MOAITextBox::MOAITextBox () :
	mLineSpacing ( 1.0f ),
	mText ( "" ),
	mTextLength ( 0 ),
	mHAlign ( LEFT_JUSTIFY ),
	mVAlign ( LEFT_JUSTIFY ),
	mSpool ( 0.0f ),
	mSpeed ( DEFAULT_SPOOL_SPEED ),
	mReveal ( REVEAL_ALL ),
	mYFlip ( false ),
	mCurrentPageIdx ( 0 ),
	mNextPageIdx ( 0 ),
	mNeedsLayout ( false ),
	mDefaultStyle ( 0 ),
	mMore ( false ) {
	
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
	UNUSED ( reveal );
	
	if ( this->mMore ) {
		this->mCurrentPageIdx = this->mNextPageIdx;
	}
	else {
		this->mCurrentPageIdx = 0;
		this->mNextPageIdx = 0;
	}
	
	this->mNeedsLayout = true;
	this->mReveal = reveal ? REVEAL_ALL : 0;
	this->mSpool = 0.0f;
}

//----------------------------------------------------------------//
void MOAITextBox::OnDepNodeUpdate () {

	MOAIProp::OnDepNodeUpdate ();

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
void MOAITextBox::PushSprite ( const MOAIGlyph* glyph, u32 idx, float x, float y, u32 rgba ) {

	MOAITextSprite textSprite;
	
	textSprite.mGlyph		= glyph;
	textSprite.mIdx			= idx;
	textSprite.mX			= x;
	textSprite.mY			= y;
	textSprite.mRGBA		= rgba;

	this->mSprites.Push ( textSprite );
}

//----------------------------------------------------------------//
void MOAITextBox::PushStyleSpan ( int base, int top, MOAITextStyleState& styleState ) {

	u32 styleID = 0;
	u32 totalStyles = this->mActiveStyles.GetTop ();
	for ( ; styleID < totalStyles; ++styleID ) {
		if ( this->mActiveStyles [ styleID ].IsMatch ( styleState )) {
			break;
		}
	}
	
	if ( styleID >= totalStyles ) {
		this->mActiveStyles.Push ( styleState );
		styleID = totalStyles;
	}

	MOAITextStyleSpan span;
	
	span.mBase			= base;
	span.mTop			= top;
	span.mStyleID		= styleID;

	this->mStyleMap.Push ( span );
}

//----------------------------------------------------------------//
void MOAITextBox::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIProp::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );

	state.SetField ( -1, "LEFT_JUSTIFY", ( u32 )LEFT_JUSTIFY );
	state.SetField ( -1, "CENTER_JUSTIFY", ( u32 )CENTER_JUSTIFY );
	state.SetField ( -1, "RIGHT_JUSTIFY", ( u32 )RIGHT_JUSTIFY );
}

//----------------------------------------------------------------//
void MOAITextBox::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIProp::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clearCurves",			_clearCurves },
		{ "getLineSize",			_getLineSize },
		{ "getStringBounds",		_getStringBounds },
		{ "getStyle",				_getStyle },
		{ "more",					_more },
		{ "nextPage",				_nextPage },
		{ "reserveCurves",			_reserveCurves },
		{ "revealAll",				_revealAll },
		{ "setAlignment",			_setAlignment },
		{ "setCurve",				_setCurve },
		{ "setLineSpacing",			_setLineSpacing },
		{ "setRect",				_setRect },
		{ "setReveal",				_setReveal },
		{ "setSpeed",				_setSpeed },
		{ "setString",				_setString },
		{ "setStringColor",			_setStringColor },
		{ "setStyle",				_setStyle },
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
	this->mLines.Reset ();
	this->mSprites.Reset ();
}

//----------------------------------------------------------------//
void MOAITextBox::ResetStyleMap () {

	u32 totalActiveStyles = this->mActiveStyles.GetTop ();
	for ( u32 i = 0; i < totalActiveStyles; ++i ) {
		MOAITextStyleState& styleState = this->mActiveStyles [ i ];
		this->LuaRelease ( styleState.mFont );
	}

	this->mActiveStyles.Reset ();
	this->mStyleMap.Reset ();
}

//----------------------------------------------------------------//
void MOAITextBox::ResetStyleSet () {

	this->LuaRelease ( this->mDefaultStyle );
	this->mDefaultStyle = 0;
	
	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		this->LuaRelease ( styleSetIt->second );
	}
	this->mStyleSet.clear ();
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
void MOAITextBox::SetRect ( float left, float top, float right, float bottom ) {

	this->mFrame.Init ( left, top, right, bottom );
}

//----------------------------------------------------------------//
void MOAITextBox::SetStyle ( MOAITextStyle* style ) {

	this->SetStyle ( 0, style );
}

//----------------------------------------------------------------//
void MOAITextBox::SetStyle ( cc8* styleName, MOAITextStyle* style ) {

	if ( styleName && strlen ( styleName )) {
		
		MOAITextStyle* prevStyle = 0;
		
		if ( this->mStyleSet.contains ( styleName )) {
			prevStyle = this->mStyleSet [ styleName ];
			if ( prevStyle == style ) return;
		}
		
		this->LuaRetain ( style );
		this->LuaRelease ( prevStyle );
		
		if ( style ) {
			this->mStyleSet [ styleName ] = style;
		}
		else {
			if ( this->mStyleSet.contains ( styleName )) {
				this->mStyleSet.erase ( styleName );
			}
		}
	}
	else {
	
		if ( this->mDefaultStyle == style ) return;
		
		this->LuaRetain ( style );
		this->LuaRelease ( this->mDefaultStyle );
		
		this->mDefaultStyle = style;
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
}

