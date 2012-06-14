// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUAREF_H
#define MOAILUAREF_H

class MOAILuaRuntime;
class MOAILuaState;
class MOAILuaStateHandle;

//================================================================//
// MOAILuaRefTable
//================================================================//
class MOAILuaRefTable {
private:

	friend class MOAILuaRef;

	int		mTableID;
	
	static const u32		MAX_REF_ID = 0xffffffff;

	static const u32		REFID_CHUNK_SIZE = 1024;
	USLeanArray < u32 >		mRefIDStack;
	u32						mRefIDStackTop;

	//----------------------------------------------------------------//
	void		ReleaseRefID		( int refID );
	int			ReserveRefID		();

public:

	//----------------------------------------------------------------//
	void		Clear				();
	void		InitStrong			();
	void		InitWeak			();
				MOAILuaRefTable		();
				~MOAILuaRefTable	();
	void		PushRef				( MOAILuaState& state, int refID );
	int			Ref					( MOAILuaState& state, int idx );
	void		Unref				( MOAILuaState& state, int refID );
};

//================================================================//
// MOAILuaRef
//================================================================//
class MOAILuaRef {
private:

	int			mRef;
	bool		mOwnsRef;
	bool		mWeak;

	//----------------------------------------------------------------//
	void					SetRef			( MOAILuaState& state, int idx, bool weak );

public:

	static const bool STRONG_REF	= false;
	static const bool WEAK_REF		= true;

	//----------------------------------------------------------------//
	void					Clear			();
	bool					IsNil			();
	bool					IsWeak			();
	u32						GetID			();
	MOAILuaStateHandle		GetSelf			();
	void					MakeStrong		();
	void					MakeWeak		();
							MOAILuaRef		();
							MOAILuaRef		( const MOAILuaRef& assign );
							~MOAILuaRef		();
	bool					PushRef			( MOAILuaState& state );
	void					SetStrongRef	( MOAILuaState& state, int idx );
	void					SetWeakRef		( MOAILuaState& state, int idx );
	void					Take			( const MOAILuaRef& assign );
	void					WeakCopy		( const MOAILuaRef& assign );

	//----------------------------------------------------------------//
	inline void operator = ( const MOAILuaRef& assign ) {
		this->Take ( assign );
	}

	//----------------------------------------------------------------//
	inline bool operator < ( const MOAILuaRef& compare ) const {
	
		if ( this->mWeak != compare.mWeak ) {
			return this->mWeak;
		}
		return ( this->mRef < compare.mRef );
	}

	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mRef != LUA_NOREF );
	}
};

#endif
