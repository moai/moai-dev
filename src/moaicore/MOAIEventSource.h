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
protected:

	//----------------------------------------------------------------//
	virtual void	AffirmListenerTable		( USLuaState& state ) = 0;
	bool			PushListener			( u32 eventID, USLuaState& state );
	bool			PushListenerAndSelf		( u32 eventID, USLuaState& state );
	virtual bool	PushListenerTable		( USLuaState& state ) = 0;
	void			SetListener				( lua_State* L, u32 idx );

public:

	//----------------------------------------------------------------//
	
					MOAIEventSource			();
	virtual			~MOAIEventSource		();
};

//================================================================//
// MOAIInstanceEventSource
//================================================================//
/**	@name	MOAIInstanceEventSource
	@text	Derivation of MOAIEventSource for non-global lua objects.
*/
class MOAIInstanceEventSource :
	public virtual MOAIEventSource {
private:

	USLuaLocal		mListenerTable;

	//----------------------------------------------------------------//
	static int		_setListener				( lua_State* L );

protected:

	//----------------------------------------------------------------//
	void			AffirmListenerTable			( USLuaState& state );
	bool			PushListenerTable			( USLuaState& state );

public:

	//----------------------------------------------------------------//
					MOAIInstanceEventSource		();
	virtual			~MOAIInstanceEventSource	();
	void			RegisterLuaFuncs			( USLuaState& state );
};

//================================================================//
// MOAIGlobalEventSource
//================================================================//
/**	@name	MOAIGlobalEventSource
	@text	Derivation of MOAIEventSource for global lua objects.
*/
class MOAIGlobalEventSource :
	public virtual MOAIEventSource {
private:

	USLuaRef		mListenerTable;

	//----------------------------------------------------------------//
	static int		_setListener				( lua_State* L );

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
	void			AffirmListenerTable			( USLuaState& state );
	bool			PushListenerTable			( USLuaState& state );

public:

	//----------------------------------------------------------------//
					MOAIGlobalEventSource		();
	virtual			~MOAIGlobalEventSource		();
};

#endif
