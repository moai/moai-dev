// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTITIONHOLDER_H
#define	MOAIPARTITIONHOLDER_H

class MOAIPartition;

//================================================================//
// MOAIPartitionHolder
//================================================================//
// TODO: doxygen
class MOAIPartitionHolder :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaSharedPtr < MOAIPartition >		mPartition;

	//----------------------------------------------------------------//
	static int			_clear						( lua_State* L );
	static int			_getPartition				( lua_State* L );
	static int			_setPartition				( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	//----------------------------------------------------------------//
	MOAIPartition*		GetPartition				();
						MOAIPartitionHolder			( ZLContext& context );
						~MOAIPartitionHolder		();
};

#endif
