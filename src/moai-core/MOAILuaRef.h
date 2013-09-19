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
protected:

	MOAILuaRefTable*		mRefTable;
	int						mRef;

	//----------------------------------------------------------------//
	void					SetRef				( MOAILuaState& state, int idx, MOAILuaRefTable& refTable );
							
	//----------------------------------------------------------------//
	inline void operator = ( const MOAILuaRef& assign ) {
		UNUSED ( assign );
	}
	
	//----------------------------------------------------------------//
	MOAILuaRef ( const MOAILuaRef& assign ) {
		UNUSED ( assign );
	}
							
public:

	//----------------------------------------------------------------//
	void					Clear				();
	u32						GetID				();
	MOAIScopedLuaState		GetSelf				();
							MOAILuaRef			();
							~MOAILuaRef			();
	bool					PushRef				( MOAILuaState& state );
	virtual void			SetRef				( MOAILuaState& state, int idx ) = 0;

	//----------------------------------------------------------------//
	inline bool operator < ( const MOAILuaRef& compare ) const {
		return ( this->mRef < compare.mRef );
	}
	
	//----------------------------------------------------------------//
	inline operator bool () {
		return this->mRef != 0;
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