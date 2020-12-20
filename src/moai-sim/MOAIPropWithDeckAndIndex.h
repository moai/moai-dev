// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROPWITHDECKANDINDEX_H
#define	MOAIPROPWITHDECKANDINDEX_H

#include <moai-sim/MOAIAbstractProp.h>
#include <moai-sim/MOAIHasDeck.h>

//================================================================//
// MOAIPropWithDeckAndIndex
//================================================================//
// TODO: doxygen
class MOAIPropWithDeckAndIndex :
	public virtual MOAIAbstractProp,
	public virtual MOAIHasDeck {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLIndex		mIndex;

	//----------------------------------------------------------------//
	static int			_getIndex					( lua_State* L );
	static int			_setIndex					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds			MOAIAbstractProp_GetModelBounds			();
	MOAIPickResult		MOAIAbstractProp_PickByPoint			( ZLVec3D loc );
	MOAIPickResult		MOAIAbstractProp_PickByRay				( ZLVec3D loc, ZLVec3D normal );
	void				MOAIAbstractRenderNode_Render			( u32 renderPhase );
	bool				MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	DECL_ATTR_HELPER ( MOAIPropWithDeckAndIndex )

	enum {
		ATTR_INDEX,
		TOTAL_ATTR,
	};

	GET_SET ( ZLIndex, Index, mIndex )

	//----------------------------------------------------------------//
						MOAIPropWithDeckAndIndex		();
						~MOAIPropWithDeckAndIndex		();
};

#endif
