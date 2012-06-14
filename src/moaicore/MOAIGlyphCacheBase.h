// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHCACHEBASE_H
#define	MOAIGLYPHCACHEBASE_H

#include <moaicore/MOAILua.h>

class MOAIGlyph;
class MOAIImage;
class MOAITextureBase;
class MOAIFont;

//================================================================//
// MOAIGlyphCacheBase
//================================================================//
/**	@name	MOAIGlyphCacheBase
	@text	<p>Base class for implementations of glyph caches. A glyph
			cache is responsible for allocating textures to hold
			rendered glyphs and for placing individuals glyphs on
			those textures.</p>
			
			<p>Even though the glyph cache is responsible for placing
			glyphs on textures, the glyph cache does not have
			to keep track of glyph metrics. Glyph metrics are stored
			independently by the font. This means that glyph caches
			with equivalent textures may be swapped out for use with
			the same font.</p>
*/
class MOAIGlyphCacheBase :
	public MOAILuaObject {
protected:

	friend class MOAIFont;
	
	USColor::Format mColorFormat;

	//----------------------------------------------------------------//
	static int					_setColorFormat			( lua_State* L );

public:
	
	//----------------------------------------------------------------//
	virtual MOAIImage*			GetGlyphImage			( MOAIGlyph& glyph ) = 0;
	virtual MOAITextureBase*	GetGlyphTexture			( MOAIGlyph& glyph ) = 0;
	virtual MOAIImage*			GetImage				() = 0;
	virtual bool				IsDynamic				() = 0;
								MOAIGlyphCacheBase		();
								~MOAIGlyphCacheBase		();
	virtual void				PlaceGlyph				( MOAIFont& font, MOAIGlyph& glyph ) = 0;
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	virtual void				RemoveGlyph				( MOAIGlyph& glyph ) = 0;
	virtual void				SetImage				( MOAIFont& font, MOAIImage& image ) = 0;
};

#endif
