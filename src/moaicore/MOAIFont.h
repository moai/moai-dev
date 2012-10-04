// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONT_H
#define	MOAIFONT_H

#include <moaicore/MOAIGlyphSet.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAISpanList.h>

class MOAIFontReader;
class MOAIGlyph;
class MOAIGlyphCacheBase;
class MOAITextureBase;

#define DPI 72
#define POINTS_TO_PIXELS(points,dpi) (( points * dpi ) / DPI )
#define PIXELS_TO_POINTS(pixels,dpi) (( pixels * DPI ) / dpi )

//================================================================//
// MOAIFont
//================================================================//
/**	@name	MOAIFont
	@text	<p>MOAIFont is the top level object for managing sets of
			glyphs associated with a single font face. An instance of
			MOAIFont may contain glyph sets for multiple sizes of the font.
			Alternatively, a separate instance of MOAIFont may be used for each
			font size. Using a single font object for each size of a font face
			can make it easier to unload font sizes that are no longer needed.</p>
			
			<p>An instance of MOAIFont may represent a dynamic or static font. Dynamic fonts
			are used to retrieve glyphs from a font file format on an as-needed basis.
			Static fonts have no associated font file format and therefore contain a fixed
			set of glyphs at runtime. For languages demanding very large character sets
			(such as Chinese), dynamic fonts ae typically used. For languages where it is
			feasible to pre-render a full set of glyphs to texture (or bitmap fonts), static
			fonts may be used.</p>
			
			<p>MOAIFont orchestrates objects derived from MOAIFontReader and MOAIGlyphCacheBase
			to render glyphs into glyph sets. MOAIFontReader is responsible for interpreting
			the font file format (if any), retrieving glyph metrics (including kerning)
			and rendering glyphs to texture. MOAIGlyphCache is responsible for
			allocating textures to hold glyphs and for manageing glyph placement within
			textures. For dyanmic fonts, the typical setup uses MOAIFreeTypeFontReader
			and MOAIGlyphCache. For static fonts, there is usually no font reader;
			MOAIStaticGlyphCache is loaded directly from a serialized file and its texture
			memory is initialized with MOAIFont's setImage () command.</p>
			
			<p>As mentioned, a single MOAIFont may be used to render multiple sizes of a font
			face. When glyphs need to be laid out or rendered, the font object will return
			a set of glyphs matching whatever size was requested. It is also possible to specify
			a default size that will be used if no size is requested for rendering or if no
			matching size is found. If no default size is set by the user, it will be set
			automatically the first time a specific size is requested.</p>
			
			<p>MOAIFont can also control how or if kerning tables are loaded when glyphs are
			being rendered. The default behavior is to load kerning information automatically.
			It is possible to prevent kerning information from being loaded. In this case,
			kerning tables may be loaded manually if so desired.</p>
	
	@const	FONT_AUTOLOAD_KERNING
	@const	DEFAULT_FLAGS
*/
class MOAIFont :
	public MOAILuaObject {
protected:

	STLString mFilename;
	u32 mFlags;
	
	MOAILuaSharedPtr < MOAIFontReader > mReader;
	MOAILuaSharedPtr < MOAIGlyphCacheBase > mCache;
	
	// for now
	typedef STLMap < float, MOAIGlyphSet >::iterator GlyphSetsIt;
	STLMap < float, MOAIGlyphSet > mGlyphSets;

	float mDefaultSize;

	//----------------------------------------------------------------//
	static int			_getFilename			( lua_State* L );
	static int			_getFlags				( lua_State* L );
	static int			_getImage				( lua_State* L );
	static int			_load					( lua_State* L );
	static int			_loadFromBMFont			( lua_State* L );
	static int			_preloadGlyphs			( lua_State* L );
	static int			_rebuildKerningTables	( lua_State* L );
	static int			_setCache				( lua_State* L );
	static int			_setDefaultSize			( lua_State* L );
	static int			_setFlags				( lua_State* L );
	static int			_setImage				( lua_State* L );
	static int			_setReader				( lua_State* L );

	//----------------------------------------------------------------//
	#ifdef DOXYGEN
		static int		_loadFromTTF			( lua_State* L );
	#endif

	//----------------------------------------------------------------//
	void				BuildKerning			( MOAIGlyph* glyphs, MOAIGlyph* pendingGlyphs );
	void				RebuildKerning			( MOAIGlyphSet& glyphSet );

public:
	
	DECL_LUA_FACTORY ( MOAIFont )
	
	GET ( cc8*, Filename, mFilename );
	GET ( MOAIGlyphCacheBase*, Cache, mCache );
	
	enum {
		FONT_AUTOLOAD_KERNING		= 0x01,
	};
	
	static const u32 DEFAULT_FLAGS = FONT_AUTOLOAD_KERNING;
	
	//----------------------------------------------------------------//
	void				AffirmGlyph				( float size, u32 c );
	MOAIGlyphSet&		AffirmGlyphSet			( float size );
	MOAIGlyphSet*		GetGlyphSet				( float size );
	MOAITextureBase*	GetGlyphTexture			( MOAIGlyph& glyph );
	void				Init					( cc8* filename );
	void				InitWithBMFont			( cc8* filename );
	static bool			IsControl				( u32 c );
	static bool			IsWhitespace			( u32 c );
						MOAIFont				();
						~MOAIFont				();
	void				ProcessGlyphs			();
	void				RebuildKerning			();
	void				RebuildKerning			( float size );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ResizePageList			( u32 size );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
