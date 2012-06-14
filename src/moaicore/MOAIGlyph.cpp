// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAIGlyphCachePage.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAIQuadBrush.h>

//================================================================//
// MOAIGlyph
//================================================================//

//----------------------------------------------------------------//
void MOAIGlyph::Draw ( MOAITextureBase& texture, float x, float y, float scale ) const {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.SetTexture ( &texture );
	
	MOAIQuadBrush glQuad;
	
	x += this->mBearingX * scale;
	y -= this->mBearingY * scale;

	glQuad.SetVerts (
		x,
		y,
		x + ( this->mWidth * scale ),
		y + ( this->mHeight * scale )
	);
	
	float uScale = 1.0f / texture.GetWidth ();
	float vScale = 1.0f / texture.GetHeight ();
	
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
USRect MOAIGlyph::GetRect ( float x, float y ) const {

	USRect rect;
		
	x += ( this->mBearingX );
	y -= ( this->mBearingY ); 

	rect.Init (
		x,
		y,
		x + this->mWidth,
		y + this->mHeight
	);

	return rect;
}

//----------------------------------------------------------------//
MOAIGlyph::MOAIGlyph () :
	mCode ( 0xffffffff ),
	mPageID ( NULL_PAGE_ID ),
	mWidth ( 0.0f ),
	mHeight ( 0.0f ),
	mAdvanceX ( 0.0f ),
	mBearingX ( 0.0f ),
	mBearingY ( 0.0f ),
	mSrcX ( 0 ),
	mSrcY ( 0 ),
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

	this->mCode			= state.GetField ( -1, "mCode", this->mCode );
	this->mPageID		= state.GetField ( -1, "mPageID", this->mPageID );
	
	this->mWidth		= state.GetField ( -1, "mWidth", this->mWidth );
	this->mHeight		= state.GetField ( -1, "mHeight", this->mHeight );
	this->mAdvanceX		= state.GetField ( -1, "mAdvanceX", this->mAdvanceX );
	this->mBearingX		= state.GetField ( -1, "mBearingX", this->mBearingX );
	this->mBearingY		= state.GetField ( -1, "mBearingY", this->mBearingY );

	this->mSrcX			= state.GetField ( -1, "mSrcX", this->mSrcX );
	this->mSrcY			= state.GetField ( -1, "mSrcY", this->mSrcY );
	
	if ( state.GetFieldWithType ( -1, "mKernTable", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mKernTable.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
		
			if ( state.GetFieldWithType ( -1, i + 1, LUA_TTABLE )) {
				
				this->mKernTable [ i ].mName = state.GetField ( -1, "mName", 0 );
				this->mKernTable [ i ].mX = state.GetField ( -1, "mX", 0.0f );
				this->mKernTable [ i ].mY = state.GetField ( -1, "mY", 0.0f );
			}
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAIGlyph::SerializeOut ( MOAILuaState& state ) {

	state.SetField ( -1, "mCode", this->mCode );
	state.SetField ( -1, "mPageID", this->mPageID );

	state.SetField ( -1, "mWidth", this->mWidth );
	state.SetField ( -1, "mHeight", this->mHeight );
	state.SetField ( -1, "mAdvanceX", this->mAdvanceX );
	state.SetField ( -1, "mBearingX", this->mBearingX );
	state.SetField ( -1, "mBearingY", this->mBearingY );
	
	state.SetField ( -1, "mSrcX", this->mSrcX );
	state.SetField ( -1, "mSrcY", this->mSrcY );
	
	if ( this->mKernTable.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mKernTable.Size (); ++i ) {
		
			lua_pushnumber ( state, i + 1 );
			lua_newtable ( state );
			
			state.SetField ( -1, "mName", this->mKernTable [ i ].mName );
			state.SetField ( -1, "mX", this->mKernTable [ i ].mX );
			state.SetField ( -1, "mY", this->mKernTable [ i ].mY );
			
			lua_settable ( state, -3 );
		}
		lua_setfield ( state, -2, "mKernTable" );
	}
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

//----------------------------------------------------------------//
void MOAIGlyph::SetSourceLoc ( u32 srcX, u32 srcY ) {

	this->mSrcX = srcX;
	this->mSrcY = srcY;
}