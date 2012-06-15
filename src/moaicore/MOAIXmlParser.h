// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIXMLPARSER_H
#define	MOAIXMLPARSER_H

#include <moaicore/MOAILua.h>

class TiXmlNode;

//================================================================//
// MOAIXmlParser
//================================================================//
/**	@name	MOAIXmlParser
	@text	Converts XML DOM to Lua trees. Provided as a convenience;
			not advised for parsing very large XML documents. (Use of
			XML not advised at all - use JSON or Lua.)
*/
class MOAIXmlParser :
	public MOAIGlobalClass < MOAIXmlParser, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_parseFile					( lua_State* L );
	static int	_parseString				( lua_State* L );

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_SINGLETON ( MOAIXmlParser )
	
	//----------------------------------------------------------------//
					MOAIXmlParser			();
					~MOAIXmlParser			();
	static void		Parse					( MOAILuaState& state, TiXmlNode* node );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
