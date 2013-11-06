//
//  MOAITextRenderer.h
//  libmoai
//
//  Created by Isaac Barrett on 11/5/13.
//
//

#ifndef MOAITEXTRENDERER_H
#define MOAITEXTRENDERER_H

#include <moaicore/MOAILua.h>

class MOAIFreeTypeFont;

class MOAITextRenderer : public MOAILuaObject {
private:
	MOAILuaSharedPtr <MOAIFreeTypeFont>	mFont;
	
	float			mFontSize;
	
	float			mWidth;
	float			mHeight;
	
	int				mHorizontalAlignment;
	int				mVerticalAlignment;
	int				mWordBreak;
	
	bool			mReturnGlyphBounds;
	
	
	static int			_render					( lua_State* L );
	static int			_renderSingleLine		( lua_State* L );
	static int			_setAlignment			( lua_State* L );
	static int			_setDimensions			( lua_State* L );
	static int			_setFont				( lua_State* L );
	static int			_setFontSize			( lua_State* L );
	static int			_setHeight				( lua_State* L );
	static int			_setReturnGlyphBounds	( lua_State* L );
	static int			_setWidth				( lua_State* L );
	static int			_setWordBreak			( lua_State* L );
	
	
public:
	DECL_LUA_FACTORY ( MOAIFreeTypeFont )
	
	
	MOAITextRenderer ();
	~MOAITextRenderer ();
	
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif /* defined(MOAITEXTRENDERER_H) */
