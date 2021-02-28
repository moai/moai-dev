// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMATH_H
#define MOAIMATH_H

//================================================================//
// MOAIMath
//================================================================//
// TODO: doxygen
class MOAIMath :
	public virtual ZLContextClass,
	public virtual MOAILuaObject {
private:
	
	#ifdef MOAI_WITH_SFMT
	
		struct SFMT_T*	mSFMT;
		
		//----------------------------------------------------------------//
		static int		_randSFMT		( lua_State* L );
		static int		_seedSFMT		( lua_State* L );

	#endif

	//----------------------------------------------------------------//
	void			_RegisterLuaClass	( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_SINGLETON ( MOAIMath )

	//----------------------------------------------------------------//
					MOAIMath			( ZLContext& context );
					~MOAIMath			();
};

#endif
