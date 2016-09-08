// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIFontReader.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGlyphCache.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIImageTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIStaticGlyphCache.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getCache
	@text	Returns glyph cache.
	
	@in		MOAIFont self
	@out	MOAILuaObject cache
*/
int MOAIFont::_getCache ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	state.Push (( MOAILuaObject* )self->mCache );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getDefaultSize
	@text	Requests the font's default size
	
	@in		MOAIFont self
	@out	number defaultSize
*/
int MOAIFont::_getDefaultSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	state.Push ( self->mDefaultSize );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFilename
	@text	Returns the filename of the font.
	
	@in		MOAIFont self
	@out	string name
*/
int MOAIFont::_getFilename ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	state.Push ( self->mFilename );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFlags
	@text	Returns the current flags.
	
	@in		MOAIFont self
	@out	number flags
*/
int MOAIFont::_getFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	state.Push ( self->mFlags );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getImage
	@text	Requests a 'glyph map image' from the glyph cache currently
			attached to the font. The glyph map image stitches together the
			texture pages used by the glyph cache to produce a single image
			that represents a snapshot of all of the texture memory being
			used by the font.
	
	@in		MOAIFont self
	@out	MOAIImage image
*/
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
/**	@lua	getCache
	@text	Returns font reader.
	
	@in		MOAIFont self
	@out	MOAILuaObject reader
*/
int MOAIFont::_getReader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	state.Push (( MOAILuaObject* )self->mReader );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	load
	@text	Sets the filename of the font for use when loading glyphs.

	@in		MOAIFont self
	@in		string filename			The path to the font file to load.
	@out	nil
*/
int MOAIFont::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "US" )

	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	self->Init ( filename );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	loadFromBMFont
	@text	Sets the filename of the font for use when loading a BMFont.
 
	@in		MOAIFont self
	@in		string filename			The path to the BMFont file to load.
	@opt	table textures			Table of preloaded textures.
	@out	nil
*/
int	MOAIFont::_loadFromBMFont ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "US" )
	
	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	self->Init ( filename );

	// Check if there are preloaded textures
	MOAITexture** preloadedTextures = 0;
	u16 numPreloadedTextures = ( u16 )lua_objlen ( state, 3 );

	if ( numPreloadedTextures > 0 ) {

		preloadedTextures = new MOAITexture* [ numPreloadedTextures ];
		memset ( preloadedTextures, 0, sizeof ( MOAITexture* ) * numPreloadedTextures );
		
		// Get all the preloaded textures
		for ( u16 i=0; i<numPreloadedTextures; i++ ) {
			lua_pushinteger ( state, i + 1 );
			lua_gettable ( state, -2 );
			
			MOAITexture* texture = state.GetLuaObject < MOAITexture >( -1, true );
			preloadedTextures [i] = texture;

			lua_pop ( state, 1 );
		}
	}

	self->InitWithBMFont ( filename, numPreloadedTextures, preloadedTextures );

	delete [] preloadedTextures;

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	preloadGlyphs
	@text	Loads and caches glyphs for quick access later.

	@in		MOAIFont self
	@in		string charCodes		A string which defines the characters found in the this->
	@in		number points			The point size to be rendered onto the internal texture.
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
*/
int MOAIFont::_preloadGlyphs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )

	cc8* charCodes	= state.GetValue < cc8* >( 2, "" );
	float points	= state.GetValue < float >( 3, 0 );
	float dpi		= state.GetValue < float >( 4, DPI );
	
	float size = POINTS_TO_PIXELS ( points, dpi );
	
	if ( self->mDefaultSize == 0.0f ) {
		self->mDefaultSize = size;
	}
	
	int idx = 0;
	while ( charCodes [ idx ]) {
		u32 c = moai_u8_nextchar ( charCodes, &idx );
		self->AffirmGlyph ( size, c );
	}
	self->ProcessGlyphs ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	rebuildKerningTables
	@text	Forces a full reload of the kerning tables for either a single
			glyph set within the font (if a size is specified) or for all
			glyph sets in the font.
	
	@overload
	
		@in		MOAIFont self
		@out	nil
	
	@overload
	
		@in		MOAIFont self
		@in		number points			The point size to be rendered onto the internal texture.
		@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
		@out	nil
