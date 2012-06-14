// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHCACHE_H
#define	MOAIGLYPHCACHE_H

#include <moaicore/MOAIGlyphCacheBase.h>

class MOAIFont;
class MOAIGlyphCachePage;

//================================================================//
// MOAIGlyphCache
//================================================================//
/**	@name	MOAIGlyphCache
	@text	<p>This is the default implementation of a dynamic glyph cache.
			Right now it can only grow but support for reference counting
			glyphs and garbage collection will be added later.</p>
			
			<p>The current implementation is set up in anticipation of
			garbage collection. If you use MOAIFont's getImage () to inspect
			the work of this cache you'll see that it is not as efficient
			in terms of texture use as it could be - glyphs are grouped by
			size, all glyphs for a given face size are given the same height
			and layout is orientated around rows. All of this will make it
			much easier to replace individual glyph slots as the set of glyphs
			that needs to be in memory changes. That said, we may offer an
			alternative dynamic cache implementation that attempts a more
			compact use of texture space, the tradeoff being that there
			won't be any garbage collection.</p>
			
			<p>This implementation of the dynamic glyph cache does not implement
			setImage ().</p>
			
			<p>Of course, you can also derive your own implementaion
			from MOAIGlyphCacheBase.</p>
*/
class MOAIGlyphCache :
	public MOAIGlyphCacheBase {
protected:

	MOAIFont* mFont;
	USLeanArray < MOAIGlyphCachePage* > mPages;

	//----------------------------------------------------------------//
	void				ClearPages					();
	
public:
	
	DECL_LUA_FACTORY ( MOAIGlyphCache )
	
	//----------------------------------------------------------------//
	MOAIImage*			GetGlyphImage				( MOAIGlyph& glyph );
	MOAITextureBase*	GetGlyphTexture				( MOAIGlyph& glyph );
	MOAIImage*			GetImage					();
	bool				IsDynamic					();
						MOAIGlyphCache				();
						~MOAIGlyphCache				();
	void				PlaceGlyph					( MOAIFont& font, MOAIGlyph& glyph );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				RemoveGlyph					( MOAIGlyph& glyph );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetImage					( MOAIFont& font, MOAIImage& image );
};

#endif
