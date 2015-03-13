// Copyright (c) 2012 Double Fine Productions. All Rights Reserved.

#include "pch.h"
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIProfilerReportBox.h>

#if defined ( MOAI_OS_WINDOWS )
	#include <windows.h>
	#include <Psapi.h>
#endif

#if defined ( MOAI_OS_IPHONE )
	#include <mach/mach.h>
	#include <mach/mach_host.h>
#endif

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize( "", on )
#endif

//================================================================//
// constants
//================================================================//

const float kThresholdAvgDurationMicroSec = 100;

const cc8* kCloumn_Names[] = {

	"name", // COLUMN_NAME,
	"#",	// COLUMN_HITS,
	"%",	// COLUMN_PERCENTAGE,
	"time", // COLUMN_DURATION,
	"avg",	// COLUMN_AVGDURATION,
	"min",	// COLUMN_MINDURATION,
	"max",	// COLUMN_MAXDURATION,
	"count" // COLUMN_COUNT
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getRect
	@text	Returns the two dimensional boundary of the frame.

	@in		MOAIProfilerReportBox self
	@out	number xMin
	@out	number yMin
	@out	number xMax
	@out	number yMax
*/
int MOAIProfilerReportBox::_getRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )

	ZLRect rect = self->mFrame;
	rect.Bless ();
	
	lua_pushnumber ( state, rect.mXMin );
	lua_pushnumber ( state, rect.mYMin );
	lua_pushnumber ( state, rect.mXMax );
	lua_pushnumber ( state, rect.mYMax );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getFont
	@text	Gets the font.
	
	@in		MOAIProfilerReportBox self
	@out	MOAIFont font
*/
int MOAIProfilerReportBox::_getFont ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )

	MOAIFont* font = self->GetFont ();
	if ( font ) {
		font->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getFontSize
	@text	Gets the size of the font.
	
	@in		MOAIProfilerReportBox self
	@out	number size
*/
int MOAIProfilerReportBox::_getFontSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )
	
	lua_pushnumber ( state, self->GetFontSize () );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getBackgroundColor
	@text	Gets the background color.
	
	@in		MOAIProfilerReportBox self
	@out	number r
	@out	number g
	@out	number b
	@out	number a
*/
int MOAIProfilerReportBox::_getBackgroundColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )

	ZLColorVec color = ZLColor::Set ( self->mBackgroundColor );
	
	lua_pushnumber ( state, color.mR );
	lua_pushnumber ( state, color.mG );
	lua_pushnumber ( state, color.mB );
	lua_pushnumber ( state, color.mA );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getRowColor
	@text	Gets the row color.
	
	@in		MOAIProfilerReportBox self
	@out	number r
	@out	number g
	@out	number b
	@out	number a
*/
int MOAIProfilerReportBox::_getRowColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )

	ZLColorVec color = ZLColor::Set ( self->mRowColor );
	
	lua_pushnumber ( state, color.mR );
	lua_pushnumber ( state, color.mG );
	lua_pushnumber ( state, color.mB );
	lua_pushnumber ( state, color.mA );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getTextColor
	@text	Gets the text color.
	
	@in		MOAIProfilerReportBox self
	@out	number r
	@out	number g
	@out	number b
	@out	number a
*/
int MOAIProfilerReportBox::_getTextColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )

	ZLColorVec color = ZLColor::Set ( self->mTextColor );
	
	lua_pushnumber ( state, color.mR );
	lua_pushnumber ( state, color.mG );
	lua_pushnumber ( state, color.mB );
	lua_pushnumber ( state, color.mA );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getOrientation
	@text	Gets the orientation.
	
	@in		MOAIProfilerReportBox self
	@out	number orientation
*/
int MOAIProfilerReportBox::_getOrientation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )
	
	lua_pushnumber ( state, self->GetOrientation () );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Sets the rectangular area for this frame.

	@in		MOAIProfilerReportBox self
	@in		number x1					The X coordinate of the rect's upper-left point.
	@in		number y1					The Y coordinate of the rect's upper-left point.
	@in		number x2					The X coordinate of the rect's lower-right point.
	@in		number y2					The Y coordinate of the rect's lower-right point.
	@out	nil
