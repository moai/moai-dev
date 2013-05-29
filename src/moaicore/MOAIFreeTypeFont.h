// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFREETYPEFONT_H
#define MOAIFREETYPEFONT_H


#include <moaicore/MOAILua.h>

class MOAIFontReader;

#define DPI 72
#define POINTS_TO_PIXELS(points,dpi) (( points * dpi ) / DPI )
#define PIXELS_TO_POINTS(pixels,dpi) (( pixels * DPI ) / dpi )

//================================================================//
// MOAIFreeTypeFont
//================================================================//
/**	@name	MOAIFreeTypeFont
	@text	A replacement of MOAIFont that does not use MOAIGlyph or MOAIGlyphSet.
			
 
 */

class MOAIFreeTypeFont :
	public MOAILuaObject {
protected:
	STLString mFilename;
	u32 mFlags;
		
	MOAILuaSharedPtr < MOAIFontReader > mReader;
		
	float mDefaultSize;
		
		
	//----------------------------------------------------------------//
	static int			_load					( lua_State* L );
	static int			_setReader				( lua_State* L );
	//----------------------------------------------------------------//
		
		
public:
		
		
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );

};


#endif /* defined(MOAIFREETYPEFONT_H) */
