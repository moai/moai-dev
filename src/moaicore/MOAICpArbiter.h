// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICPARBITER_H
#define	MOAICPARBITER_H
#if USE_CHIPMUNK

#include <chipmunk/chipmunk.h>

class MOAICpSpace;

//================================================================//
// MOAICpArbiter
//================================================================//
/**	@name	MOAICpArbiter
	@text	Chipmunk Arbiter.
*/
class MOAICpArbiter :
	public virtual USLuaObject {
private:

	cpArbiter*	mArbiter;

	//----------------------------------------------------------------//
	static int		_countContacts					( lua_State* L );
	static int		_getContactDepth				( lua_State* L );
	static int		_getContactNormal				( lua_State* L );
	static int		_getContactPoint				( lua_State* L );
	static int		_getTotalImpulse				( lua_State* L );
	static int		_getTotalImpulseWithFriction	( lua_State* L );
	static int		_isFirstContact					( lua_State* L );
	
	//----------------------------------------------------------------//

public:
	
	DECL_LUA_FACTORY ( MOAICpArbiter )
	
	GET_SET ( cpArbiter*, Arbiter, mArbiter )
	
	//----------------------------------------------------------------//
					MOAICpArbiter			();
					~MOAICpArbiter			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
#endif