*/
int MOAIProfilerReportBox::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "UNNNN" )

	float left		= state.GetValue < float >( 2, 0.0f );
	float top		= state.GetValue < float >( 3, 0.0f );
	float right		= state.GetValue < float >( 4, 0.0f );
	float bottom	= state.GetValue < float >( 5, 0.0f );

	self->SetRect ( left, top, right, bottom );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFont
	@text	Sets or clears the style's font.
	
	@in		MOAIProfilerReportBox self
	@opt	MOAIFont font		Default value is nil.
	@out	nil
*/
int MOAIProfilerReportBox::_setFont ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )
	MOAIFont* font = state.GetLuaObject < MOAIFont >( 2, true );
	self->SetFont ( font );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFontSize
	@text	Sets the size of the font.
	
	@in		MOAIProfilerReportBox self
	@in		number size
	@out	nil
*/
int MOAIProfilerReportBox::_setFontSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "UN" )
	self->SetFontSize ( state.GetValue < float >( 2, 0.0f ) );
	return 0;
}

int MOAIProfilerReportBox::_setUserMemory ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "UN" )
	self->SetUserMemory ( state.GetValue < u32 >( 2, 0 ) );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBackgroundColor
	@text	Initialize the background color.
	
	@in		MOAIProfilerReportBox self
	@in		number r	Default value is 0.
	@in		number g	Default value is 0.
	@in		number b	Default value is 0.
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIProfilerReportBox::_setBackgroundColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "UNNN" )
	self->mBackgroundColor = state.GetColor32 ( 2, 0.0f, 0.0f, 0.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRowColor
	@text	Initialize the row color.
	
	@in		MOAIProfilerReportBox self
	@in		number r	Default value is 0.
	@in		number g	Default value is 0.
	@in		number b	Default value is 0.
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIProfilerReportBox::_setRowColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "UNNN" )
	self->mRowColor = state.GetColor32 ( 2, 0.0f, 0.0f, 0.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTextColor
	@text	Initialize the text color.
	
	@in		MOAIProfilerReportBox self
	@in		number r	Default value is 0.
	@in		number g	Default value is 0.
	@in		number b	Default value is 0.
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIProfilerReportBox::_setTextColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "UNNN" )
	self->mTextColor = state.GetColor32 ( 2, 0.0f, 0.0f, 0.0f, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setOrientation
	@text	Sets the orientation.
	
	@in		MOAIProfilerReportBox self
	@in		number orientation	Default value is 0.
	@out	nil
*/
int MOAIProfilerReportBox::_setOrientation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "UN" )
	self->SetOrientation ( state.GetValue < u8 >( 2, 0 ) );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	enableProfiling
	@text	Enables profile stats collection
	
	@in		MOAIProfilerReportBox self
	@out	nil
*/
int MOAIProfilerReportBox::_enableProfiling ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )
	MOAIProfiler::Get ().EnableProfiling ( true );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	disableProfiling
	@text	Disables profile stats collection
	
	@in		MOAIProfilerReportBox self
	@out	nil
*/
int MOAIProfilerReportBox::_disableProfiling ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProfilerReportBox, "U" )
	MOAIProfiler::Get ().EnableProfiling ( false );
	return 0;
}

//================================================================//
// MOAIProfilerReportBox
//================================================================//

