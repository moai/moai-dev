// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGlyphSet.h>

//================================================================//
// MOAIGlyphSet
//================================================================//

//----------------------------------------------------------------//
const MOAIGlyph& MOAIGlyphSet::AffirmGlyph ( u32 c ) {

	if ( !this->mGlyphMap.contains ( c )) {
	
		MOAIGlyph& glyph = this->mGlyphMap [ c ];
		glyph.mNext = this->mPending;
		this->mPending = &glyph;
		glyph.mCode = c;
		
		return glyph;
	}
	return this->mGlyphMap [ c ];
}

//----------------------------------------------------------------//
MOAIGlyph& MOAIGlyphSet::EditGlyph ( u32 c ) {

	if ( !this->mGlyphMap.contains ( c )) {
	
		MOAIGlyph& glyph = this->mGlyphMap [ c ];
		glyph.mNext = this->mGlyphs;
		this->mGlyphs = &glyph;
		glyph.mCode = c;
		
		return glyph;
	}
	return this->mGlyphMap [ c ];
}

//----------------------------------------------------------------//
MOAIGlyph* MOAIGlyphSet::GetGlyph ( u32 c ) {

	return &this->mGlyphMap [ c ];
}

//----------------------------------------------------------------//
MOAIGlyphSet::MOAIGlyphSet () :
	mSize ( 0.0f ),
	mHeight ( 0.0f ),
	mAscent ( 0.0f ),
	mPending ( 0 ),
	mGlyphs ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGlyphSet::~MOAIGlyphSet (){
}

//----------------------------------------------------------------//
void MOAIGlyphSet::SerializeIn ( MOAILuaState& state ) {
	UNUSED ( state );
	
	this->mSize		= state.GetField ( -1, "mSize", this->mSize );
	this->mHeight	= state.GetField ( -1, "mHeight", this->mHeight );
	this->mAscent	= state.GetField ( -1, "mAscent", this->mAscent );

	if ( state.GetFieldWithType ( -1, "mGlyphMap", LUA_TTABLE )) {

		u32 itr = state.PushTableItr ( -1 );
		while ( state.TableItrNext ( itr )) {
			u32 c = state.GetValue < u32 >( -2, 0 );
			MOAIGlyph& glyph = this->mGlyphMap [ c ];
			glyph.SerializeIn ( state );
		}
		state.Pop ( 1 );
	}
	
	GlyphMapIt glyphMapIt = this->mGlyphMap.begin ();
	for ( ; glyphMapIt != this->mGlyphMap.end (); ++glyphMapIt ) {
		MOAIGlyph& glyph = glyphMapIt->second;
		
		if ( glyph.mPageID == MOAIGlyph::NULL_PAGE_ID ) {
			glyph.mNext = this->mPending;
			this->mPending = &glyph;
		}
		else {
			glyph.mNext = this->mGlyphs;
			this->mGlyphs = &glyph;
		}
	}
}

//----------------------------------------------------------------//
void MOAIGlyphSet::SerializeOut ( MOAILuaState& state ) {
	UNUSED ( state );
	
	state.SetField ( -1, "mSize", this->mSize );
	state.SetField ( -1, "mHeight", this->mHeight );
	state.SetField ( -1, "mAscent", this->mAscent );

	lua_newtable ( state );
	GlyphMapIt glyphMapIt = this->mGlyphMap.begin ();
	for ( ; glyphMapIt != this->mGlyphMap.end (); ++glyphMapIt ) {
	
		u32 c = glyphMapIt->first;
		MOAIGlyph& glyph = glyphMapIt->second;
	
		lua_pushnumber ( state, c );
		lua_newtable ( state );
		glyph.SerializeOut ( state );
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mGlyphMap" );
}