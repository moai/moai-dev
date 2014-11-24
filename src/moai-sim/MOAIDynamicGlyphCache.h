// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDYNAMICGLYPHCACHE_H
#define	MOAIDYNAMICGLYPHCACHE_H

#include <moai-sim/MOAIGlyphCache.h>

class MOAIFont;
class MOAIDynamicGlyphCachePage;

//================================================================//
// MOAIDynamicGlyphCache
//================================================================//
/**	@lua	MOAIDynamicGlyphCache
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
			
			<p>Of course, you can also derive your own implementation
			from MOAIGlyphCache.</p>
*/
class MOAIDynamicGlyphCache :
	public MOAIGlyphCache {
protected:

	friend class MOAIDynamicGlyphCachePage;

	MOAIFont* mFont;
	ZLLeanArray < MOAIDynamicGlyphCachePage* > mPages;

	ZLColor::ColorFormat mColorFormat;
	
	ZLRect mPadding;
	
	//----------------------------------------------------------------//
	static int			_setColorFormat				( lua_State* L );
	static int			_setPadding					( lua_State* L );

	//----------------------------------------------------------------//
	void				ClearPages					();
	
public:
	
	DECL_LUA_FACTORY ( MOAIDynamicGlyphCache )
	
	//----------------------------------------------------------------//
	MOAIImage*			GetGlyphImage				( MOAIGlyph& glyph );
	MOAITextureBase*	GetGlyphTexture				( MOAIGlyph& glyph );
	MOAIImage*			GetImage					();
	bool				IsDynamic					();
						MOAIDynamicGlyphCache		();
						~MOAIDynamicGlyphCache		();
	int					PlaceGlyph					( MOAIFont& font, MOAIGlyph& glyph );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
