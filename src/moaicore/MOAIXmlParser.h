// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIXMLPARSER_H
#define	MOAIXMLPARSER_H

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
	public USGlobalClass < MOAIXmlParser, USLuaObject > {
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
	static void		Parse					( USLuaState& state, TiXmlNode* node );
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
};

#endif
