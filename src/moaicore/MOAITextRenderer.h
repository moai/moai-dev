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
	
	static int			_renderTexture			( lua_State* L );
	static int			_renderTextureSingleLine( lua_State* L );
	static int			_setFont				( lua_State* L );
	
public:
	DECL_LUA_FACTORY ( MOAIFreeTypeFont )
};

#endif /* defined(MOAITEXTRENDERER_H) */
