// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONT_H
#define	MOAIFONT_H

#include <moaicore/MOAIGlyphDeck.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAISpanList.h>

class MOAIGlyph;

//================================================================//
// MOAIFont
//================================================================//
class MOAIFont :
	public MOAILuaObject {
private:

	MOAIGlyphPage* mPages;

	STLString	mFilename;
	
	// for now
	typedef STLMap < float, MOAIGlyphDeck >::iterator GlyphDecksIt;
	STLMap < float, MOAIGlyphDeck > mGlyphDecks;

	//----------------------------------------------------------------//
	static int			_load					( lua_State* L );
	static int			_preloadGlyphs			( lua_State* L );

	//----------------------------------------------------------------//
	MOAISpan < MOAIGlyph* >*	Alloc				( MOAIGlyph& glyph );
	void						UpdateGlyphs		( u32 target );

public:
	
	DECL_LUA_FACTORY ( MOAIFont )
	
	//----------------------------------------------------------------//
	void				AffirmGlyph				( float points, u32 c );
	MOAIGlyphDeck*		GetGlyphDeck			( float points );
	void				Init					( cc8* filename );
	void				LoadGlyphMetrics		();
						MOAIFont				();
						~MOAIFont				();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void				WritePages				();
};

#endif
