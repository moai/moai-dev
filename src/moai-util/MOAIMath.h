// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMATH_H
#define MOAIMATH_H

//================================================================//
// MOAIMath
//================================================================//
// TODO: doxygen
class MOAIMath :
	public MOAIGlobalClass < MOAIMath, MOAILuaObject > {
private:
	
	#ifdef MOAI_WITH_SFMT
	
		struct SFMT_T*	mSFMT;
		
		//----------------------------------------------------------------//
		static int		_randSFMT			( lua_State* L );
		static int		_seedSFMT			( lua_State* L );

	#endif

public:
	
	DECL_LUA_SINGLETON ( MOAIMath )

	//----------------------------------------------------------------//
					MOAIMath			();
					~MOAIMath			();
	void			RegisterLuaClass	( MOAILuaState& state );
};

#endif
