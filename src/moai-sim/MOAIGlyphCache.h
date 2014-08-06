// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHCACHE_H
#define	MOAIGLYPHCACHE_H

class MOAIGlyph;
class MOAIImage;
class MOAITextureBase;
class MOAIFont;

//================================================================//
// MOAIGlyphCache
//================================================================//
/**	@lua	MOAIGlyphCache
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
class MOAIGlyphCache :
	virtual public MOAILuaObject {
protected:

	friend class MOAIFont;

public:
	
	enum {
		STATUS_OK,
		STATUS_ERROR,
		STATUS_UNSUPPORTED,
	};
	
	//----------------------------------------------------------------//
	virtual MOAIImage*			GetGlyphImage			( MOAIGlyph& glyph ) = 0;
	virtual MOAITextureBase*	GetGlyphTexture			( MOAIGlyph& glyph ) = 0;
	virtual MOAIImage*			GetImage				() = 0;
	virtual bool				IsDynamic				() = 0;
								MOAIGlyphCache			();
								~MOAIGlyphCache			();
	virtual int					PlaceGlyph				( MOAIFont& font, MOAIGlyph& glyph );
	virtual void				PostRender				( MOAIGlyph& glyph );
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	virtual int					RemoveGlyph				( MOAIGlyph& glyph );
	virtual int					SetImage				( MOAIFont& font, MOAIImage& image );
};

#endif
