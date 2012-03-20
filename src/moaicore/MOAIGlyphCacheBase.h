// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHCACHEBASE_H
#define	MOAIGLYPHCACHEBASE_H

#include <moaicore/MOAILua.h>

class MOAIGlyph;
class MOAIImage;
class MOAITextureBase;

//================================================================//
// MOAIGlyphCacheBase
//================================================================//
class MOAIGlyphCacheBase :
	public MOAILuaObject {
protected:

	friend class MOAIFont;
	
	USColor::Format mColorFormat;

	//----------------------------------------------------------------//
	static int					_getImage				( lua_State* L );
	static int					_setColorFormat			( lua_State* L );
	static int					_setImage				( lua_State* L );

public:
	
	//----------------------------------------------------------------//
	virtual MOAIImage*			GetGlyphImage			( MOAIGlyph& glyph ) = 0;
	virtual MOAITextureBase*	GetGlyphTexture			( MOAIGlyph& glyph ) = 0;
	virtual MOAIImage*			GetImage				() = 0;
	virtual bool				IsDynamic				() = 0;
								MOAIGlyphCacheBase		();
								~MOAIGlyphCacheBase		();
	virtual void				PlaceGlyph				( MOAIGlyph& glyph ) = 0;
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	virtual void				RemoveGlyph				( MOAIGlyph& glyph ) = 0;
	virtual void				SetImage				( MOAIImage& image ) = 0;
};

#endif
