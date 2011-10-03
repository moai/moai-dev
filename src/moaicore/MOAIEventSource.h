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

	USLuaPrivateRef		mListenerTable;

	//----------------------------------------------------------------//
	static int			_setListener			( lua_State* L );

	//----------------------------------------------------------------//
	void				AffirmListenerTable		( USLuaState& state );

protected:

	//----------------------------------------------------------------//
	template < typename TYPE >
	static int _setListener ( lua_State* L ) {
	
		u32 idx = 1;
		
		USLuaState state ( L );
		if ( !state.IsType ( idx, LUA_TNUMBER )) {
			idx = 2;
		}
		
		if ( state.IsType ( idx, LUA_TNUMBER )) {
		
			TYPE& global = TYPE::Get ();
			global.SetListener ( L, idx );
		}
		return 0;
	}

	//----------------------------------------------------------------//
	bool				PushListener			( u32 eventID, USLuaState& state );
	bool				PushListenerAndSelf		( u32 eventID, USLuaState& state );
	void				SetListener				( lua_State* L, u32 idx );

public:

	//----------------------------------------------------------------//
	
						MOAIEventSource			();
	virtual				~MOAIEventSource		();
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
};

#endif
