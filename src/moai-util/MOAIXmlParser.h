// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIXMLPARSER_H
#define	MOAIXMLPARSER_H
#if MOAI_WITH_TINYXML && MOAI_WITH_EXPAT

class MOAIStream;
class TiXmlNode;

//================================================================//
// MOAIXmlParser
//================================================================//
/**	@lua	MOAIXmlParser
	@text	Converts XML DOM to Lua trees. Provided as a convenience;
			not advised for parsing very large XML documents. (Use of
			XML not advised at all - use JSON or Lua.)
*/
class MOAIXmlParser :
	public virtual MOAILuaObject {
private:

	ZLXmlReader mReader;
	MOAILuaSharedPtr < MOAIStream >	mStream;

	//----------------------------------------------------------------//
	static int	_getElementAttribute		( lua_State* L );
	static int	_getElementAttributes		( lua_State* L );
	static int	_getElementLineNumber		( lua_State* L );
	static int	_getElementName				( lua_State* L );
	static int	_getElementText				( lua_State* L );
	static int	_parseFile					( lua_State* L );
	static int	_parseString				( lua_State* L );
	static int	_setStream					( lua_State* L );
	static int	_step						( lua_State* L );

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_FACTORY ( MOAIXmlParser )
	
	//----------------------------------------------------------------//
					MOAIXmlParser			();
					~MOAIXmlParser			();
	static void		Parse					( MOAILuaState& state, TiXmlNode* node );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif