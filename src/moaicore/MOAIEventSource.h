// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEVENTSOURCE_H
#define	MOAIEVENTSOURCE_H

//================================================================//
// MOAIEventSource
//================================================================//
/**	@name	MOAIEventSource
	@text	Base class for all Lua-bound Moai objects that emit events
			and have an event table.
*/
class MOAIEventSource :
	public virtual USLuaObject {
private:

	USLuaRef			mListenerTable;

	//----------------------------------------------------------------//
	static int			_setListener			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIEventSource )

	//----------------------------------------------------------------//
	void				AffirmListenerTable		( USLuaState& state );
						MOAIEventSource			();
	virtual				~MOAIEventSource		();
	bool				PushListenerAndSelf		( u32 eventID, USLuaState& state );
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
};

#endif
