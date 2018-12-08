// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXEDPROPBASE_H
#define	MOAIINDEXEDPROPBASE_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIDeckPropBase.h>
#include <moai-sim/MOAIDrawable.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAITransform.h>
#include <moai-core/MOAILuaSharedPtr.h>

//================================================================//
// MOAIIndexedPropBase
//================================================================//
// TODO: doxygen
class MOAIIndexedPropBase :
	public virtual MOAIDeckPropBase {
protected:
	
	ZLIndex		mIndex;
	
	//----------------------------------------------------------------//
	static int		_getIndex					( lua_State* L );
	static int		_setIndex					( lua_State* L );

	//----------------------------------------------------------------//
	bool			MOAINode_ApplyAttrOp		( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );

public:

	DECL_ATTR_HELPER ( MOAIIndexedPropBase )

	enum {
		ATTR_INDEX,
		TOTAL_ATTR,
	};

	GET_SET ( ZLIndex, Index, mIndex )

	//----------------------------------------------------------------//
					MOAIIndexedPropBase			();
					~MOAIIndexedPropBase		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
