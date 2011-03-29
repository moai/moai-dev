// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAITextBox.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>() clearCurves ( self )</tt>\n
\n
	Removes set of text curves.
	@param self (in)
*/
int MOAITextBox::_clearCurves ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "U" )

	self->ClearCurves ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( more ) more ( self )</tt>\n
\n
	Returns 'true' if there are additional pages of text.
	@param self (in)
	@param more (out)
*/
int MOAITextBox::_more ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "U" )
	
	bool more = self->More ();
	lua_pushboolean ( L, more );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>() nextPage ( self )</tt>\n
\n
	Advances to next page of text (if any) or wraps to start of text (if at end).
	@param self (in)
*/
int MOAITextBox::_nextPage ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "U" )

	bool reveal = state.GetValue < bool >( 2, true );
	self->NextPage ( reveal );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() reserveCurves ( self, nCurves )</tt>\n
\n
	Reserves space for curve references.
	@param self (in)
	@param nCurves (in)
*/
int MOAITextBox::_reserveCurves ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveCurves ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() revealAll ( self )</tt>\n
\n
	Display as much text as will fit in text box.
	@param self (in)
*/
int MOAITextBox::_revealAll ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "U" )
	
	self->mReveal = REVEAL_ALL;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setAlignment ( mAlignment )</tt>\n
	\n
	Sets the text alignment.
	@param mAlignment The alignment to set.  Values are as follows:
	\li \c LEFT_JUSTIFY - Aligns text to the left.
	\li \c CENTER_JUSTIFY - Centers the text.
	\li \c RIGHT_JUSTIFY - Aligns text to the right.
*/
int MOAITextBox::_setAlignment ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UN" )

	u32 alignment = state.GetValue < u32 >( 2, USFont::LEFT_JUSTIFY );
	self->mJustify = alignment;

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setCurve ( self, curveID, curve )</tt>\n
\n
	Install an anim curve. Used to offset text by y value of curve.
	@param self (in)
	@param self (curveID)
	@param self (curve)
*/
int MOAITextBox::_setCurve ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UNU" )
	
	u32 index = state.GetValue < u32 >( 2, 0 ) - 1;
	
	MOAIAnimCurve* curve = state.GetLuaObject < MOAIAnimCurve >( 3 );
	if ( !curve ) return 0;

	self->SetCurve ( index, curve );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setFont ( self, font )</tt>\n
\n
	Uses the font specified by a MOAIFont object.
	@param self (in)
	@param font (in) MOAIFont object to be used by the textbox.
*/
int MOAITextBox::_setFont ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UU" )

	MOAIFont* font = state.GetLuaObject < MOAIFont >( 2 );
	if ( !font ) return 0;
	
	self->SetFont ( font );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setParent ( self, parent )</tt>\n
\n
	Sets this textbox as a child to the specified parent transform, prim or layout.
	@param self (in)
	@param parent (in) The object which will be the parent of this textbox.
*/
int MOAITextBox::_setParent ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UU" )

	MOAITransform* parent = state.GetLuaObject < MOAITransform >( 2 );
	if ( !parent ) return 0;
	
	self->SetParent ( parent );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setRect ( self, left, top, right, bottom )</tt>\n
\n
	Sets the area rect for this text box.  Does not affect size.
	@param self (in)
	@param left The X coordinate of the rect's upper-left point.
	@param top The Y coordinate of the rect's upper-left point.
	@param right The X coordinate of the rect's lower-right point.
	@param bottom The Y coordinate of the rect's lower-right point.
*/
int MOAITextBox::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UNNNN" )

	float left		= state.GetValue < float >( 2, 0.0f );
	float top		= state.GetValue < float >( 3, 0.0f );
	float right		= state.GetValue < float >( 4, 0.0f );
	float bottom	= state.GetValue < float >( 5, 0.0f );

	self->SetRect ( left, top, right, bottom );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setReveal ( self, reveal )</tt>\n
\n
	Sets the number of renderable characters (i.e. not whitespace) to be
	shown. Can range from 0 to any value. Values greater than the number
	of visible glyps in the current text will be ignored.
	@param self (in)
	@param reveal (in)
*/
int MOAITextBox::_setReveal ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UN" )

	self->mReveal = state.GetValue < u32 >( 2, self->mReveal );
	self->mSpool = ( float )self->mReveal;

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setSpeed ( speed )</tt>\n
\n
	Sets the base spool speed.
	@param self (in)
	@param speed (in)
*/
int MOAITextBox::_setSpeed ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UN" )
	
	self->mSpeed = state.GetValue < float >( 2, self->mSpeed );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setString ( self, newStr )</tt>\n
\n
	Sets or changes the string.
	@param self (in)
	@param newStr (in) The string to be displayed by this textbox.
*/
int MOAITextBox::_setString ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "US" )

	cc8* text = state.GetValue < cc8* >( 2, "" );
	self->SetText ( text );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setTextSize ( self, newTextSize )</tt>\n