//----------------------------------------------------------------//
void MOAIProfilerReportBox::Draw ( int subPrimID, float lod ) {
	UNUSED ( subPrimID );
	UNUSED ( lod );
	
	if ( !mFont ) return;
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	// Get memory stats
	u32 availableMainMem, usedMainMem;
	_GetMemoryStats ( availableMainMem, usedMainMem );
	float relativeMemUsed = availableMainMem > 0 ? (float) usedMainMem / (float) availableMainMem : 0;

	float simRate = ( float )MOAISim::Get ().GetSimDuration () * 1000.0f;
	float renderRate = ( float )MOAIRenderMgr::Get ().GetRenderDuration () * 1000.0f;
	size_t luaMem = MOAILuaRuntime::Get ().GetMemoryUsage ();
	size_t texMem = gfxDevice.GetTextureMemoryUsage ();

	// used for drawing the memory bar
	float memBarWidth = mMemoryXRange.mY - mMemoryXRange.mX;
	float memUsedWidth = memBarWidth * relativeMemUsed;

	const float cellOffsetX = 2;

	// Get current state
	const ZLMatrix4x4& orgUvMatrix = gfxDevice.GetUVTransform ();
	const ZLMatrix4x4& orgWorldTransform = gfxDevice.GetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
	const ZLMatrix4x4& orgViewTransform = gfxDevice.GetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM );
	const ZLMatrix4x4& orgProjTransform = gfxDevice.GetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM );

	MOAIBlendMode origBlendMode = gfxDevice.GetBlendMode ();

	// Set state
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_WORLD, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM );

	float rotation = 0;
	if ( mOrientation == 1 ) {

		rotation = 1.57f;
	}
	else if ( mOrientation == 2 ) {

		rotation = -1.57f;
	}

	// Calculate projection matrix
	ZLMatrix4x4 proj;

	u32 windowWidth = gfxDevice.GetWidth ();
	u32 windowheight = gfxDevice.GetHeight ();

	proj.Ortho ( 2.0f / (float)windowWidth, -2.0f / (float)windowheight, 0, 1 );

	ZLMatrix4x4 offset;
	offset.Translate ( -1, 1, 0 );
	proj.Append ( offset );

	if ( rotation != 0 ) {

		ZLMatrix4x4 rot;
		rot.RotateZ ( rotation );
		proj.Multiply ( orgProjTransform, rot );
	}
		
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM, proj );

	MOAIDraw::Bind ();

	// Get some font metrics
	mFontSize = mFontSize > 0 ? mFontSize : mFont->GetDefaultSize ();
	MOAIGlyphSet* glyphSet = mFont->GetGlyphSet ( mFontSize );
	float fontHeight = glyphSet->GetHeight ();
	mLineHeight = fontHeight + 4;
	mLineOffset = fontHeight + 10;
	
	// Calculate vertical layout
	mSummaryYRange.Init ( this->mFrame.mYMin + 2, this->mFrame.mYMin + 2 + this->mLineHeight );
	mHeaderYRange.Init ( this->mSummaryYRange.mX + mLineOffset, this->mSummaryYRange.mY + 2 + this->mLineHeight );
	mBodyYRange.Init ( this->mHeaderYRange.mX + mLineOffset, this->mFrame.mYMax );

	// Calculate horizonatal layout
	float frameWidth = this->mFrame.mXMax - this->mFrame.mXMin;
	mOverviewXRange.Init ( this->mFrame.mXMin + cellOffsetX, this->mFrame.mXMin + cellOffsetX + frameWidth * 0.3f );
	mMemoryXRange.Init ( this->mOverviewXRange.mY + cellOffsetX, this->mFrame.mXMax - cellOffsetX * 2 );

	mColumns [ COLUMN_NAME ].mX = this->mFrame.mXMin + cellOffsetX;
	mColumns [ COLUMN_NAME ].mWidth = 50;
	mColumns [ COLUMN_HITS ].mWidth = 4;
	mColumns [ COLUMN_PERCENTAGE ].mWidth = 10;
	mColumns [ COLUMN_DURATION ].mWidth = 5;
	mColumns [ COLUMN_AVGDURATION ].mWidth = 5;
	mColumns [ COLUMN_MINDURATION ].mWidth = 5;
	mColumns [ COLUMN_MAXDURATION ].mWidth = 5;

	float widthNorm = 0;
	for ( u32 i = 0; i < COLUMN_COUNT; i++ ) {

		widthNorm += mColumns [ i ].mWidth;
	}
	
	frameWidth = this->mFrame.mXMax - 4 - mColumns [ COLUMN_NAME ].mX - cellOffsetX * COLUMN_COUNT;

	widthNorm = 1.0f / widthNorm;
	for ( u32 i = 0; i < COLUMN_COUNT; i++ ) {

		mColumns [ i ].mWidth = ceil ( mColumns [ i ].mWidth * widthNorm * frameWidth );
	}
	for ( u32 i = 1; i < COLUMN_COUNT; i++ ) {

		mColumns [ i ].mX = mColumns [ i - 1 ].mX + mColumns [ i - 1 ].mWidth + cellOffsetX;
	}

	// Prepare immediate mode drawing
	MOAIDraw::Bind ();

	gfxDevice.SetUVTransform ();
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );

	gfxDevice.SetBlendMode ( ZGL_BLEND_FACTOR_SRC_ALPHA, ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );

	// Draw the table background	
	gfxDevice.SetPenColor ( mBackgroundColor );
	MOAIDraw::DrawRectFill ( this->mFrame.mXMin, this->mFrame.mYMin, this->mFrame.mXMax, this->mFrame.mYMax, false );

	// Draw the summary row
	gfxDevice.SetPenColor ( mRowColor );

	MOAIDraw::DrawRectFill ( mOverviewXRange.mX, mSummaryYRange.mX, mOverviewXRange.mY, mSummaryYRange.mY, false );

	// ToDo: Cache the colors
	ZLColorVec memColorLow, memColorMed, memColorHigh, memColorCur;
	memColorLow.Set ( 0.25f, 0.8f, 0.25f, 0.5f );
	memColorMed.Set ( 0.7f, 0.7f, 0.25f, 0.5f );
	memColorHigh.Set ( 0.8f, 0.25f, 0.25f, 0.5f );

	if ( relativeMemUsed < 0.5f ) {
		memColorCur = memColorLow;
	}
	else {		
		if ( relativeMemUsed < 0.75f ) {
			memColorCur.Lerp ( ZLInterpolate::kLinear, memColorLow, memColorMed, (relativeMemUsed - 0.5f) / 0.25f );
		}
		else {
			memColorCur.Lerp ( ZLInterpolate::kLinear, memColorMed, memColorHigh, (relativeMemUsed - 0.75f) / 0.25f );
		}
	}

	gfxDevice.SetPenColor ( memColorCur );
	MOAIDraw::DrawRectFill ( mMemoryXRange.mX, mSummaryYRange.mX, mMemoryXRange.mX + memUsedWidth, mSummaryYRange.mY, false );
	
	gfxDevice.SetPenColor ( mRowColor );
	MOAIDraw::DrawRectFill (mMemoryXRange.mX + memUsedWidth, mSummaryYRange.mX, mMemoryXRange.mY, mSummaryYRange.mY, false );

	// Draw the rows
	gfxDevice.SetPenColor ( mRowColor );

	mCursorY = mHeaderYRange.mX;	
	for ( u32 i = 0; i < COLUMN_COUNT; i++ ) {

		MOAIDraw::DrawRectFill ( mColumns [ i ].mX, mCursorY, mColumns [ i ].mX + mColumns [ i ].mWidth, mCursorY + mLineHeight, false );
	}

	mCursorY += mLineOffset;
	while ( mCursorY < this->mFrame.mYMax ) {

		MOAIDraw::DrawRectFill ( this->mFrame.mXMin + 2, mCursorY, this->mFrame.mXMax - 4, mCursorY + mLineHeight, false );
		mCursorY += mLineOffset;
	}

	// Draw the text of the profile report
	gfxDevice.SetPenColor ( mTextColor );
	
	// Draw general frame information
	MOAIDraw::BeginDrawString ( 1, *mFont, mFontSize, 0, 0 );
	
	sprintf ( mTemp, "Perf - sim:%.1fms rnd:%.1fms", simRate, renderRate );
	MOAIDraw::DrawString ( mTemp, mOverviewXRange.mX + 3, mSummaryYRange.mX, 0, 0 );

	sprintf ( mTemp, "Mem - lua:%ukb tex:%umb usr:%ukb used:%umb avail:%umb", (u32) (luaMem / 1024), (u32) (texMem / (1024 * 1024)),(mUserMemory / 1024), usedMainMem, availableMainMem );
	MOAIDraw::DrawString ( mTemp, mMemoryXRange.mX + 3, mSummaryYRange.mX, 0, 0 );

	MOAIDraw::DrawString ( kCloumn_Names [ COLUMN_NAME ], mColumns [ COLUMN_NAME ].mX + 3, mHeaderYRange.mX, 0, 0 );

	// Draw the header first
	for ( u32 i = 1; i < COLUMN_COUNT; i++ ) {
		
		MOAIDraw::DrawString ( kCloumn_Names [ i ], mColumns [ i ].mX + 3, mHeaderYRange.mX, 0, 0 );
	}

	MOAIDraw::EndDrawString ();
	
	// Get the current reports and show them
	MOAIProfiler& profiler = MOAIProfiler::Get ();
	
	float cursor = mBodyYRange.mX;

	u32 numProfileReports = profiler.GetNumProfileReports ();
	for ( u32 i = 0; i < numProfileReports; i++ ) {

		MOAIProfilerReport* profileReport = profiler.LockProfileReport ( i );
		if ( profileReport ) {

			mOneOverTotalDurationMicroSec = (float) profileReport->GetTotalDurationMicroSec ();
			mOneOverTotalDurationMicroSec = mOneOverTotalDurationMicroSec > 0 ? 1.0f / mOneOverTotalDurationMicroSec : 0.0f;
			
			gfxDevice.SetPenColor ( mTextColor );

			// ToDo: Prefix the name of the report
			
			MOAIDraw::BeginDrawString ( 1, *mFont, mFontSize, 0, 0 );

			// Draw the content
			mCursorY = cursor;
			profileReport->TraverseProfileEntries ( DrawEntryText, this );

			MOAIDraw::EndDrawString ();

	
			ZLColorVec col;
			col.SetRGBA ( mTextColor );
			col.mA *= 0.5f;
			gfxDevice.SetPenColor ( col.PackRGBA () );

			mCursorY = cursor;
			profileReport->TraverseProfileEntries ( DrawEntryVisuals, this );

			profiler.UnlockProfileReport ( i );

			cursor = mCursorY;
		}
	}

	// TODO: when profile not enabled
	const float memStatX = 0.f;
	const float memStatY = 0.f;
	const float memStatWidth = 600.0f;
	const float memStatHeight = mLineHeight;

	ZLColorVec col;
	col.SetRGBA ( mRowColor );
	col.mA *= 0.5f;
	gfxDevice.SetPenColor ( col );
	MOAIDraw::DrawRectFill ( memStatX, memStatY, memStatX + memStatWidth, memStatY + memStatHeight, false );
	gfxDevice.SetPenColor ( mTextColor );
	sprintf ( mTemp, "Mem - lua:%ukb usr:%ukb used:%umb avail:%umb", (u32) (luaMem / 1024), (mUserMemory / 1024), usedMainMem, availableMainMem );
	MOAIDraw::DrawString ( mTemp, memStatX, memStatY, 1, *mFont, mFontSize, 0, 0, 0, 0 );

	// Restore render state
	gfxDevice.SetUVTransform ( orgUvMatrix );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, orgWorldTransform );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM, orgViewTransform );
	gfxDevice.SetBlendMode ( origBlendMode );
	
	if ( rotation != 0 ) {

		gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM, orgProjTransform );
	}
}

//----------------------------------------------------------------//
u32 MOAIProfilerReportBox::OnGetModelBounds ( ZLBox& bounds ) {

	bounds.Init ( this->mFrame.mXMin, this->mFrame.mYMax, this->mFrame.mXMax, this->mFrame.mYMin, 0.0f, 0.0f );
	return MOAIProp::BOUNDS_OK;
}

//----------------------------------------------------------------//
MOAIProfilerReportBox::MOAIProfilerReportBox () :
	mFont ( 0 ),
	mFontSize ( 0 ),
	mBackgroundColor ( ZLColorVec ( 0, 0, 0, 0.25f ).PackRGBA () ),
	mRowColor ( ZLColorVec ( 0.2f, 0.2f, 0.2f, 0.5f ).PackRGBA () ),
	mTextColor ( ZLColorVec ( 1, 1, 1, 1 ).PackRGBA () ),
	mOrientation ( 0 ),
	mUserMemory ( 0 ),
	mMemoryXRange ( 0, 0 ),
	mLineHeight ( 0 ),
	mSummaryYRange ( 0, 0 ),
	mOverviewXRange ( 0, 0 ),
	mHeaderYRange ( 0, 0 ),
	mBodyYRange ( 0, 0 ),
	mLineOffset ( 0 ),
	mCursorY ( 0 ),
	mOneOverTotalDurationMicroSec ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGraphicsProp )
	RTTI_END
	
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f ); 
	//this->SetMask ( MOAIProp::CAN_DRAW );
	this->mBlendMode.SetBlend ( ZGL_BLEND_FACTOR_SRC_ALPHA, ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
}

