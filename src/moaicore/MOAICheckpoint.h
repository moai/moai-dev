// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICHECKPOINT_H
#define	MOAICHECKPOINT_H

//================================================================//
// MOAISyncKey
//================================================================//
class MOAISyncKey {
private:

	enum {
		IDLE,
		BUSY,
		RESET,
	};

	u32		mCount;
	u32		mState;
	u32		mNext;

	friend class MOAICheckpoint;
};

//================================================================//
// MOAICheckpoint
//================================================================//
/**	@brief Sychronization class for threads.
*/
class MOAICheckpoint :
	public virtual USLuaObject {
private:

	static const u32 BLOCK_SIZE		= 4;
	static const u32 NULL_ID		= 0xffffffff;

	USLeanArray < MOAISyncKey > mKeys;

	u32		mActiveKeys; // ID of first active state
	u32		mFreeKeys; // ID of first free state

	bool	mBusy;

	//----------------------------------------------------------------//
	static int	_newKey			( lua_State* L );
	static int	_releaseKey		( lua_State* L );
	static int	_skip			( lua_State* L );
	static int	_sync			( lua_State* L );
	
	//----------------------------------------------------------------//
	u32				AddKey					();
	void			AdvanceKeys				();
	bool			CheckKeysPending		();
	void			ReleaseKey				( u32 keyID );
	void			Skip					( u32 keyID );
	bool			Sync					( u32 keyID );
	
public:
	
	DECL_LUA_FACTORY ( MOAICheckpoint )
	
	//----------------------------------------------------------------//						
					MOAICheckpoint			();
					~MOAICheckpoint			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	STLString		ToString				();
};

#endif
