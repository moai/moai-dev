// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKHOLDER_H
#define	MOAIDECKHOLDER_H

#include <moai-sim/MOAIAbstractDrawable.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAITransform.h>
#include <moai-core/MOAILuaSharedPtr.h>

class MOAIDeck;

//================================================================//
// MOAIDeckHolder
//================================================================//
/**	@lua	MOAIDeckHolder
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	@attr	ATTR_PARTITION
*/
class MOAIDeckHolder :
	public virtual MOAINode {
private:
	
	//----------------------------------------------------------------//
	static int			_getDeck				( lua_State* L );
	static int			_setDeck				( lua_State* L );

protected:

	MOAILuaSharedPtr < MOAIDeck > mDeck;

public:
	
	GET ( MOAIDeck*, Deck, mDeck )

	//----------------------------------------------------------------//
						MOAIDeckHolder			();
						~MOAIDeckHolder			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
