// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKHOLDERWITHINDEX_H
#define	MOAIDECKHOLDERWITHINDEX_H

#include <moai-sim/MOAIAbstractDrawable.h>
#include <moai-sim/ZLBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIDeckHolder.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAITransform.h>
#include <moai-core/MOAILuaSharedPtr.h>

//================================================================//
// MOAIDeckHolderWithIndex
//================================================================//
// TODO: doxygen
class MOAIDeckHolderWithIndex :
	public virtual MOAIDeckHolder {
protected:
	
	ZLIndex		mIndex;
	
	//----------------------------------------------------------------//
	static int		_getIndex					( lua_State* L );
	static int		_setIndex					( lua_State* L );

	//----------------------------------------------------------------//
	bool			MOAINode_ApplyAttrOp		( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );

public:

	DECL_ATTR_HELPER ( MOAIDeckHolderWithIndex )

	enum {
		ATTR_INDEX,
		TOTAL_ATTR,
	};

	GET_SET ( ZLIndex, Index, mIndex )

	//----------------------------------------------------------------//
					MOAIDeckHolderWithIndex		();
					~MOAIDeckHolderWithIndex	();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
