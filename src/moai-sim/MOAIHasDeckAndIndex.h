// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASDECKANDINDEX_H
#define	MOAIHASDECKANDINDEX_H

#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIHasDeck.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// MOAIHasDeckAndIndex
//================================================================//
// TODO: doxygen
class MOAIHasDeckAndIndex :
	public virtual MOAIHasDeck {
protected:
	
	ZLIndex		mIndex;
	
	//----------------------------------------------------------------//
	static int		_getIndex					( lua_State* L );
	static int		_setIndex					( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	bool			MOAINode_ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	DECL_ATTR_HELPER ( MOAIHasDeckAndIndex )

	enum {
		ATTR_INDEX,
		TOTAL_ATTR,
	};

	GET_SET ( ZLIndex, Index, mIndex )

	//----------------------------------------------------------------//
					MOAIHasDeckAndIndex		();
					~MOAIHasDeckAndIndex	();
};

#endif
