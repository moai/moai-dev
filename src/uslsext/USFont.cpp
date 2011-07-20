// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USAnimCurve.h>
#include <uslsext/USBinarySearch.h>
#include <uslsext/USFont.h>
#include <uslsext/USGLQuad.h>
#include <uslsext/USRadixSort32.h>
#include <contrib/utf8.h>

//================================================================//
// USGlyph
//================================================================//

//----------------------------------------------------------------//
void USGlyph::Draw ( float points, float x, float y ) const {

	if ( this->mWidth ) {

		USGLQuad glQuad;
		
		x += ( this->mBearingX * points );
		y += ( this->mYOff * points ); 

		glQuad.SetVerts (
			x,
			y,
			x + ( this->mWidth * points ),
			y + ( this->mHeight * points )
		);

		glQuad.SetUVs ( this->mUVRect );
		glQuad.Draw ();
	}
}

//----------------------------------------------------------------//
USKernVec USGlyph::GetKerning ( u32 name ) const {

	u32 total = this->mKernTable.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		USKernVec& kernVec = this->mKernTable [ i ];
		
		if ( kernVec.mName == name ) {
			return kernVec;
		}
	}
	
	USKernVec kernVec;
	kernVec.mName = 0xffffffff;
	kernVec.mX = 0.0f;
	kernVec.mY = 0.0f;
	
	return kernVec;
}

//----------------------------------------------------------------//
USRect USGlyph::GetRect ( float points, float x, float y ) const {

	USRect rect;
		
	x += ( this->mBearingX * points );
	y += ( this->mYOff * points ); 

	rect.Init (
		x,
		y,
		x + ( this->mWidth * points ),
		y + ( this->mHeight * points )
	);

	return rect;
}

//----------------------------------------------------------------//
void USGlyph::ReserveKernTable ( u32 total ) {

	this->mKernTable.Init ( total );
}

