// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROJECTIONPROP_H
#define	MOAIPROJECTIONPROP_H

#include <moai-sim/MOAIAbstractDrawable.h>
#include <moai-sim/MOAIPartitionHull.h>

class MOAIGraphicsPropBase;

//================================================================//
// MOAIProjectionProp
//================================================================//
// TODO: doxygen
class MOAIProjectionProp :
	public virtual MOAIAbstractDrawable,
	public virtual MOAIPartitionHull {
protected:

	MOAILuaSharedPtr < MOAIGraphicsPropBase >	mSourceProp;
	MOAILuaSharedPtr < MOAIViewLayer > 			mSourceLayer;
	MOAILuaSharedPtr < MOAIViewLayer > 			mDestLayer;

	//----------------------------------------------------------------//
	static int				_init						( lua_State* L );

	//----------------------------------------------------------------//
	void					MOAIAbstractDrawable_Draw				( int subPrimID );
	void					MOAIAbstractDrawable_DrawDebug			( int subPrimID );
	void					MOAINode_Update							();
	u32						MOAIPartitionHull_AffirmInterfaceMask	( MOAIPartition& partition );
	ZLBounds				MOAIPartitionHull_GetModelBounds		();

public:

	DECL_LUA_FACTORY ( MOAIProjectionProp )

	//----------------------------------------------------------------//
							MOAIProjectionProp			();
	virtual					~MOAIProjectionProp			();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
