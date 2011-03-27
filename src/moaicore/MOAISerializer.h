// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISERIALIZER_H
#define	MOAISERIALIZER_H

//================================================================//
// MOAISerializer
//================================================================//
/**	@brief Manages serialization state.
*/
class MOAISerializer :
	public USLuaSerializer {
private:

	//----------------------------------------------------------------//
	static int			_serializeToString		( lua_State* L );

	//----------------------------------------------------------------//
	cc8*				GetFileMagic			();

public:
	
	DECL_LUA_FACTORY ( MOAISerializer )
	
	//----------------------------------------------------------------//
						MOAISerializer			();
						~MOAISerializer			();
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
	STLString			ToString				();
};

#endif