//----------------------------------------------------------------//
MOAIProfilerReportBox::~MOAIProfilerReportBox () {
}

//----------------------------------------------------------------//
void MOAIProfilerReportBox::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIGraphicsProp::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getRect",				_getRect },
		{ "getFont",				_getFont },
		{ "getFontSize",			_getFontSize },
		{ "getBackgroundColor",		_getBackgroundColor },
		{ "getRowColor",			_getRowColor },
		{ "getTextColor",			_getTextColor },
		{ "getOrientation",			_getOrientation },

		{ "setRect",				_setRect },
		{ "setFont",				_setFont },
		{ "setUserMemory",          _setUserMemory },
		{ "setFontSize",			_setFontSize },
		{ "setBackgroundColor",		_setBackgroundColor },
		{ "setRowColor",			_setRowColor },
		{ "setTextColor",			_setTextColor },
		{ "setOrientation",			_setOrientation },
		
		{ "enableProfiling",		_enableProfiling },
		{ "disableProfiling",		_disableProfiling },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIProfilerReportBox::SetRect ( float left, float top, float right, float bottom ) {

	this->mFrame.Init ( left, top, right, bottom );
}

//----------------------------------------------------------------//
u32 MOAIProfilerReportBox::DrawEntryText ( MOAIProfilerEntryBase* entry, void* userData ) {

	MOAIProfilerEntry* curEntry = (MOAIProfilerEntry*)entry;

	MOAIProfilerReportBox* self = (MOAIProfilerReportBox*) userData;
	bool bContinue = self->_DrawEntryText ( curEntry );

	return bContinue ? MOAIProfilerReport::TRAVERSAL_CONTINUE : MOAIProfilerReport::TRAVERSAL_STOP;
}