//----------------------------------------------------------------//
void USGlyph::SerializeIn ( USLuaState& state ) {
	
	this->mCode = state.GetField ( -1, "mCode", this->mCode );
	
	if ( state.GetFieldWithType ( -1, "mUVRect", LUA_TTABLE )) {
	
		this->mUVRect.mXMin = state.GetField ( -1, "mXMin", 0.0f );
		this->mUVRect.mYMin = state.GetField ( -1, "mYMin", 0.0f );
		this->mUVRect.mXMax = state.GetField ( -1, "mXMax", 0.0f );
		this->mUVRect.mYMax = state.GetField ( -1, "mYMax", 0.0f );
		
		state.Pop ( 1 );
	}
	
	this->mWidth = state.GetField ( -1, "mWidth", this->mWidth );
	this->mHeight = state.GetField ( -1, "mHeight", this->mHeight );
	this->mYOff = state.GetField ( -1, "mYOff", this->mYOff );
	
	this->mAdvanceX = state.GetField ( -1, "mAdvanceX", this->mAdvanceX );
	this->mBearingX = state.GetField ( -1, "mBearingX", this->mBearingX );
	
	if ( state.GetFieldWithType ( -1, "mKernTable", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mKernTable.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mKernTable [ i ].mName = state.GetValue < u32 >( -1, 0 );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void USGlyph::SerializeOut ( USLuaState& state ) {

	state.SetField ( -1, "mCode", this->mCode );

	lua_newtable ( state );
	state.SetField ( -1, "mXMin", this->mUVRect.mXMin );
	state.SetField ( -1, "mYMin", this->mUVRect.mYMin );
	state.SetField ( -1, "mXMax", this->mUVRect.mXMax );
	state.SetField ( -1, "mYMax", this->mUVRect.mYMax );
	lua_setfield ( state, -2, "mUVRect" );
	
	state.SetField ( -1, "mWidth", this->mWidth );
	state.SetField ( -1, "mHeight", this->mHeight );
	state.SetField ( -1, "mYOff", this->mYOff );
	
	state.SetField ( -1, "mAdvanceX", this->mAdvanceX );
	state.SetField ( -1, "mBearingX", this->mBearingX );
	
	if ( this->mKernTable.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mKernTable.Size (); ++i ) {
			state.SetFieldByIndex ( -1, i + 1, this->mKernTable [ i ].mName );
		}
		lua_setfield ( state, -2, "mKernTable" );
	}
}

//----------------------------------------------------------------//
void USGlyph::SetKernVec ( u32 id, const USKernVec& kernVec ) {

	this->mKernTable [ id ] = kernVec;
}

//----------------------------------------------------------------//
void USGlyph::SetScreenRect ( float width, float height, float yOff ) {
	
	this->mWidth = width;
	this->mHeight = height;
	this->mYOff = yOff;
}

//----------------------------------------------------------------//
USGlyph::USGlyph () :
	mCode ( 0xffffffff ),
	mWidth ( 0.0f ),
	mHeight ( 0.0f ),
	mYOff ( 0.0f ),
	mAdvanceX ( 0.0f ),
	mBearingX ( 0.0f ) {
}

//----------------------------------------------------------------//
USGlyph::~USGlyph () {
}

//================================================================//
// USFont
//================================================================//

//----------------------------------------------------------------//
void USFont::DrawGlyph ( u32 c, float points, float x, float y ) {

	const USGlyph& glyph = this->GetGlyphForChar ( c );
	glyph.Draw ( points, x, y );
}

//----------------------------------------------------------------//
USGlyph& USFont::GetGlyphForChar ( u32 c ) {

	u32 id = this->GetIDForChar ( c );
	return this->GetGlyphForID ( id );
}

//----------------------------------------------------------------//
USGlyph& USFont::GetGlyphForID ( u32 id ) {

	if ( id == INVALID_ID ) {
		return this->mDummy;
	}

	if ( id & WIDE_ID_BIT ) {
		return this->mWideGlyphs [ id & WIDE_ID_MASK ];
	}
	return this->mByteGlyphs [ id ];
}

//----------------------------------------------------------------//
u32 USFont::GetIDForChar ( u32 c ) {

	if ( this->IsWideChar ( c )) {
		
		// TODO: replace sorted lookup w/ AVL tree
		u32 size = this->mWideGlyphMap.Size ();
		u32 id = USBinarySearch < u32 >( this->mWideGlyphMap, c, size );
		if ( id < size ) {
			return id | WIDE_ID_BIT;
		}
	}
	else {
		if ( this->mByteGlyphMapBase <= c ) {
			c -= this->mByteGlyphMapBase;
			if ( c < this->mByteGlyphMap.Size ()) {
				return this->mByteGlyphMap [ c ];
			}
		}
	}
	return INVALID_ID;
}

//----------------------------------------------------------------//
void USFont::Init ( cc8* charCodes ) {

	u32 byteCharTop = 0;
	u32 byteCharBase = 0x000000ff;
	u32 totalWideChars = 0;
	
	for ( int i = 0; charCodes [ i ]; ) {
		u32 c = u8_nextchar( charCodes, &i );
		
		if ( this->IsWideChar ( c )) {
			totalWideChars++;
		}
		else {
			
			if ( c < byteCharBase ) {
				byteCharBase = c;
			}
			if ( c > byteCharTop ) {
				byteCharTop = c;
			}
		}
	}
	
	byteCharTop += 1;
	u32 totalByteChars = ( byteCharBase < byteCharTop ) ? byteCharTop - byteCharBase : 0;
	
	this->mByteGlyphs.Init ( totalByteChars );
	this->mByteGlyphMap.Init ( totalByteChars );
	this->mByteGlyphMapBase = ( u8 )byteCharBase;
	
	this->mWideGlyphs.Init ( totalWideChars );
	this->mWideGlyphMap.Init ( totalWideChars );
	
	u32 b = 0;
	u32 w = 0;
	for ( int i = 0; charCodes [ i ]; ) {
		
		u32 c = u8_nextchar( charCodes, &i );
		
		if ( this->IsWideChar ( c )) {
			this->mWideGlyphMap [ w++ ] = c;
		}
		else {
			this->mByteGlyphMap [ c - this->mByteGlyphMapBase ] = ( u8 )b++;
		}
	}
	RadixSort32 < u32 >( this->mWideGlyphMap, totalWideChars );
}

//----------------------------------------------------------------//
bool USFont::IsWideChar ( u32 c ) {

	return ( c & 0xffffff00 ) != 0;
}

//----------------------------------------------------------------//
void USFont::SerializeIn ( USLuaState& state ) {
	UNUSED ( state );
	
	if ( state.GetFieldWithType ( -1, "mByteGlyphs", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mByteGlyphs.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mByteGlyphs [ i ].SerializeIn ( state );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
	
	if ( state.GetFieldWithType ( -1, "mByteGlyphMap", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mByteGlyphMap.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mByteGlyphMap [ i ] = state.GetValue < u8 >( -1, 0 );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
	
	this->mByteGlyphMapBase		= state.GetField ( -1, "mByteGlyphMapBase", this->mByteGlyphMapBase );
	
	if ( state.GetFieldWithType ( -1, "mWideGlyphs", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mWideGlyphs.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mWideGlyphs [ i ].SerializeIn ( state );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
	
	if ( state.GetFieldWithType ( -1, "mWideGlyphMap", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mWideGlyphMap.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mWideGlyphMap [ i ] = state.GetValue < u32 >( -1, 0 );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
	
	this->mScale				= state.GetField ( -1, "mScale", this->mScale );
	this->mLineSpacing			= state.GetField ( -1, "mLineSpacing", this->mLineSpacing );
}

//----------------------------------------------------------------//
void USFont::SerializeOut ( USLuaState& state ) {
	
	if ( this->mByteGlyphs.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mByteGlyphs.Size (); ++i ) {
			lua_pushnumber ( state, i + 1 );
			lua_newtable ( state );
			this->mByteGlyphs [ i ].SerializeOut ( state );
			lua_settable ( state, -3 );
		}
		lua_setfield ( state, -2, "mByteGlyphs" );
	}
	
	if ( this->mByteGlyphMap.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mByteGlyphMap.Size (); ++i ) {
			state.SetFieldByIndex ( -1, i + 1, this->mByteGlyphMap [ i ]);
		}
		lua_setfield ( state, -2, "mByteGlyphMap" );
	}

	state.SetField ( -1, "mByteGlyphMapBase", this->mByteGlyphMapBase );
	
	if ( this->mWideGlyphs.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mWideGlyphs.Size (); ++i ) {
			lua_pushnumber ( state, i + 1 );
			lua_newtable ( state );
			this->mWideGlyphs [ i ].SerializeOut ( state );
			lua_settable ( state, -3 );
		}
		lua_setfield ( state, -2, "mWideGlyphs" );
	}
	
	if ( this->mWideGlyphMap.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mWideGlyphMap.Size (); ++i ) {
			state.SetFieldByIndex ( -1, i + 1, this->mWideGlyphMap [ i ]);
		}
		lua_setfield ( state, -2, "mWideGlyphMap" );
	}
	
	state.SetField ( -1, "mScale", this->mScale );
	state.SetField ( -1, "mLineSpacing", this->mLineSpacing );
}

//----------------------------------------------------------------//
void USFont::SetGlyph ( const USGlyph& glyph ) {

	u32 id = this->GetIDForChar ( glyph.mCode );
	if ( id != INVALID_ID ) {
	
		if ( glyph.mAdvanceX > this->mDummy.mAdvanceX ) {
			this->mDummy.mAdvanceX = glyph.mAdvanceX;
		}
	
		this->GetGlyphForID ( id ) = glyph;
	}
}

//----------------------------------------------------------------//
u32 USFont::Size () {

	return this->mByteGlyphs.Size () + this->mWideGlyphs.Size ();
}

//----------------------------------------------------------------//
USFont::USFont () :
	mByteGlyphMapBase ( 0 ),
	mScale ( 1.0f ),
	mLineSpacing ( 1.0f ) {
}

//----------------------------------------------------------------//
USFont::~USFont () {
}
