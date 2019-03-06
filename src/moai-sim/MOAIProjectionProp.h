// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROJECTIONPROP_H
#define	MOAIPROJECTIONPROP_H

#include <moai-sim/MOAIAbstractDrawable.h>
#include <moai-sim/MOAIPartitionHull.h>

class MOAIAbstractViewLayer;

//================================================================//
// MOAIProjectionProp
//================================================================//
// TODO: doxygen
class MOAIProjectionProp :
	public virtual MOAIAbstractDrawable,
	public virtual MOAIPartitionHull {
protected:

	MOAILuaSharedPtr < MOAIGraphicsPropBase >	mSourceProp;
	MOAILuaSharedPtr < MOAIAbstractViewLayer >	mSourceLayer;
	MOAILuaSharedPtr < MOAIAbstractViewLayer >	mDestLayer;

	ZLReal				mFront;

	//----------------------------------------------------------------//
	static int			_init						( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAIAbstractDrawable_Draw				( int subPrimID );
	void				MOAIAbstractDrawable_DrawDebug			( int subPrimID );
	bool				MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void				MOAINode_Update							();

public:

	DECL_LUA_FACTORY ( MOAIProjectionProp )

	enum {
		DEBUG_DRAW_WORLD_BOUNDS,
		TOTAL_DEBUG_LINE_STYLES,
	};

	enum {
		ATTR_FRONT,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
						MOAIProjectionProp			();
	virtual				~MOAIProjectionProp			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