//----------------------------------------------------------------//
u32 MOAIProfilerReportBox::DrawEntryVisuals	( MOAIProfilerEntryBase* entry, void* userData ) {
	
	MOAIProfilerEntry* curEntry = (MOAIProfilerEntry*)entry;

	MOAIProfilerReportBox* self = (MOAIProfilerReportBox*) userData;
	bool bContinue = self->_DrawEntryVisuals ( curEntry );

	return bContinue ? MOAIProfilerReport::TRAVERSAL_CONTINUE : MOAIProfilerReport::TRAVERSAL_STOP;
}

//----------------------------------------------------------------//
bool MOAIProfilerReportBox::_DrawEntryText ( MOAIProfilerEntry* entry ) {

	if ( entry->mAvgDurationMicroSec > kThresholdAvgDurationMicroSec ) {

		// Draw the name of the profile entry
		cc8* entryName = entry->mName.GetString ();
		float offsetX = (float)entry->mDepth * 10;
		MOAIDraw::DrawString ( entryName, mColumns [ COLUMN_NAME ].mX + offsetX, mCursorY + 1, mColumns [ COLUMN_NAME ].mWidth, mLineOffset );
		
		// Draw the values of the profile entry
		offsetX = 1;
		sprintf ( mTemp, "%i", entry->mHitCount );
		MOAIDraw::DrawString ( mTemp, mColumns [ COLUMN_HITS ].mX + offsetX, mCursorY + 1, mColumns [ COLUMN_HITS ].mWidth, mLineOffset );

		sprintf ( mTemp, "%.1f", entry->mTotalDurationMicroSec / 1000.0f );
		MOAIDraw::DrawString ( mTemp, mColumns [ COLUMN_DURATION ].mX + offsetX, mCursorY + 1, mColumns [ COLUMN_DURATION ].mWidth, mLineOffset );

		sprintf ( mTemp, "%.1f", entry->mAvgDurationMicroSec / 1000.0f );
		MOAIDraw::DrawString ( mTemp, mColumns [ COLUMN_AVGDURATION ].mX + offsetX, mCursorY + 1, mColumns [ COLUMN_AVGDURATION ].mWidth, mLineOffset );
		
		sprintf ( mTemp, "%.1f", entry->mMinDurationMicroSec / 1000.0f );
		MOAIDraw::DrawString ( mTemp, mColumns [ COLUMN_MINDURATION ].mX + offsetX, mCursorY + 1, mColumns [ COLUMN_MINDURATION ].mWidth, mLineOffset );

		sprintf ( mTemp, "%.1f", entry->mMaxDurationMicroSec / 1000.0f );
		MOAIDraw::DrawString ( mTemp, mColumns [ COLUMN_MAXDURATION ].mX + offsetX, mCursorY + 1, mColumns [ COLUMN_MAXDURATION ].mWidth, mLineOffset );

		mCursorY += mLineOffset;

		return mCursorY < mBodyYRange.mY;
	}

	return false;
}

