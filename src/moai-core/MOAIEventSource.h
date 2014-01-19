// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEVENTSOURCE_H
#define	MOAIEVENTSOURCE_H

#include <moai-core/MOAILua.h>

//================================================================//
// MOAIEventSource
//================================================================//
/**	@name	MOAIEventSource
	@text	Base class for all Lua-bound Moai objects that emit events
			and have an event table.
*/
class MOAIEventSource :
	public virtual MOAILuaObject {
protected:

	//----------------------------------------------------------------//
	virtual void	AffirmListenerTable		( MOAILuaState& state ) = 0;
	virtual bool	PushListenerTable		( MOAILuaState& state ) = 0;
	void			SetListener				( lua_State* L, u32 idx );

public:

	//----------------------------------------------------------------//
					MOAIEventSource			();
	virtual			~MOAIEventSource		();
	bool			PushListener			( u32 eventID, MOAILuaState& state );
	bool			PushListenerAndSelf		( u32 eventID, MOAILuaState& state );
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

	MOAILuaMemberRef	mListenerTable;

	//----------------------------------------------------------------//
	static int		_getListener				( lua_State* L );
	static int		_setListener				( lua_State* L );

protected:

	//----------------------------------------------------------------//
	void			AffirmListenerTable			( MOAILuaState& state );
	bool			PushListenerTable			( MOAILuaState& state );

public:

	//----------------------------------------------------------------//
					MOAIInstanceEventSource		();
	virtual			~MOAIInstanceEventSource	();
	void			RegisterLuaFuncs			( MOAILuaState& state );
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

	MOAILuaStrongRef	mListenerTable;

	//----------------------------------------------------------------//
	static int		_getListener				( lua_State* L );
	static int		_setListener				( lua_State* L );

protected:

	//----------------------------------------------------------------//
	/**	@name	getListener
		@text	Gets the listener callback for a given event ID.

		@in		number eventID				The ID of the event.
		@out	function					The listener callback.
	*/
	template < typename TYPE >
	static int _getListener ( lua_State* L ) {

		u32 idx = 1;

		MOAILuaState state ( L );
		if ( !state.IsType ( idx, LUA_TNUMBER )) {
			idx = 2;
		}

		if ( state.IsType ( idx, LUA_TNUMBER )) {

			u32 eventID = state.GetValue < u32 >( idx, 0 );
			TYPE& global = TYPE::Get ();
			if ( global.PushListener( eventID, state )) {
				return 1;
			}
		}

		state.Push ();
		return 1;
	}

	//----------------------------------------------------------------//
	/**	@name	setListener
		@text	Sets a listener callback for a given event ID. It is up
				to individual classes to declare their event IDs.
			
		@in		number eventID				The ID of the event.
		@opt	function callback			The callback to be called when the object emits the event. Default value is nil.
		@out	nil
	*/
	template < typename TYPE >
	static int _setListener ( lua_State* L ) {
	
		u32 idx = 1;
		
		MOAILuaState state ( L );
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
	void			AffirmListenerTable			( MOAILuaState& state );
	bool			PushListenerTable			( MOAILuaState& state );

public:

	//----------------------------------------------------------------//
					MOAIGlobalEventSource		();
	virtual			~MOAIGlobalEventSource		();
};

#endif
