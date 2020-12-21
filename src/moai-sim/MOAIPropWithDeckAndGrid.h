// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROPWITHDECKANDGRID_H
#define	MOAIPROPWITHDECKANDGRID_H

#include <moai-sim/MOAIAbstractProp.h>
#include <moai-sim/MOAIHasDeck.h>

//================================================================//
// MOAIPropWithDeckAndGrid
//================================================================//
// TODO: doxygen
class MOAIPropWithDeckAndGrid :
	public virtual MOAIAbstractProp,
	public virtual MOAIHasDeck {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	MOAILuaSharedPtr < MOAIGrid >			mGrid;
	ZLVec2D									mGridScale;
	
	//----------------------------------------------------------------//
	static int			_getGrid				( lua_State* L );
	static int			_setGrid				( lua_State* L );
	static int			_setGridScale			( lua_State* L );

	//----------------------------------------------------------------//
	void				DrawGrid								( MOAIRenderPhaseEnum::_ renderPhase, const MOAICellCoord &c0, const MOAICellCoord &c1 );
	void				GetGridFrameInView						( const ZLAffine3D& worldToLocalMtx, MOAICellCoord& c0, MOAICellCoord& c1 ); // TODO: this shoudln't be here

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_SerializeIn							( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				_SerializeOut							( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	ZLBounds			MOAIAbstractProp_GetModelBounds			();
	MOAIPickResult		MOAIAbstractProp_PickByPoint			( ZLVec3D loc );
	MOAIPickResult		MOAIAbstractProp_PickByRay				( ZLVec3D loc, ZLVec3D normal );
	void				MOAIAbstractRenderNode_Render			( MOAIRenderPhaseEnum::_ renderPhase );
	bool				MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void				MOAIPartitionHull_AddToSortBuffer		( MOAIPartitionResultBuffer& buffer, u32 key = 0 );

public:

	//----------------------------------------------------------------//
						MOAIPropWithDeckAndGrid			();
						~MOAIPropWithDeckAndGrid		();
};

#endif