*/
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
/**	@lua	setCache
	@text	Attaches or clears the glyph cache associated with the font.
			The cache is an object derived from MOAIGlyphCache and may be
			a dynamic cache that can allocate space for new glyphs on an
			as-needed basis or a static cache that only supports direct
			loading of glyphs and glyph textures through MOAIFont's
			setImage () command.

	@in		MOAIFont self
	@opt	MOAIGlyphCache cache		Default value is nil.
	@out	nil
*/
int MOAIFont::_setCache ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	self->mCache.Set ( *self, state.GetLuaObject < MOAIGlyphCache >( 2, true ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDefaultSize
	@text	Selects a glyph set size to use as the default size when no
			other size is specified by objects wishing to use MOAIFont to
			render text.

	@in		MOAIFont self
	@in		number points			The point size to be rendered onto the internal texture.
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
*/
int MOAIFont::_setDefaultSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	
	float points	= state.GetValue < float >( 2, 0 );
	float dpi		= state.GetValue < float >( 3, DPI );
			
	self->mDefaultSize = POINTS_TO_PIXELS ( points, dpi );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFilter
	@text	Sets the filtering mode for glyph textures.
	
	@in		MOAIFont self
	@opt	number minFilter
	@out	number magFilter
	@out	MOAILuaObject cache
*/
int MOAIFont::_setFilter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	
	int min = state.GetValue < int >( 2, ZGL_SAMPLE_LINEAR );
	int mag = state.GetValue < int >( 3, min );
	
	MOAISingleTexture::CheckFilterModes ( min, mag );
	
	self->mMinFilter = min;
	self->mMagFilter = mag;

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFlags
	@text	Set flags to control font loading behavior. Right now the
			only supported flag is FONT_AUTOLOAD_KERNING which may be used
			to enable automatic loading of kern tables. This flag is initially
			true by default.

	@in		MOAIFont self
	@opt	number flags			Flags are FONT_AUTOLOAD_KERNING or DEFAULT_FLAGS. DEFAULT_FLAGS is the same as FONT_AUTOLOAD_KERNING.
									Alternatively, pass '0' to clear the flags.
	@out	nil
*/
int MOAIFont::_setFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	self->mFlags = state.GetValue < u32 >( 2, DEFAULT_FLAGS );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setImage
	@text	Passes an image to the glyph cache currently attached to the font.
			The image will be used to recreate and initialize the texture memory
			managed by the glyph cache and used by the font. It will not affect
			any glyph entires that have already been laid out and stored in
			the glyph cache.
			
			If no cache is attached to the font, an instance of MOAIStaticGlyphCache
			will automatically be allocated.
	
	@in		MOAIFont self
	@in		MOAIImage image
	@out	nil
*/
int MOAIFont::_setImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "UU" )

	if ( !self->mCache ) {
		MOAIGlyphCache* glyphCache = new MOAIStaticGlyphCache ();
		self->mCache.Set ( *self, glyphCache );
	}

	assert ( self->mCache );

	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2, true );
	if ( image ) {
		self->mCache->SetImage ( *self, *image );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setReader
	@text	Attaches or clears the MOAIFontReader associated with the font.
			MOAIFontReader is responsible for loading and rendering glyphs from
			a font file on demand. If you are using a static font and do not
			need a reader, set this field to nil.

	@in		MOAIFont self
	@opt	MOAIFontReader reader		Default value is nil.
	@out	nil
*/
int MOAIFont::_setReader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	self->mReader.Set ( *self, state.GetLuaObject < MOAIFontReader >( 2, true ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setShader
	@text	Set the preferred shader for the font. (May be overridden by
			a prop.)

	@in		MOAIFont self
	@in		MOAIShader shader
	@out	MOAIShader shader
*/
int MOAIFont::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )

	MOAIShader* shader = MOAIShader::AffirmShader ( state, 2 );
	self->mShader.Set ( *self, shader );

	state.Push ( shader );
	return 1;
}

//================================================================//
// DOXYGEN
//================================================================//

#ifdef DOXYGEN

	//----------------------------------------------------------------//
	/**	@lua	loadFromTTF
		@text	Preloads a set of glyphs from a TTF or OTF. Included for
				backward compatibility. May be removed in a future release.
		
		@in		MOAIFont self
		@in		string filename
		@in		string charcodes
		@in		number points			The point size to be loaded from the TTF.
		@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
		@out	nil
	*/
	int MOAIFont::_loadFromTTF ( lua_State* L ) {
	}

#endif

//================================================================//
// MOAIFont
//================================================================//

//----------------------------------------------------------------//
void MOAIFont::AffirmGlyph ( float size, u32 c ) {

	size = size > 0.0f ? size : this->mDefaultSize;
	if ( size ==  0.0f ) return;

	if ( this->mCache && this->mCache->IsDynamic ()) {
		MOAIGlyphSet& glyphSet = this->AffirmGlyphSet ( size );
		glyphSet.AffirmGlyph ( c );
	}
}

//----------------------------------------------------------------//
MOAIGlyphSet& MOAIFont::AffirmGlyphSet ( float size ) {

	assert ( size > 0.0f );

	MOAIGlyphSet& glyphSet = this->mGlyphSets [ size ];
	glyphSet.mSize = size;
	
	if ( this->mDefaultSize <= 0.0f ) {
		this->mDefaultSize = size;
	}
	return glyphSet;
}

//----------------------------------------------------------------//
// iterate through the pending glyphs in each set and attempt to
// update them to match target - i.e. metrics or metrics and bitmap
void MOAIFont::BuildKerning ( MOAIGlyph* glyphs, MOAIGlyph* pendingGlyphs ) {

	if ( !this->mReader->HasKerning ()) return;
	MOAIKernVec kernTable [ MOAIGlyph::MAX_KERN_TABLE_SIZE ]; // TODO: handle overflow cases

	// iterate over the orignal glyphs and add kerning info for new glyphs
	for ( MOAIGlyph* glyphIt = glyphs; glyphIt; glyphIt = glyphIt->mNext ) {
		MOAIGlyph& glyph = *glyphIt;
		
		size_t kernTableSize = 0;
		size_t oldTableSize = glyph.mKernTable.Size ();
		
		this->mReader->SelectGlyph ( glyph.mCode );
		
		// iterate over just the new glyphs; check each one against olf glyphs for kerning info
		for ( MOAIGlyph* glyphIt2 = pendingGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			// skip if glyph is already in old glyph's kerning table
			// may happen if glyphs are purged and then re-added
			bool unknown = true;
			for ( size_t i = 0; i < oldTableSize; ++i ) {
				if ( glyph.mKernTable [ i ].mName == glyph2.mCode ) {
					unknown = false;
					break;
				}
			} 
			
			if ( unknown ) {
				MOAIKernVec kernVec;
				if ( this->mReader->GetKernVec ( glyph2.mCode, kernVec )) {
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
		
		size_t kernTableSize = 0;
		
		this->mReader->SelectGlyph ( glyph.mCode );
		
		// iterate over the original glyphs
		for ( MOAIGlyph* glyphIt2 = glyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			MOAIKernVec kernVec;
			if ( this->mReader->GetKernVec ( glyph2.mCode, kernVec )) {
				assert ( kernTableSize < MOAIGlyph::MAX_KERN_TABLE_SIZE );
				kernTable [ kernTableSize++ ] = kernVec;
			}
		}
		
		// iterate over the new glyphs
		for ( MOAIGlyph* glyphIt2 = pendingGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			MOAIKernVec kernVec;
			if ( this->mReader->GetKernVec ( glyph2.mCode, kernVec )) {
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
MOAIGlyphSet* MOAIFont::GetGlyphSet ( float size ) {

	if (( size > 0.0f ) && this->mGlyphSets.contains ( size )) {
		return &this->mGlyphSets [ size ];
	}
	else if ( size == 0.0f ) {
		if ( this->mDefaultSize <= 0.0f ) return 0;
		return &this->mGlyphSets [ this->mDefaultSize ];
	}
	else if ( this->mGlyphSets.size ()) {
		
		MOAIFont::GlyphSetsIt glyphSetsIt = this->mGlyphSets.begin ();
		MOAIGlyphSet* bestMatch = &( glyphSetsIt++ )->second;
		float bestSizeDiff = ABS ( size - bestMatch->mSize );
		
		for ( ; glyphSetsIt != this->mGlyphSets.end (); ++glyphSetsIt ) {
			MOAIGlyphSet& glyphSet = glyphSetsIt->second;
			float sizeDiff = ABS ( size - glyphSet.mSize );
			
			if ( sizeDiff < bestSizeDiff ) {
				bestMatch = &glyphSet;
				bestSizeDiff = sizeDiff;
			}
		}
		return bestMatch;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAISingleTexture* MOAIFont::GetGlyphTexture ( MOAIGlyph& glyph ) {

	assert ( this->mCache );
	return this->mCache->GetGlyphTexture ( glyph );
}

//----------------------------------------------------------------//
void MOAIFont::Init ( cc8* filename ) {

	this->mFilename = ZLFileSys::GetAbsoluteFilePath ( filename );

	if ( !ZLFileSys::CheckFileExists ( filename )) {
		ZLLog_Warning ( "WARNING: font file %s does not exist\n", filename );
	}
}

//----------------------------------------------------------------//
bool MOAIFont::IsControl ( u32 c ) {

	if ( !c ) return true;
	if ( c == '\t' ) return true;
	if ( c == '\r' ) return true;
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
	mFlags ( DEFAULT_FLAGS ),
	mDefaultSize ( 0.0f ),
	mMinFilter ( ZGL_SAMPLE_LINEAR ),
	mMagFilter ( ZGL_SAMPLE_LINEAR ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		RTTI_EXTEND ( MOAIInstanceEventSource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFont::~MOAIFont () {

	this->mReader.Set ( *this, 0 );
	this->mCache.Set ( *this, 0 );
	this->mShader.Set ( *this, 0 );
}

//----------------------------------------------------------------//
// iterate through the pending glyphs in each set and attempt to
// update them to match target - i.e. metrics or metrics and bitmap
void MOAIFont::ProcessGlyphs () {

	// this function gets called frequenty to process any pending glyphs on the fly.
	// for that reason, it should exit quickly if there are no pending glyphs.

	MOAIFontReader* fontReader = this->mReader;
	if ( !fontReader ) return;

	bool fontIsOpen = false;
	
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
		
		// only open the font here as we know that we have pending glyphs to process
		if ( !fontIsOpen ) {
			fontIsOpen = this->mReader->OpenFontFile ( this->mFilename ) == MOAIFontReader::OK;
		}

		if ( !fontIsOpen ) {
			ZLLog_Error ( "ERROR: unable to open font file %s for reading glyphs.\n", this->mFilename.c_str ());
			return;
		}
		
		// get the face metrics
		fontReader->SelectFace ( glyphSet.mSize );
		fontReader->GetFaceMetrics ( glyphSet );
		
		// build kerning tables (if face has kerning info)
		if (( this->mFlags & FONT_AUTOLOAD_KERNING ) && this->mReader->HasKerning ()) {
			this->BuildKerning ( glyphs, pendingGlyphs );
		}
		
		// render the new glyphs and move them to the processed list
		for ( MOAIGlyph* glyphIt = pendingGlyphs; glyphIt; ) {
			MOAIGlyph& glyph = *glyphIt;
			glyphIt = glyphIt->mNext;
			
			// move the glyph into the processed glyphs list
			glyph.mNext = glyphSet.mGlyphs;
			glyphSet.mGlyphs = &glyph;
			
			fontReader->SelectGlyph ( glyph.mCode );
			fontReader->GetGlyphMetrics ( glyph );
						
			// place and render the glyph
			this->RenderGlyph ( glyph );
		}
	}

	if ( fontIsOpen ) {
		this->mReader->CloseFontFile ();
	}
}

//----------------------------------------------------------------//
void MOAIFont::RebuildKerning () {

	if ( !this->mReader ) return;
	if ( !this->mGlyphSets.size ()) return;
	
	if ( this->mReader->OpenFontFile ( this->mFilename ) == MOAIFontReader::OK ) {
		if ( this->mReader->HasKerning ()) {
		
			MOAIFont::GlyphSetsIt glyphSetsIt = this->mGlyphSets.begin ();
			for ( ; glyphSetsIt != this->mGlyphSets.end (); ++glyphSetsIt ) {
				MOAIGlyphSet& glyphSet = glyphSetsIt->second;
				this->RebuildKerning ( glyphSet );
			}
		}
		this->mReader->CloseFontFile ();
	}
}

//----------------------------------------------------------------//
void MOAIFont::RebuildKerning ( float size ) {

	if ( !this->mReader ) return;
	if ( !this->mReader->HasKerning ()) return;
	if ( !this->mGlyphSets.contains ( size )) return;
	
	if ( this->mReader->OpenFontFile ( this->mFilename ) == MOAIFontReader::OK ) {
		MOAIGlyphSet& glyphSet = this->mGlyphSets [ size ];
		this->RebuildKerning ( glyphSet );
		this->mReader->CloseFontFile ();
	}
}

//----------------------------------------------------------------//
void MOAIFont::RebuildKerning ( MOAIGlyphSet& glyphSet ) {
	
	MOAIKernVec kernTable [ MOAIGlyph::MAX_KERN_TABLE_SIZE ];
	
	// get the face metrics
	this->mReader->SelectFace ( glyphSet.mSize );

	u32 kernTableSize = 0;

	// iterate over the orignal glyphs and add kerning info for new glyphs
	for ( MOAIGlyph* glyphIt = glyphSet.mGlyphs; glyphIt; glyphIt = glyphIt->mNext ) {
		MOAIGlyph& glyph = *glyphIt;
		
		this->mReader->SelectGlyph ( glyph.mCode );
		
		// iterate over just the new glyphs; check each one against old glyphs for kerning info
		for ( MOAIGlyph* glyphIt2 = glyphSet.mGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			MOAIKernVec kernVec;
			if ( this->mReader->GetKernVec ( glyph2.mCode, kernVec ) == MOAIFontReader::OK ) {
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
	MOAIInstanceEventSource::RegisterLuaClass ( state );
	
	state.SetField ( -1, "EVENT_RENDER_GLYPH",		( u32 )EVENT_RENDER_GLYPH );
	
	state.SetField ( -1, "DEFAULT_FLAGS",			( u32 )DEFAULT_FLAGS );
	state.SetField ( -1, "FONT_AUTOLOAD_KERNING",	( u32 )FONT_AUTOLOAD_KERNING );
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIInstanceEventSource::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getCache",					_getCache },
		{ "getDefaultSize",				_getDefaultSize },
		{ "getFlags",					_getFlags },
		{ "getFilename",				_getFilename },
		{ "getImage",					_getImage },
		{ "getReader",					_getReader },
		{ "load",						_load },
		{ "loadFromBMFont",				_loadFromBMFont },
		{ "preloadGlyphs",				_preloadGlyphs },	
		{ "rebuildKerningTables",		_rebuildKerningTables },
		{ "setCache",					_setCache },
		{ "setDefaultSize",				_setDefaultSize },
		{ "setFilter",					_setFilter },
		{ "setFlags",					_setFlags },
		{ "setImage",					_setImage },
		{ "setReader",					_setReader },
		{ "setShader",					_setShader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFont::RenderGlyph ( MOAIGlyph& glyph ) {

	MOAIFontReader* fontReader = this->mReader;
	if ( !fontReader ) return;
	
	MOAIGlyphCache* glyphCache = this->GetCache ();
	if ( !( glyphCache && glyphCache->IsDynamic ())) return;

	glyphCache->PlaceGlyph ( *this, glyph );

	MOAIImage* image = glyphCache->GetGlyphImage ( glyph );
	if ( image ) {

		float x = glyph.mSrcX - glyph.mBearingX;
		float y = glyph.mSrcY + glyph.mBearingY;

		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->PushListenerAndSelf ( EVENT_RENDER_GLYPH, state )) {
			
			state.Push ( fontReader );
			state.Push ( image );
		
			state.Push ( glyph.GetCode ());
		
			state.Push ( x );
			state.Push ( y );

			state.Push ( glyph.GetSrcX ());
			state.Push ( glyph.GetSrcY ());
			state.Push ( glyph.GetSrcX () + glyph.GetWidth ());
			state.Push ( glyph.GetSrcY () + glyph.GetHeight ());
		
			state.DebugCall ( 10, 0 );
		}
		else {
			fontReader->RenderGlyph ( *image, x, y );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFont::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	this->mFilename = state.GetField ( -1, "mFilename", this->mFilename );
	this->mFlags = state.GetField ( -1, "mFlags", this->mFlags );
	this->mDefaultSize = state.GetField ( -1, "mDefaultSize", this->mDefaultSize );
	
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
	state.SetField ( -1, "mDefaultSize", this->mDefaultSize );
	
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
