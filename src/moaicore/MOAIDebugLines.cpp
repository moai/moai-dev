// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIDebugLineStyle
//================================================================//

//----------------------------------------------------------------//
MOAIDebugLineStyle::MOAIDebugLineStyle () :
	mVisible ( false ),
	mColor ( 0xffffffff ),
	mSize ( 1 ) {
}

//----------------------------------------------------------------//
MOAIDebugLineStyle::~MOAIDebugLineStyle () {
}

//================================================================//
// MOAIDebugLine
//================================================================//

//----------------------------------------------------------------//
void MOAIDebugLine::Draw () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.SetPenColor ( this->mColor );
	gfxDevice.SetPenWidth ( this->mWidth );

	gfxDevice.BeginPrim ();
	
		gfxDevice.WriteVtx ( this->mVtx [ 0 ]);
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( this->mVtx [ 1 ]);
		gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDebugLine::SetVerts ( float x0, float y0, float x1, float y1 ) {

	this->mVtx [ 0 ].mX = x0;
	this->mVtx [ 0 ].mY = y0;
	this->mVtx [ 0 ].mZ = 0.0f;
	
	this->mVtx [ 1 ].mX = x1;
	this->mVtx [ 1 ].mY = y1;
	this->mVtx [ 1 ].mZ = 0.0f;
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	setStyle
	@text	Sets the particulars of a given debug line style.
	
	@in		number styleID		See MOAIDebugLines class documentation for a list of styles.
	@opt	number size			Pen size (in pixels) for the style. Default value is 1.
	@opt	number r			Red component of line color. Default value is 1.
	@opt	number g			Green component of line color. Default value is 1.
	@opt	number b			Blue component of line color. Default value is 1.
	@opt	number a			Alpha component of line color. Default value is 1.
	@out	nil
*/
int MOAIDebugLines::_setStyle ( lua_State* L ) {
	
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	u32 styleID		= state.GetValue < u32 >( 1, 0 );
	u32 size		= state.GetValue < u32 >( 2, 1 );
	float r			= state.GetValue < float >( 3, 1.0f );
	float g			= state.GetValue < float >( 4, 1.0f );
	float b			= state.GetValue < float >( 5, 1.0f );
	float a			= state.GetValue < float >( 6, 1.0f );
	
	u32 color = USColor::PackRGBA ( r, g, b, a );
	
	MOAIDebugLines::Get ().SetStyle ( styleID, size, color );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showStyle
	@text	Enables of disables drawing of a given debug line style.
	
	@in		number styleID		See MOAIDebugLines class documentation for a list of styles.
	@opt	boolean show		Default value is 'true'
	@out	nil
*/
int MOAIDebugLines::_showStyle ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	u32 styleID		= state.GetValue < u32 >( 1, 0 );
	bool show		= state.GetValue < bool >( 2, true );
	
	MOAIDebugLines::Get ().ShowStyle ( styleID, show );
	
	return 0;
}

//================================================================//
// MOAIDebugLines
//================================================================//

//----------------------------------------------------------------//
bool MOAIDebugLines::Bind ( u32 styleID ) {

	MOAIDebugLineStyle& style = this->mStyles [ styleID ];
	
	if ( style.mVisible ) {
		this->SetPenColor ( style.mColor );
		this->SetPenWidth ( style.mSize );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIDebugLines::Draw () {

	if ( !this->mTop ) return;
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetTexture ();
	gfxDevice.SetBlendMode ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	gfxDevice.SetShaderPreset ( MOAIShaderMgr::LINE_SHADER );
	
	gfxDevice.SetPrimType ( GL_LINES );
	gfxDevice.SetVertexPreset ( MOAIVertexFormatMgr::XYZWC );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	
	for ( u32 i = 0; i < this->mTop; ++i ) {
		this->mLineBuffer [ i ].Draw ();
	}
	
	this->Reset ();
}

//----------------------------------------------------------------//
void MOAIDebugLines::DrawEllipse ( USVec2D& loc, float xRad, float yRad, u32 steps ) {
#if USE_OPENGLES1
	static const u32 MAX = 64;
	if ( steps > MAX ) steps = MAX;
	USVec2D vtx [ MAX * 2 ];
	
	glVertexPointer ( 2, GL_FLOAT, 0, vtx );
	glEnableClientState ( GL_VERTEX_ARRAY );

	float angle = ( float )TWOPI / ( float )steps;
	float angleStep = ( float )PI;
	
	for ( u32 i = 0; i < steps; ++i, angleStep += angle ) {
		
		vtx [ i ].mX = loc.mX + ( Sin ( angleStep ) * xRad );
		vtx [ i ].mY = loc.mY + ( Cos ( angleStep ) * yRad );
	}
	
	for ( u32 i = 0; i < steps; ++i ) {
		
		USVec2D& v0 = vtx [ i ];
		USVec2D& v1 = vtx [( i + 1 ) % steps ];
		
		this->DrawLine ( v0.mX, v0.mY, v1.mX, v1.mY );
	}
#endif
}

//----------------------------------------------------------------//
void MOAIDebugLines::DrawLine ( const USVec2D& v0, const USVec2D& v1 ) {

	this->DrawLine ( v0.mX, v0.mY, v1.mX, v1.mY );
}

//----------------------------------------------------------------//
void MOAIDebugLines::DrawLine ( float x0, float y0, float x1, float y1 ) {

	MOAIDebugLine* line = this->NextLine ();
	if ( line ) {

		if ( this->mPenSpace == MODEL_SPACE ) {

			USVec2D v0 ( x0, y0 );
			USVec2D v1 ( x1, y1 );

			this->mModelToWorldMtx.Transform ( v0 );
			this->mModelToWorldMtx.Transform ( v1 );
	
			line->SetVerts ( v0.mX, v0.mY, v1.mX, v1.mY );
		}
		else {
		
			line->SetVerts ( x0, y0, x1, y1 );
		}
		
		line->mColor = this->mPenColor;
		line->mWidth = ( float )this->mPenWidth;
	}
}

//----------------------------------------------------------------//
void MOAIDebugLines::DrawRect ( const USRect& rect ) {

	this->DrawLine ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMin );
	this->DrawLine ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMax );
	this->DrawLine ( rect.mXMin, rect.mYMin, rect.mXMin, rect.mYMax );
	this->DrawLine ( rect.mXMax, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIDebugLines::DrawQuad ( const USQuad& quad ) {

	this->DrawLine ( quad.mV [ 0 ].mX, quad.mV [ 0 ].mY, quad.mV [ 1 ].mX, quad.mV [ 1 ].mY );
	this->DrawLine ( quad.mV [ 1 ].mX, quad.mV [ 1 ].mY, quad.mV [ 2 ].mX, quad.mV [ 2 ].mY );
	this->DrawLine ( quad.mV [ 2 ].mX, quad.mV [ 2 ].mY, quad.mV [ 3 ].mX, quad.mV [ 3 ].mY );
	this->DrawLine ( quad.mV [ 3 ].mX, quad.mV [ 3 ].mY, quad.mV [ 0 ].mX, quad.mV [ 0 ].mY );
}

//----------------------------------------------------------------//
bool MOAIDebugLines::IsVisible ( u32 styleID ) {

	return this->mStyles [ styleID ].mVisible;
}

//----------------------------------------------------------------//
MOAIDebugLines::MOAIDebugLines () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->Reset ();
}

//----------------------------------------------------------------//
MOAIDebugLines::~MOAIDebugLines () {
}

//----------------------------------------------------------------//
MOAIDebugLine* MOAIDebugLines::NextLine () {

	if ( !this->mLineBuffer.Size ()) {
		this->mLineBuffer.Init ( MAX_LINES );
	}

	if ( this->mTop < this->mLineBuffer.Size ()) {
		return &this->mLineBuffer [ this->mTop++ ];
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIDebugLines::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "setStyle",			_setStyle },
		{ "showStyle",			_showStyle },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
	
	state.SetField ( -1, "PARTITION_CELLS",			( u32 )PARTITION_CELLS );
	state.SetField ( -1, "PARTITION_PADDED_CELLS",	( u32 )PARTITION_PADDED_CELLS );
	state.SetField ( -1, "PROP_MODEL_BOUNDS",		( u32 )PROP_MODEL_BOUNDS );
	state.SetField ( -1, "PROP_WORLD_BOUNDS",		( u32 )PROP_WORLD_BOUNDS );
	state.SetField ( -1, "TEXT_BOX",				( u32 )TEXT_BOX );
	state.SetField ( -1, "TEXT_BOX_BASELINES",		( u32 )TEXT_BOX_BASELINES );
	state.SetField ( -1, "TEXT_BOX_LAYOUT",			( u32 )TEXT_BOX_LAYOUT );
}

//----------------------------------------------------------------//
void MOAIDebugLines::Reset () {

	this->mTop = 0;
	this->mModelToWorldMtx.Ident ();
	this->SetPen ( 0xffffffff, 1.0f, MODEL_SPACE );
}

//----------------------------------------------------------------//
void MOAIDebugLines::SetPen ( u32 penColor, float penWidth, u32 penSpace ) {

	this->mPenColor = penColor;
	this->mPenWidth = ( u32 )penWidth;
	this->mPenSpace = penSpace;
}

//----------------------------------------------------------------//
void MOAIDebugLines::SetStyle ( u32 styleID, u32 size, u32 color ) {

	MOAIDebugLineStyle& style = this->mStyles [ styleID ];
	
	style.mVisible = true;
	style.mColor = color;
	style.mSize = size;
}

//----------------------------------------------------------------//
void MOAIDebugLines::SetWorldMtx () {

	this->mModelToWorldMtx.Ident ();
}

//----------------------------------------------------------------//
void MOAIDebugLines::SetWorldMtx ( const USAffine3D& mtx ) {

	this->mModelToWorldMtx = mtx;
}


//----------------------------------------------------------------//
void MOAIDebugLines::ShowStyle ( u32 styleID, bool show ) {

	this->mStyles [ styleID ].mVisible = show;
}
