// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIGlyphCacheBase.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIFontReader.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGlyphCacheBase.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIStaticGlyphCache.h>
#include <moaicore/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_getFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	state.Push ( self->mFlags );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_getImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )

	if ( self->mCache ) {
		MOAIImage* image = self->mCache->GetImage ();
		if ( image ) {
			state.Push ( image );
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Sets the font file for use when loading glyphs.

	@in		MOAIFont self
	@in		string filename			The path to the TTF file to load.
	@out	nil
*/
int MOAIFont::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "US" )

	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	self->Init ( filename );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	preloadGlyphs
	@text	Loads and caches glyphs for quick access later.

	@in		MOAIFont self
	@in		string charCodes		A string which defines the characters found in the this->
	@in		number points			The point size to be rendered onto the internal texture.
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
*/
int MOAIFont::_preloadGlyphs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "USN" )

	cc8* charCodes	= state.GetValue < cc8* >( 2, "" );
	float points	= state.GetValue < float >( 3, 0 );
	float dpi		= state.GetValue < float >( 4, DPI );
	
	float size = POINTS_TO_PIXELS ( points, dpi );
	
	int idx = 0;
	while ( charCodes [ idx ]) {
		u32 c = u8_nextchar ( charCodes, &idx );
		self->AffirmGlyph ( size, c );
	}
	self->ProcessGlyphs ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_rebuildKerningTables ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	
	if ( self->mReader ) {
	
		if ( state.IsType ( 2, LUA_TNUMBER )) {
			
			float points	= state.GetValue < float >( 2, 0 );
			float dpi		= state.GetValue < float >( 3, DPI );
			
			float size = POINTS_TO_PIXELS ( points, dpi );
			self->RebuildKerning ( size );
		}
		else {
			self->RebuildKerning ();
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_setCache ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	self->mCache.Set ( *self, state.GetLuaObject < MOAIGlyphCacheBase >( 2 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_setFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	self->mFlags = state.GetValue < u32 >( 2, DEFAULT_FLAGS );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_setImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "UU" )

	if ( !self->mCache ) {
		MOAIGlyphCacheBase* glyphCache = new MOAIStaticGlyphCache ();
		self->mCache.Set ( *self, glyphCache );
	}

	assert ( self->mCache );

	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2 );
	if ( image ) {
		self->mCache->SetImage ( *image );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_setReader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	self->mReader.Set ( *self, state.GetLuaObject < MOAIFontReader >( 2 ));
	return 0;
}

//================================================================//
// MOAIFont
//================================================================//

//----------------------------------------------------------------//
void MOAIFont::AffirmGlyph ( float points, u32 c ) {

	MOAIGlyphSet& glyphSet = this->mGlyphSets [ points ];
	glyphSet.mPoints = points;
	glyphSet.AffirmGlyph ( c );
}

//----------------------------------------------------------------//
// iterate through the pending glyphs in each set and attempt to
// update them to match target - i.e. metrics or metrics and bitmap
void MOAIFont::BuildKerning ( MOAIGlyph* glyphs, MOAIGlyph* pendingGlyphs ) {

	if ( !this->mReader->HasKerning ()) return;
	MOAIKernVec kernTable [ MOAIGlyph::MAX_KERN_TABLE_SIZE ];

	// iterate over the orignal glyphs and add kerning info for new glyphs
	for ( MOAIGlyph* glyphIt = glyphs; glyphIt; glyphIt = glyphIt->mNext ) {
		MOAIGlyph& glyph = *glyphIt;
		
		u32 kernTableSize = 0;
		u32 oldTableSize = glyph.mKernTable.Size ();
		
		// iterate over just the new glyphs; check each one against olf glyphs for kerning info
		for ( MOAIGlyph* glyphIt2 = pendingGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			// skip if glyph is already in old glyph's kerning table
			// may happen if glyphs are purged and then re-added
			bool unknown = true;
			for ( u32 i = 0; i < oldTableSize; ++i ) {
				if ( glyph.mKernTable [ i ].mName == glyph2.mCode ) {
					unknown = false;
					break;
				}
			} 
			
			if ( unknown ) {
				MOAIKernVec kernVec;
				if ( this->mReader->GetKernVec ( glyph, glyph2, kernVec )) {
					assert ( kernTableSize < MOAIGlyph::MAX_KERN_TABLE_SIZE );
					kernTable [ kernTableSize++ ] = kernVec;
				}
			}
		}
		
		// resize the old kerning table and copy in the new kern vecs (if any)
		if ( kernTableSize ) {
			glyph.mKernTable.Resize ( oldTableSize + kernTableSize );
			memcpy ( &glyph.mKernTable [ oldTableSize ], kernTable, sizeof ( MOAIKernVec ) * kernTableSize );
		}
	}
	
	// iterate over the new glyphs and add kerning info for all glyphs
	for ( MOAIGlyph* glyphIt = pendingGlyphs; glyphIt; glyphIt = glyphIt->mNext ) {
		MOAIGlyph& glyph = *glyphIt;
		
		u32 kernTableSize = 0;
		
		// iterate over the original glyphs
		for ( MOAIGlyph* glyphIt2 = glyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			MOAIKernVec kernVec;
			if ( this->mReader->GetKernVec ( glyph, glyph2, kernVec )) {
				assert ( kernTableSize < MOAIGlyph::MAX_KERN_TABLE_SIZE );
				kernTable [ kernTableSize++ ] = kernVec;
			}
		}
		
		// iterate over the new glyphs
		for ( MOAIGlyph* glyphIt2 = pendingGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			MOAIKernVec kernVec;
			if ( this->mReader->GetKernVec ( glyph, glyph2, kernVec )) {
				assert ( kernTableSize < MOAIGlyph::MAX_KERN_TABLE_SIZE );
				kernTable [ kernTableSize++ ] = kernVec;
			}
		}
		
		// init the kern table
		if ( kernTableSize ) {
			glyph.mKernTable.Init ( kernTableSize );
			memcpy ( glyph.mKernTable, kernTable, sizeof ( MOAIKernVec ) * kernTableSize );
		}
	}
}

//----------------------------------------------------------------//
MOAIGlyphSet* MOAIFont::GetGlyphDeck ( float points ) {

	return &this->mGlyphSets [ points ];
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIFont::GetGlyphTexture ( MOAIGlyph& glyph ) {

	assert ( this->mCache );
	return this->mCache->GetGlyphTexture ( glyph );
}

//----------------------------------------------------------------//
void MOAIFont::Init ( cc8* filename ) {

	this->mFilename = filename;
}

//----------------------------------------------------------------//
bool MOAIFont::IsControl ( u32 c ) {

	if ( !c ) return true;
	if ( c == '\t' ) return true;
	if ( c == '\n' ) return true;
	
	return false;
}

//----------------------------------------------------------------//
bool MOAIFont::IsWhitespace ( u32 c ) {

	if ( c == ' ' ) return true;
	
	return false;
}

//----------------------------------------------------------------//
MOAIFont::MOAIFont () :
	mFlags ( DEFAULT_FLAGS ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFont::~MOAIFont () {

	this->mReader.Set ( *this, 0 );
	this->mCache.Set ( *this, 0 );
}

//----------------------------------------------------------------//
// iterate through the pending glyphs in each set and attempt to
// update them to match target - i.e. metrics or metrics and bitmap
void MOAIFont::ProcessGlyphs () {

	if ( !this->mReader ) return;

	this->mReader->OpenFont ( *this );
	
	MOAIFont::GlyphSetsIt glyphSetsIt = this->mGlyphSets.begin ();
	for ( ; glyphSetsIt != this->mGlyphSets.end (); ++glyphSetsIt ) {
		MOAIGlyphSet& glyphSet = glyphSetsIt->second;
		
		// save pointers to the two glyph lists
		MOAIGlyph* glyphs = glyphSet.mGlyphs;
		MOAIGlyph* pendingGlyphs = glyphSet.mPending;
		
		// all pending glyphs will be moved to the processed glyphs list
		// so clear the pending glyphs list
		glyphSet.mPending = 0;
		
		// if no pending glyphs, move on to the next deck
		if ( !pendingGlyphs ) continue;
		
		// get the face metrics
		this->mReader->SetFaceSize ( glyphSet.mPoints );
		this->mReader->GetFaceMetrics ( glyphSet );
		
		// build kerning tables (if face has kerning info)
		if (( this->mFlags & FONT_AUTOLOAD_KERNING ) && this->mReader->HasKerning ()) {
			this->BuildKerning ( glyphs, pendingGlyphs );
		}
		
		//----------------------------------------------------------------//
		// render the new glyphs and move them to the processed list
		for ( MOAIGlyph* glyphIt = pendingGlyphs; glyphIt; ) {
			MOAIGlyph& glyph = *glyphIt;
			glyphIt = glyphIt->mNext;
			
			// move the glyph into the processed glyphs list
			glyph.mNext = glyphSet.mGlyphs;
			glyphSet.mGlyphs = &glyph;
			
			this->mReader->RenderGlyph ( *this, glyph );
		}
	}

	this->mReader->CloseFont ();
}

//----------------------------------------------------------------//
void MOAIFont::RebuildKerning () {

	if ( !this->mReader ) return;
	if ( !this->mReader->HasKerning ()) return;
	if ( !this->mGlyphSets.size ()) return;

	this->mReader->OpenFont ( *this );
	
	MOAIFont::GlyphSetsIt glyphSetsIt = this->mGlyphSets.begin ();
	for ( ; glyphSetsIt != this->mGlyphSets.end (); ++glyphSetsIt ) {
		MOAIGlyphSet& glyphSet = glyphSetsIt->second;
		this->RebuildKerning ( glyphSet );
	}
	
	this->mReader->CloseFont ();
}

//----------------------------------------------------------------//
void MOAIFont::RebuildKerning ( float points ) {

	if ( !this->mReader ) return;
	if ( !this->mReader->HasKerning ()) return;
	if ( !this->mGlyphSets.contains ( points )) return;
	
	this->mReader->OpenFont ( *this );
	
	MOAIGlyphSet& glyphSet = this->mGlyphSets [ points ];
	this->RebuildKerning ( glyphSet );
	
	this->mReader->CloseFont ();
}

//----------------------------------------------------------------//
void MOAIFont::RebuildKerning ( MOAIGlyphSet& glyphSet ) {
	
	MOAIKernVec kernTable [ MOAIGlyph::MAX_KERN_TABLE_SIZE ];
	
	// get the face metrics
	this->mReader->SetFaceSize ( glyphSet.mPoints );

	// iterate over the orignal glyphs and add kerning info for new glyphs
	for ( MOAIGlyph* glyphIt = glyphSet.mGlyphs; glyphIt; glyphIt = glyphIt->mNext ) {
		MOAIGlyph& glyph = *glyphIt;

		u32 kernTableSize = 0;
		
		// iterate over just the new glyphs; check each one against old glyphs for kerning info
		for ( MOAIGlyph* glyphIt2 = glyphSet.mGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			MOAIKernVec kernVec;
			if ( this->mReader->GetKernVec ( glyph, glyph2, kernVec )) {
				assert ( kernTableSize < MOAIGlyph::MAX_KERN_TABLE_SIZE );
				kernTable [ kernTableSize++ ] = kernVec;
			}
		}
		
		// init (or clear) the kern table
		glyph.mKernTable.Resize ( kernTableSize );
		
		// copy in the new kern vecs (if any)
		if ( kernTableSize ) {
			memcpy ( glyph.mKernTable, kernTable, sizeof ( MOAIKernVec ) * kernTableSize );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "DEFAULT_FLAGS",			( u32 )DEFAULT_FLAGS );
	state.SetField ( -1, "FONT_AUTOLOAD_KERNING",	( u32 )FONT_AUTOLOAD_KERNING );
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getImage",					_getImage },
		{ "getFlags",					_getFlags },
		{ "load",						_load },
		{ "preloadGlyphs",				_preloadGlyphs },	
		{ "rebuildKerningTables",		_rebuildKerningTables },
		{ "setCache",					_setCache },
		{ "setFlags",					_setFlags },
		{ "setImage",					_setImage },
		{ "setReader",					_setReader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFont::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	this->mFilename = state.GetField ( -1, "mFilename", this->mFilename );
	this->mFlags = state.GetField ( -1, "mFlags", this->mFlags );
	
	if ( state.GetFieldWithType ( -1, "mGlyphSets", LUA_TTABLE )) {

		u32 itr = state.PushTableItr ( -1 );
		while ( state.TableItrNext ( itr )) {
			float size = state.GetValue < float >( -2, 0 );
			MOAIGlyphSet& glyphSet = this->mGlyphSets [ size ];
			glyphSet.SerializeIn ( state );
		}
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAIFont::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	state.SetField ( -1, "mFilename", this->mFilename );
	state.SetField ( -1, "mFlags", this->mFlags );
	
	lua_newtable ( state );
	GlyphSetsIt glyphSetsIt = this->mGlyphSets.begin ();
	for ( ; glyphSetsIt != this->mGlyphSets.end (); ++glyphSetsIt ) {
	
		float size = glyphSetsIt->first;
		MOAIGlyphSet& glyphSet = glyphSetsIt->second;
	
		lua_pushnumber ( state, size );
		lua_newtable ( state );
		glyphSet.SerializeOut ( state );
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mGlyphSets" );
}

