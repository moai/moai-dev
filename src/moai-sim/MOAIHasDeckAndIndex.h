// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASDECKANDINDEX_H
#define	MOAIHASDECKANDINDEX_H

#include <moai-sim/MOAIHasDeck.h>
#include <moai-sim/MOAIPartitionHull.h>

//================================================================//
// MOAIHasDeckAndIndex
//================================================================//
// TODO: doxygen
class MOAIHasDeckAndIndex :
	public virtual MOAIHasDeck {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	ZLIndex		mIndex;
	
	//----------------------------------------------------------------//
	static int			_getIndex					( lua_State* L );
	static int			_setIndex					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_SerializeIn						( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				_SerializeOut						( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	MOAIPickResult		MOAIAbstractPickable_PickByPoint	( ZLVec3D loc );
	MOAIPickResult		MOAIAbstractPickable_PickByRay		( ZLVec3D loc, ZLVec3D normal );
	bool				MOAINode_ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	DECL_ATTR_HELPER ( MOAIHasDeckAndIndex )

	enum {
		ATTR_INDEX,
		TOTAL_ATTR,
	};

	GET_SET ( ZLIndex, Index, mIndex )

	//----------------------------------------------------------------//
						MOAIHasDeckAndIndex			();
						~MOAIHasDeckAndIndex		();
};

#endif
