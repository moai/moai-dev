// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROJECTIONPROP_H
#define	MOAIPROJECTIONPROP_H

#include <moai-sim/MOAIAbstractProp.h>
#include <moai-sim/MOAIAbstractDrawable.h>

class MOAIGraphicsPropBase;

//================================================================//
// MOAIProjectionProp
//================================================================//
// TODO: doxygen
class MOAIProjectionProp :
	public virtual MOAIAbstractDrawable,
	public virtual MOAIAbstractProp {
protected:

	MOAILuaSharedPtr < MOAIGraphicsPropBase >	mSourceProp;
	MOAILuaSharedPtr < MOAIAbstractViewLayer > 			mSourceLayer;
	MOAILuaSharedPtr < MOAIAbstractViewLayer > 			mDestLayer;

	//----------------------------------------------------------------//
	static int				_init						( lua_State* L );

	//----------------------------------------------------------------//
	void					MOAIAbstractDrawable_Draw				( int subPrimID );
	void					MOAIAbstractDrawable_DrawDebug			( int subPrimID );
	ZLBounds				MOAIAbstractProp_GetModelBounds			();
	void					MOAINode_Update							();

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
