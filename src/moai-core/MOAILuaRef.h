// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUAREF_H
#define MOAILUAREF_H

#include <moai-core/MOAILuaRefTable.h>

class MOAILuaObject;
class MOAILuaRuntime;
class MOAILuaState;
class MOAIScopedLuaState;

//================================================================//
// MOAILuaRef
//================================================================//
class MOAILuaRef {
private:

	MOAILuaRefTable*		mRefTable;
	bool					mOwnsRef;
	int						mRef;

	//----------------------------------------------------------------//
	void					SetRef			( MOAILuaState& state, int idx, bool weak );

public:

	friend class MOAILuaStrongRef;
	friend class MOAILuaWeakRef;

	//----------------------------------------------------------------//
	void					Clear			();
	bool					IsNil			();
	bool					IsWeak			();
	u32						GetID			();
	MOAIScopedLuaState		GetSelf			();
	void					MakeStrong		();
	void					MakeWeak		();
							MOAILuaRef		();
							MOAILuaRef		( const MOAILuaRef& assign );
	virtual					~MOAILuaRef		();
	bool					PushRef			( MOAILuaState& state );
	virtual void			SetRef			( MOAILuaState& state, int idx ) = 0; // TODO
	void					SetStrongRef	( MOAILuaState& state, int idx );
	void					SetWeakRef		( MOAILuaState& state, int idx );
	void					Take			( const MOAILuaRef& assign );

	//----------------------------------------------------------------//
	inline void operator = ( const MOAILuaRef& assign ) {
		this->Take ( assign );
	}

	//----------------------------------------------------------------//
	inline bool operator < ( const MOAILuaRef& compare ) const {
	
		if ( this->mRefTable == compare.mRefTable ) {
			return ( this->mRef < compare.mRef );
		}
		return this->mRefTable < compare.mRefTable;
	}

	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mRef != LUA_NOREF );
	}
};

//================================================================//
// MOAILuaStrongRef
//================================================================//
class MOAILuaStrongRef :
	public MOAILuaRef {
public:

	//----------------------------------------------------------------//
	void			SetRef			( MOAILuaState& state, int idx );
};

//================================================================//
// MOAILuaWeakRef
//================================================================//
class MOAILuaWeakRef :
	public MOAILuaRef {
public:

	//----------------------------------------------------------------//
	void			SetRef			( MOAILuaState& state, int idx );
};

//================================================================//
// MOAILuaMemberRef
//================================================================//
class MOAILuaMemberRef {
private:

	int					mRef;
	MOAILuaObject*		mOwner;

public:
	
	//----------------------------------------------------------------//
	void		Clear					();
				MOAILuaMemberRef		();
				~MOAILuaMemberRef		();
	bool		PushRef					( MOAILuaState& state );
	void		SetRef					( MOAILuaObject& owner, MOAILuaState& state, int idx );
	
	//----------------------------------------------------------------//
	inline operator bool () {
		return this->mRef != LUA_NOREF;
	}
};

#endif