\n
	Sets or changes the size of the text. To avoid sampling, this should
	be the same as the point size retuned by the font (which is, confusingly,
	*different* from the 'point size' specified when creating the font; naming to
	be udpated Soon(tm)!)
	@param self (in)
	@param newTextSize The size the text will be.
*/
int MOAITextBox::_setTextSize ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UN" )

	float newTextSize = state.GetValue < float >( 2, 0.0f );
	self->SetTextSize ( newTextSize );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setYFlip ( self, yFlip )</tt>\n
\n
	Sets the rendering direction for the text. Default assumes a window
	style screen space (positive Y moves down the screen). Set to 'true'
	to render text for world style coordinate systems (positive Y moves
	up the screen).
	@param self (in)
	@param yFlip (in)
*/
int MOAITextBox::_setYFlip ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "UB" )

	self->mYFlip = state.GetValue < bool >( 2, self->mYFlip );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( spoolAction ) spool ( self )</tt>\n
\n
	Sets text box reveal to 0 (i.e. hides all visible text) and returns a
	spool action that, when run, will increment the reveal count to animate
	the text appearing letter by letter. Action is automatically added to
	the root of the action tree, but may be reparented or stopped by user.
	@param self (in)
*/
int MOAITextBox::_spool ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "U" )

	self->mReveal = state.GetValue < u32 >( 2, 0 );
	self->mSpool = ( float )self->mReveal;

	self->Start ();

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>() throttle ( self, throttle )</tt>\n
\n
	Scales the spool speed.
	@param self (in)
	@param throttle (in)
*/
int MOAITextBox::_throttle ( lua_State* L ) {
	LUA_SETUP ( MOAITextBox, "U" )

	self->mThrottle = state.GetValue < float >( 2, self->mThrottle );
	
	return 0;
}

//================================================================//
// MOAITextBox
//================================================================//

const float MOAITextBox::DEFAULT_SPOOL_SPEED = 24.0f;

//----------------------------------------------------------------//
void MOAITextBox::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	MOAITransform::ApplyAttrOp ( attrID, attrOp );
}

//----------------------------------------------------------------//
void MOAITextBox::ClearCurves () {

	for ( u32 i = 0; i < this->mMOAICurves.Size (); ++i ) {
		this->mMOAICurves [ i ]->Release ();
	}
	this->mMOAICurves.Clear ();
	this->mCurves.Clear ();
}

//----------------------------------------------------------------//
void MOAITextBox::ClearPageInfo () {

	this->mPageSize = 0;
}

//----------------------------------------------------------------//
void MOAITextBox::Draw () {
	
	if ( !this->mFont ) return;
	
	USFont* font = this->mFont->Bind ();
	
	if ( font ) {
	
		USDrawBuffer& drawbuffer = USDrawBuffer::Get ();
		USAffine2D localToWorldMtx = this->GetLocalToWorldMtx ();
		
		if ( this->mYFlip ) {
			
			USAffine2D mtx;
			
			mtx.Scale ( 1.0f, -1.0f );
			localToWorldMtx.Append ( mtx );
			
			mtx.Translate ( 0.0f, ( this->mFrame.mYMin + this->mFrame.mYMax ));
			localToWorldMtx.Append ( mtx );
		}
		
		drawbuffer.SetVtxTransform ( localToWorldMtx );
		
		this->LoadShader ();
		drawbuffer.SetBlendMode ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
		if ( this->mReveal && ( this->mTextOffset <= this->mTextLength )) {
			this->Layout ();
			this->mFont->Draw ( this->mLayout, this->mReveal );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextBox::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX )) {
		
		debugLines.SetWorldMtx ( this->GetLocalToWorldMtx ());
		debugLines.SetPenSpace ( MOAIDebugLines::MODEL_SPACE );
		
		debugLines.DrawRect ( this->mFrame );
	}
}

//----------------------------------------------------------------//
u32 MOAITextBox::GetLocalFrame ( USRect& frame ) {

	frame = this->mFrame;
	return MOAIProp::BOUNDS_OK;
}

//----------------------------------------------------------------//
bool MOAITextBox::IsBusy () {

	if ( this->IsActive ()) {
		this->Layout ();
		return ( this->mReveal < this->mLayout.GetTop ());
	}
	return false;
}

//----------------------------------------------------------------//
void MOAITextBox::Layout () {

	if ( this->mPageSize ) return;
	if ( this->mTextOffset > this->mTextLength ) return;
	if ( !this->mFont ) return;
	
	this->mLayout.Reset ();
	
	USTextStyler styler = this->mStyler;
	this->mPageSize = this->mFont->Layout (
		this->mLayout,
		&this->mText [ this->mTextOffset ],
		this->mPoints,
		this->mFrame,
		this->mJustify,
		styler,
		this->mCurves,
		this->mCurves.Size ()
	);
	
	this->mNextStyler = styler;
}

