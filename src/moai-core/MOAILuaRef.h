// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUAREF_H
#define MOAILUAREF_H

class MOAILuaObject;
class MOAILuaRuntime;
class MOAILuaState;
class MOAIScopedLuaState;

//================================================================//
// MOAILuaRef
//================================================================//
class MOAILuaRef {
protected:

	int					mRef;
	bool				mOwnsRef;
	int					mRefTableID;

	//----------------------------------------------------------------//
	void					SetRef				( MOAILuaState& state, int idx, int refTableID );

public:

	//----------------------------------------------------------------//
	void					Clear				();
	u32						GetID				();
	MOAIScopedLuaState		GetSelf				();
							MOAILuaRef			();
							MOAILuaRef			( const MOAILuaRef& assign );
	virtual					~MOAILuaRef			();
	bool					PushRef				( MOAILuaState& state );
	virtual void			SetRef				( MOAILuaState& state, int idx ) = 0;
	void					Take				( const MOAILuaRef& assign );

	//----------------------------------------------------------------//
	inline void operator = ( const MOAILuaRef& assign ) {
		this->Take ( assign );
	}

	//----------------------------------------------------------------//
	inline bool operator < ( const MOAILuaRef& compare ) const {
		return ( this->mRef < compare.mRef );
	}
	
	//----------------------------------------------------------------//
	inline operator bool () {
		return this->mRef != LUA_NOREF;
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