// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFREETYPETEXTBOX_H
#define MOAIFREETYPETEXTBOX_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIProp.h>

#include <ft2build.h>
#include FT_FREETYPE_H

class MOAIFreeTypeFont;
class MOAITexture;

/** @name	MOAIFreeTypeTextBox
	@text	An alternative to MOAITextBox that uses an instance of MOAIFreeTypeFont directly.
 */

class MOAIFreeTypeTextBox :
	public MOAIProp {
private:
	//friend MOAIFreeTypeFont;
		
	float				mLineSpacing;
		
	USRect				mFrame;
		
	STLString			mText;
	u32					mTextLength;
		
	u32					mHAlign;
	u32					mVAlign;
		
	bool				mYFlip;
	float				mGlyphScale;
		
	// rule for breaking words across lines
	u32 mWordBreak;
		
	// automatically fit text in box
	bool				mAutoFit;
	
	MOAIFreeTypeFont*   mFont;
	float				mFontSize;
	//MOAILuaSharedPtr <MOAIFreeTypeFont> mFont;
		
	bool				mNeedsLayout;
	
	MOAITexture*		mTexture;
	
		
		//----------------------------------------------------------------//
	static int			_getAutoFit				( lua_State* L );
	static int			_getGlyphScale			( lua_State* L );
	static int			_getRect				( lua_State* L );
	static int			_setAutoFit				( lua_State* L );
	static int			_setFont				( lua_State* L );
	static int			_setGlyphScale			( lua_State* L );
	static int			_setRect				( lua_State* L );
		
	
	void				BuildLayout				();
	void				DrawBitmap				(FT_Bitmap* bitmap, FT_Int x, FT_Int y);
	void				Layout					();
	void				OnDepNodeUpdate			();
	void				ScheduleLayout			();
		
public:
		enum {
			LEFT_JUSTIFY,
			CENTER_JUSTIFY,
			RIGHT_JUSTIFY,
		};
		
		enum {
			WORD_BREAK_NONE,
			WORD_BREAK_CHAR,
		};
		
	void				Draw					( int subPrimID );	
						MOAIFreeTypeTextBox		();
						~MOAIFreeTypeTextBox	();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SetFont					( MOAIFreeTypeFont* font );
	void				SetRect					( float left, float top, float right, float bottom );
	
};

#endif /* defined(MOAIFREETYPETEXTBOX_H) */