//----------------------------------------------------------------//
bool MOAIProfilerReportBox::_DrawEntryVisuals ( MOAIProfilerEntry* entry ) {
	
	if ( entry->mAvgDurationMicroSec > kThresholdAvgDurationMicroSec ) {
		
		// Calculate bounds of the frame
		const float start =  mColumns [ COLUMN_PERCENTAGE ].mX;
		const float width = mColumns [ COLUMN_PERCENTAGE ].mWidth;
		const float end = start + width;

		// Draw the relative duration of the current entry
		MOAIDraw::DrawRectFill ( start + width * (1.0f - entry->mRelativeDuration), mCursorY, end, mCursorY + mLineHeight, false );
		MOAIDraw::DrawRectFill ( start + width * (1.0f - entry->mFrameRelativeDuration), mCursorY, end, mCursorY + mLineHeight, false );
		
		// Prepare the graph of historic values...
		float x = start;
		const float step = width / (float)(MOAIProfilerEntry::NUM_SAMPLES - 1);
		for ( int i = MOAIProfilerEntry::NUM_SAMPLES - 1, j = 0; i >= 0; i--, j += 2, x += step ) {

			float sample = entry->mTotalDurationSamplesMicroSec [ i ] * mOneOverTotalDurationMicroSec;
			sample = min ( max ( sample, 0.0f ), 1.0f );

			mSampleVerts [ j + 0 ] = x;
			mSampleVerts [ j + 1 ] = mCursorY + mLineHeight * (1.0f - sample);
		}
		
		// ...and draw it
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		const ZLColorVec& penColor = gfxDevice.GetPenColor ();

		MOAIDraw::DrawVertexArray2D ( mSampleVerts, MOAIProfilerEntry::NUM_SAMPLES, 0xff0000ff, ZGL_PRIM_LINE_STRIP );

		gfxDevice.SetPenColor ( penColor );

		// Draw the frame
		ZLRect percentageRect;
		percentageRect.Init (  start, mCursorY, start + width, mCursorY + mLineHeight );
		MOAIDraw::DrawRectEdges ( percentageRect, 0xffffffff );

		// Next line
		mCursorY += mLineOffset;

		return mCursorY < mBodyYRange.mY;
	}

	return false;
}

//----------------------------------------------------------------//
void MOAIProfilerReportBox::_GetMemoryStats ( u32& availableMainMem, u32& usedMainMem ) const {
	
	availableMainMem = 0;
	usedMainMem = 0;
	
	#if defined ( MOAI_OS_WINDOWS )

		MEMORYSTATUSEX memStatus;
		memset ( &memStatus, 0, sizeof ( MEMORYSTATUSEX ) );
		memStatus.dwLength = sizeof ( MEMORYSTATUSEX );

		PROCESS_MEMORY_COUNTERS procMemStatus;
		memset ( &procMemStatus, 0, sizeof ( PROCESS_MEMORY_COUNTERS ) );
		procMemStatus.cb = sizeof ( PROCESS_MEMORY_COUNTERS );

		if ( GlobalMemoryStatusEx ( &memStatus ) && GetProcessMemoryInfo ( GetCurrentProcess (), &procMemStatus, procMemStatus.cb ) ) {

			usedMainMem = static_cast< u32 > (procMemStatus.WorkingSetSize / (1024 * 1024));
			availableMainMem = static_cast< u32 > (memStatus.ullAvailVirtual / (1024 * 1024));
		}

	#elif defined ( MOAI_OS_IPHONE )
		
		mach_port_t host_port;
		mach_msg_type_number_t host_size;
		vm_size_t pagesize;

		host_port = mach_host_self();
		host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
		host_page_size(host_port, &pagesize);        

		vm_statistics_data_t vm_stat;

		if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) == KERN_SUCCESS) {

			usedMainMem = static_cast< u32 > (vm_stat.active_count +
											  vm_stat.inactive_count +
											  vm_stat.wire_count) * pagesize;
			availableMainMem = static_cast< u32 > (usedMainMem + (vm_stat.free_count * pagesize));
		}
		
		usedMainMem = usedMainMem / (1024 * 1024);
		availableMainMem = availableMainMem / (1024 * 1024);

		
		task_basic_info info;
		mach_msg_type_number_t size = sizeof( info );
		kern_return_t kerr = task_info( mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size );
		if ( kerr == KERN_SUCCESS )
		{
			usedMainMem = info.resident_size / (1024 * 1024);
			//NSLog( @"task_info: 0x%08lx 0x%08lx\n", info.virtual_size, info.resident_size );
		}
		
	#endif

	// ToDo: Implement other platforms

}

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", off ) 
#endif
