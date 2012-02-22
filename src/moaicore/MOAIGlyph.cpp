// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAIGlyphPage.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAIQuadBrush.h>

//================================================================//
// MOAIGlyph
//================================================================//

//----------------------------------------------------------------//
void MOAIGlyph::Draw ( float x, float y ) const {

	MOAIGlyphPage* page = this->mPage;

	if ( this->mWidth && page ) {
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		gfxDevice.SetTexture ( page->mImageTexture );
		
		MOAIQuadBrush glQuad;
		
		x += this->mBearingX;
		y -= this->mBearingY;

		glQuad.SetVerts (
			x,
			y,
			x + this->mWidth,
			y + this->mHeight
		);
		
		float uScale = this->mPage->mUScale;
		float vScale = this->mPage->mVScale;
		
		float u = this->mSrcX * uScale;
		float v = this->mSrcY * vScale;
		
		glQuad.SetUVs (
			u,
			v,
			u + ( this->mWidth * uScale ),
			v + ( this->mHeight * vScale )
		);
		glQuad.Draw ();
	}
}

//----------------------------------------------------------------//
MOAIKernVec MOAIGlyph::GetKerning ( u32 name ) const {

	u32 total = this->mKernTable.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		MOAIKernVec& kernVec = this->mKernTable [ i ];
		
		if ( kernVec.mName == name ) {
			return kernVec;
		}
	}
	
	MOAIKernVec kernVec;
	kernVec.mName = 0xffffffff;
	kernVec.mX = 0.0f;
	kernVec.mY = 0.0f;
	
	return kernVec;
}

//----------------------------------------------------------------//
USRect MOAIGlyph::GetRect ( float points, float x, float y ) const {

	USRect rect;
		
	x += ( this->mBearingX * points );
	y -= ( this->mBearingY * points ); 

	rect.Init (
		x,
		y,
		x + ( this->mWidth * points ),
		y + ( this->mHeight * points )
	);

	return rect;
}

//----------------------------------------------------------------//
MOAIGlyph::MOAIGlyph () :
	mCode ( 0xffffffff ),
	mWidth ( 0.0f ),
	mHeight ( 0.0f ),
	mAdvanceX ( 0.0f ),
	mBearingX ( 0.0f ),
	mBearingY ( 0.0f ),
	mSrcX ( 0 ),
	mSrcY ( 0 ),
	mPage ( 0 ),
	mNext ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGlyph::~MOAIGlyph () {
}

//----------------------------------------------------------------//
void MOAIGlyph::ReserveKernTable ( u32 total ) {

	this->mKernTable.Init ( total );
}

//----------------------------------------------------------------//
void MOAIGlyph::SerializeIn ( MOAILuaState& state ) {
	UNUSED ( state );
	
	//this->mCode = state.GetField ( -1, "mCode", this->mCode );
	//
	//if ( state.GetFieldWithType ( -1, "mUVRect", LUA_TTABLE )) {
	//
	//	this->mUVRect.mXMin = state.GetField ( -1, "mXMin", 0.0f );
	//	this->mUVRect.mYMin = state.GetField ( -1, "mYMin", 0.0f );
	//	this->mUVRect.mXMax = state.GetField ( -1, "mXMax", 0.0f );
	//	this->mUVRect.mYMax = state.GetField ( -1, "mYMax", 0.0f );
	//	
	//	state.Pop ( 1 );
	//}
	//
	//this->mWidth = state.GetField ( -1, "mWidth", this->mWidth );
	//this->mHeight = state.GetField ( -1, "mHeight", this->mHeight );
	//this->mYOff = state.GetField ( -1, "mYOff", this->mYOff );
	//
	//this->mAdvanceX = state.GetField ( -1, "mAdvanceX", this->mAdvanceX );
	//this->mBearingX = state.GetField ( -1, "mBearingX", this->mBearingX );
	//
	//if ( state.GetFieldWithType ( -1, "mKernTable", LUA_TTABLE )) {
	//	
	//	u32 size = lua_objlen ( state, -1 );
	//	this->mKernTable.Init ( size );
	//	
	//	for ( u32 i = 0; i < size; ++i ) {
	//	
	//		if ( state.GetFieldWithType ( -1, i + 1, LUA_TTABLE )) {
	//			
	//			this->mKernTable [ i ].mName = state.GetField ( -1, "mName", 0 );
	//			this->mKernTable [ i ].mX = state.GetField ( -1, "mX", 0.0f );
	//			this->mKernTable [ i ].mY = state.GetField ( -1, "mY", 0.0f );
	//		}
	//		state.Pop ( 1 );
	//	}
	//	state.Pop ( 1 );
	//}
}

//----------------------------------------------------------------//
void MOAIGlyph::SerializeOut ( MOAILuaState& state ) {
	UNUSED ( state );

	//state.SetField ( -1, "mCode", this->mCode );

	//lua_newtable ( state );
	//state.SetField ( -1, "mXMin", this->mUVRect.mXMin );
	//state.SetField ( -1, "mYMin", this->mUVRect.mYMin );
	//state.SetField ( -1, "mXMax", this->mUVRect.mXMax );
	//state.SetField ( -1, "mYMax", this->mUVRect.mYMax );
	//lua_setfield ( state, -2, "mUVRect" );
	//
	//state.SetField ( -1, "mWidth", this->mWidth );
	//state.SetField ( -1, "mHeight", this->mHeight );
	//state.SetField ( -1, "mYOff", this->mYOff );
	//
	//state.SetField ( -1, "mAdvanceX", this->mAdvanceX );
	//state.SetField ( -1, "mBearingX", this->mBearingX );
	//
	//if ( this->mKernTable.Size ()) {
	//	lua_newtable ( state );
	//	for ( u32 i = 0; i < this->mKernTable.Size (); ++i ) {
	//	
	//		lua_pushnumber ( state, i + 1 );
	//		lua_newtable ( state );
	//		
	//		state.SetField ( -1, "mName", this->mKernTable [ i ].mName );
	//		state.SetField ( -1, "mX", this->mKernTable [ i ].mX );
	//		state.SetField ( -1, "mY", this->mKernTable [ i ].mY );
	//		
	//		lua_settable ( state, -3 );
	//	}
	//	lua_setfield ( state, -2, "mKernTable" );
	//}
}

//----------------------------------------------------------------//
void MOAIGlyph::SetKernVec ( u32 id, const MOAIKernVec& kernVec ) {

	this->mKernTable [ id ] = kernVec;
}

//----------------------------------------------------------------//
void MOAIGlyph::SetScreenRect ( float width, float height, float yOff ) {
	
	this->mWidth = width;
	this->mHeight = height;
	this->mBearingY = -yOff;
}
