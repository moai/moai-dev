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
	
	// optimal size processing parameters
	float			mMaxFontSize;
	float			mMinFontSize;
	bool			mForceSingleLine;
	float			mGranularity;
	bool			mRoundToInteger;
	
	bool			mFirstProcessRun;
	
	
	static int			_processOptimalSize		( lua_State* L );
	static int			_render					( lua_State* L );
	static int			_renderSingleLine		( lua_State* L );
	static int			_resetProcess			( lua_State* L );
	static int			_setAlignment			( lua_State* L );
	static int			_setDimensions			( lua_State* L );
	static int			_setFont				( lua_State* L );
	static int			_setFontSize			( lua_State* L );
	static int			_setForceSingleLine		( lua_State* L );
	static int			_setGranularity			( lua_State* L );
	static int			_setHeight				( lua_State* L );
	static int			_setMaxFontSize			( lua_State* L );
	static int			_setMinFontSize			( lua_State* L );
	static int			_setReturnGlyphBounds	( lua_State* L );
	static int			_setRoundToInteger		( lua_State* L );
	static int			_setWidth				( lua_State* L );
	static int			_setWordBreak			( lua_State* L );
	
	
public:
	DECL_LUA_FACTORY ( MOAITextRenderer )
	
	static const int PROCESSING_IN_PROGRESS = -1;
	
	MOAITextRenderer ();
	~MOAITextRenderer ();
	
	float				ProcessOptimalSize		(cc8 *text);
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif /* defined(MOAITEXTRENDERER_H) */
