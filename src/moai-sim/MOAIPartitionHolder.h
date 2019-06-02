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

	MOAILuaSharedPtr < MOAIPartition >		mPartition;

	//----------------------------------------------------------------//
	static int			_clear						( lua_State* L );
	static int			_getPartition				( lua_State* L );
	static int			_setPartition				( lua_State* L );

public:
	
	//----------------------------------------------------------------//
	MOAIPartition*		GetPartition				();
						MOAIPartitionHolder			();
						~MOAIPartitionHolder		();
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
};

#endif
