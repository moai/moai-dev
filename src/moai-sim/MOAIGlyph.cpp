// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGlyph.h>
#include <moai-sim/MOAIGlyphSet.h>
#include <moai-sim/MOAIDynamicGlyphCachePage.h>
#include <moai-sim/MOAISingleTexture.h>
#include <moai-sim/MOAIQuadBrush.h>

//================================================================//
// MOAIGlyph
//================================================================//

//----------------------------------------------------------------//
void MOAIGlyph::Draw ( MOAISingleTexture& texture, float x, float y, float xScale, float yScale, const ZLRect& padding ) const {
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	if ( !gfxMgr.mGfxState.BindTexture ( &texture )) return;
	
	MOAIQuadBrush glQuad;
	
	x += ( this->mBearingX * xScale );
	y -= ( this->mBearingY * yScale );

	glQuad.SetVerts (
		x + ( padding.mXMin * xScale ),
		y + ( padding.mYMin * yScale ),
		x + (( this->mWidth + padding.mXMax ) * xScale ),
		y + (( this->mHeight + padding.mYMax ) * yScale )
	);
	
	// calculate this dynamically as the texture size may change
	float uScale = 1.0f / texture.GetWidth ();
	float vScale = 1.0f / texture.GetHeight ();
	
	float u = ( this->mSrcX * uScale );
	float v = ( this->mSrcY * vScale );
	
	glQuad.SetUVs (
		u + ( padding.mXMin * uScale ),
		v + ( padding.mYMin * vScale ),
		u + (( this->mWidth + padding.mXMax ) * uScale ),
		v + (( this->mHeight + padding.mYMax ) * vScale )
	);
	glQuad.Draw ();
}

//----------------------------------------------------------------//
ZLRect MOAIGlyph::GetGlyphLogicalRect ( float x, float y, float xScale, float yScale ) const {

	return this->GetLogicalRect ( x, y, this->mDeck->GetAscent (), this->mDeck->GetDescent (), xScale, yScale );
}

//----------------------------------------------------------------//
MOAIKernVec MOAIGlyph::GetKerning ( u32 name ) const {

	size_t total = this->mKernTable.Size ();
	for ( size_t i = 0; i < total; ++i ) {
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
MOAIGlyph::MOAIGlyph () :
	mCode ( NULL_CODE_ID ),
	mPageID ( NULL_PAGE_ID ),
	mSrcX ( 0 ),
	mSrcY ( 0 ),
	mNext ( 0 ),
	mDeck ( 0 ) {
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
		
		int size = ( int )lua_objlen ( state, -1 ); // TODO: cast
		this->mKernTable.Init ( size );
		
		for ( int i = 0; i < size; ++i ) {
		
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
//void MOAIGlyph::SetScreenRect ( float width, float height, float yOff ) {
//	
//	this->mWidth = width;
//	this->mHeight = height;
//	this->mBearingY = -yOff;
//}

//----------------------------------------------------------------//
void MOAIGlyph::SetSourceLoc ( u32 srcX, u32 srcY ) {

	this->mSrcX = srcX;
	this->mSrcY = srcY;
}