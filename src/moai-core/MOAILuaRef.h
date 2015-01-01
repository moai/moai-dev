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

	bool					mOwnsRef;
	int						mRefID;

public:

	enum {
		MAKE_STRONG,
		MAKE_WEAK,
	};

	friend class MOAILuaStrongRef;
	friend class MOAILuaWeakRef;

	//----------------------------------------------------------------//
	void					Clear			();
	bool					IsNil			();
	u32						GetID			();
	MOAIScopedLuaState		GetSelf			();
	void					MakeStrong		();
	void					MakeWeak		();
							MOAILuaRef		();
							MOAILuaRef		( const MOAILuaRef& assign );
	virtual					~MOAILuaRef		();
	bool					PushRef			( MOAILuaState& state );
	virtual void			SetRef			( MOAILuaObject* object );
	void					SetRef			( MOAILuaObject* object, u32 type );
	virtual void			SetRef			( MOAILuaState& state, int idx );
	void					SetRef			( MOAILuaState& state, int idx, u32 type );
	void					Take			( const MOAILuaRef& assign );

	//----------------------------------------------------------------//
	inline void operator = ( const MOAILuaRef& assign ) {
		this->Take ( assign );
	}

	//----------------------------------------------------------------//
	inline bool operator < ( const MOAILuaRef& compare ) const {
		return ( this->mRefID < compare.mRefID );
	}

	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mRefID != LUA_NOREF );
	}
};

//================================================================//
// MOAILuaStrongRef
//================================================================//
class MOAILuaStrongRef :
	public MOAILuaRef {
public:

	//----------------------------------------------------------------//
					MOAILuaStrongRef	();
					MOAILuaStrongRef	( MOAILuaObject* object );
	void			SetRef				( MOAILuaObject* object );
	void			SetRef				( MOAILuaState& state, int idx );
};

//================================================================//
// MOAILuaWeakRef
//================================================================//
class MOAILuaWeakRef :
	public MOAILuaRef {
public:

	//----------------------------------------------------------------//
					MOAILuaWeakRef		();
					MOAILuaWeakRef		( MOAILuaObject* object );
	void			SetRef				( MOAILuaObject* object );
	void			SetRef				( MOAILuaState& state, int idx );
};

//================================================================//
// MOAILuaMemberRef
//================================================================//
class MOAILuaMemberRef {
private:

	int					mRefID;
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
		return this->mRefID != LUA_NOREF;
	}
};

#endif