// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFREETYPETEXTBOX_H
#define MOAIFREETYPETEXTBOX_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIProp.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

class MOAIFreeTypeFont;
class MOAITexture;

using namespace std;

struct MOAIFreeTypeTextLine {
	int lineWidth;
	wchar_t* text;
};

struct MOAIFreeTypeImageBuffer {
	u32 width;
	u32 height;
	u8 *data;
};

/** @name	MOAIFreeTypeTextBox
	@text	An alternative to MOAITextBox that uses an instance of MOAIFreeTypeFont directly.
 */

class MOAIFreeTypeTextBox : public MOAIGlobalClass < MOAIFreeTypeTextBox, MOAILuaObject > {
private:
	
	//bool								mAutoFit;

//----------------------------------------------------------------//
	static int					_generateLabelTexture			( lua_State* L );
	//static int					_setAutoFit						( lua_State* L );
	
//----------------------------------------------------------------//

	static int							ComputeLineStart		(FT_Face face, FT_UInt unicode, int lineIndex, int alignment, FT_Int imgWidth, const vector<MOAIFreeTypeTextLine> &lines);
	static int							ComputeLineStartY		(FT_Face face, int textHeight, FT_Int imgHeight, int vAlign);

	static MOAIFreeTypeImageBuffer		InitBitmapData			(u32 width, u32 height);
	static vector<MOAIFreeTypeTextLine> GenerateLines			(FT_Face face, FT_Int maxWidth, cc8* text, int wordBreak);
	static MOAIFreeTypeTextLine			BuildLine				(wchar_t* buffer, size_t buf_len, FT_Face face, int pen_x, u32 lastChar);
	static void							RenderLines				(const vector<MOAIFreeTypeTextLine> &lines, u8 *renderBitmap, FT_Int imgWidth, FT_Int imgHeight, int bitmapWidth, int bitmapHeight, FT_Face face, int hAlign, int vAlign);
	static void							DrawBitmap				(FT_Bitmap *bitmap, FT_Int x, FT_Int y, u8 *renderBitmap, FT_Int imgWidth, FT_Int imgHeight, int bitmapWidth, int bitmapHeight);
	
public:

	DECL_LUA_SINGLETON( MOAIFreeTypeTextBox )
	
	static MOAITexture*					GenerateTexture			( cc8* text, MOAIFreeTypeFont *font, float size, float width, float height, int alignment, int wordbreak, int vAlignment, bool autoFit );
										MOAIFreeTypeTextBox		();
										~MOAIFreeTypeTextBox	();
	void								RegisterLuaClass		( MOAILuaState& state );
	//void								RegisterLuaFuncs		( MOAILuaState& state );
};

#endif /* defined(MOAIFREETYPETEXTBOX_H) */
