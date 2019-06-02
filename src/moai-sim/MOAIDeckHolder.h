// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKHOLDER_H
#define	MOAIDECKHOLDER_H

#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAITransform.h>
#include <moai-core/MOAILuaSharedPtr.h>

class MOAIDeck;

//================================================================//
// MOAIDeckHolder
//================================================================//
// TODO: doxygen
class MOAIDeckHolder :
	public virtual MOAINode {
protected:
	
	MOAILuaSharedPtr < MOAIDeck > mDeck;
	
	//----------------------------------------------------------------//
	static int			_getDeck				( lua_State* L );
	static int			_setDeck				( lua_State* L );


	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	GET ( MOAIDeck*, Deck, mDeck )

	//----------------------------------------------------------------//
						MOAIDeckHolder			();
						~MOAIDeckHolder			();
};

#endif
