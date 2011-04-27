// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUAREF_H
#define LUAREF_H

class USLuaRuntime;
class USLuaState;
class USLuaStateHandle;

//================================================================//
// USLuaRef
//================================================================//
class USLuaRef {
protected:

	int			mRef;
	bool		mOwnsRef;
	bool		mWeak;

public:

	static const bool STRONG_REF	= false;
	static const bool WEAK_REF		= true;

	//----------------------------------------------------------------//
	void					Clear			();
	bool					IsNil			();
	bool					IsWeak			();
	u32						GetID			();
	USLuaStateHandle		GetSelf			();
	void					MakeStrong		();
	void					MakeWeak		();
	bool					PushRef			( USLuaState& state );
	void					SetRef			( USLuaState& state, int idx, bool weak );
	void					Take			( const USLuaRef& assign );
							USLuaRef		();
							USLuaRef		( const USLuaRef& assign );
							~USLuaRef		();
	void					WeakCopy		( const USLuaRef& assign );

	//----------------------------------------------------------------//
	inline void operator = ( const USLuaRef& assign ) {
		this->Take ( assign );
	}

	//----------------------------------------------------------------//
	inline bool operator < ( const USLuaRef& compare ) const {
	
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