//----------------------------------------------------------------//
MOAITextBox::MOAITextBox () :
	mText ( "" ),
	mTextLength ( 0 ),
	mTextOffset ( 0 ),
	mJustify ( USFont::LEFT_JUSTIFY ),
	mPoints ( 0 ),
	mPageSize ( 0 ),
	mSpool ( 0.0f ),
	mSpeed ( DEFAULT_SPOOL_SPEED ),
	mThrottle ( 1.0f ),
	mReveal ( REVEAL_ALL ),
	mYFlip ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp2D )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f ); 
	this->SetMask ( MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
}

//----------------------------------------------------------------//
MOAITextBox::~MOAITextBox () {

	this->ClearCurves ();
}

//----------------------------------------------------------------//
bool MOAITextBox::More () {
	
	this->Layout ();
	u32 offset = this->mTextOffset + this->mPageSize;
	if ( offset >= this->mTextLength ) {
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAITextBox::NextPage ( bool reveal ) {
	
	this->Layout ();
	
	u32 offset = this->mTextOffset + this->mPageSize;
	if ( offset >= this->mTextLength ) {
		offset = 0;
	}
	this->mTextOffset = offset;
	this->mStyler = this->mNextStyler;
	
	this->ClearPageInfo ();
	this->mReveal = reveal ? REVEAL_ALL : 0;
	this->mSpool = 0.0f;
}

//----------------------------------------------------------------//
void MOAITextBox::OnUpdate ( float step ) {
	
	this->mSpool += ( this->mSpeed * this->mThrottle * step );
	this->mReveal = ( u32 )this->mSpool;
}

//----------------------------------------------------------------//
void MOAITextBox::RegisterLuaClass ( USLuaState& state ) {

	MOAIProp2D::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );

	state.SetField ( -1, "LEFT_JUSTIFY", ( u32 )USFont::LEFT_JUSTIFY );
	state.SetField ( -1, "CENTER_JUSTIFY", ( u32 )USFont::CENTER_JUSTIFY );
	state.SetField ( -1, "RIGHT_JUSTIFY", ( u32 )USFont::RIGHT_JUSTIFY );
}

//----------------------------------------------------------------//
void MOAITextBox::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIProp2D::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "clearCurves",		_clearCurves },
		{ "more",				_more },
		{ "nextPage",			_nextPage },
		{ "reserveCurves",		_reserveCurves },
		{ "revealAll",			_revealAll },
		{ "setAlignment",		_setAlignment },
		{ "setCurve",			_setCurve },
		{ "setFont",			_setFont },
		{ "setRect",			_setRect },
		{ "setReveal",			_setReveal },
		{ "setString",			_setString },
		{ "setTextSize",		_setTextSize },
		{ "setYFlip",			_setYFlip },
		{ "spool",				_spool },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITextBox::ReserveCurves ( u32 total ) {

	this->ClearCurves ();
	
	this->mMOAICurves.Init ( total );
	this->mMOAICurves.Fill ( 0 );
	
	this->mCurves.Init ( total );
	this->mCurves.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAITextBox::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {

	MOAIProp2D::SerializeIn ( state, serializer );
	MOAIAction::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAITextBox::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {

	MOAIProp2D::SerializeOut ( state, serializer );
	MOAIAction::SerializeOut ( state, serializer );
}

//----------------------------------------------------------------//
void MOAITextBox::SetCurve ( u32 idx, MOAIAnimCurve* curve ) {

	if ( idx > this->mMOAICurves.Size ()) return;

	curve->Retain ();
	
	if ( this->mMOAICurves [ idx ]) {
		this->mMOAICurves [ idx ]->Release ();
	}
	
	this->mMOAICurves [ idx ] = curve;
	this->mCurves [ idx ] = curve;
}

//----------------------------------------------------------------//
void MOAITextBox::SetFont ( MOAIFont* font ) {

	this->mFont = font;
	this->ClearPageInfo ();
}

//----------------------------------------------------------------//
void MOAITextBox::SetRect ( float left, float top, float right, float bottom ) {

	this->mFrame.Init ( left, top, right, bottom );
	this->ClearPageInfo ();
}

//----------------------------------------------------------------//
void MOAITextBox::SetText ( cc8* text ) {

	this->mStyler.Reset ();

	this->mText = text;
	this->mTextLength = ( u32 )this->mText.length ();
	this->mTextOffset = 0;
	
	this->ClearPageInfo ();
	this->mReveal = REVEAL_ALL;
	this->mSpool = 0.0f;
}

//----------------------------------------------------------------//
void MOAITextBox::SetTextSize( float newSize ) {

	this->mPoints = newSize;
	this->ClearPageInfo ();
}

//----------------------------------------------------------------//
STLString MOAITextBox::ToString () {

	STLString repr;

	// TODO
	return repr;
}
