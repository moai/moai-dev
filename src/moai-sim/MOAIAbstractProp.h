// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTPROP_H
#define	MOAIABSTRACTPROP_H

#include <moai-sim/MOAIPartitionHull.h>

//================================================================//
// MOAIAbstractProp
//================================================================//
// TODO: doxygen
class MOAIAbstractProp :
	public virtual MOAIPartitionHull,
	public virtual MOAIAbstractRenderable,
	public virtual MOAITransform {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	u32					mFlags;
	ZLBox				mModelBoundsOverride;
	ZLVec3D				mModelBoundsPad;

	//----------------------------------------------------------------//
	static int			_getModelBounds				( lua_State* L );
	static int			_getModelBoundsSize			( lua_State* L );
	static int			_setExpandForSort			( lua_State* L );
	static int			_setFlag					( lua_State* L );
	static int			_setModelBounds				( lua_State* L );
	static int			_setModelBoundsPad			( lua_State* L );

	//----------------------------------------------------------------//
	void					LoadVertexTransform					();

	//----------------------------------------------------------------//
	virtual ZLBounds			MOAIAbstractProp_GetModelBounds			() = 0; // get the prop bounds in model space
	virtual ZLMatrix4x4			MOAIAbstractProp_GetWorldDrawingMtx		() const;
	virtual MOAIPickResult		MOAIAbstractProp_PickByPoint			( ZLVec3D loc );
	virtual MOAIPickResult		MOAIAbstractProp_PickByRay				( ZLVec3D loc, ZLVec3D normal );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIPickResult		MOAIAbstractPickable_PickByPoint		( ZLVec3D loc );
	MOAIPickResult		MOAIAbstractPickable_PickByRay			( ZLVec3D loc, ZLVec3D normal );
	bool				MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void				MOAINode_Update							();
	void				MOAIPartitionHull_AddToSortBuffer		( MOAIPartitionResultBuffer& buffer, u32 key );

public:

	enum {
		DEBUG_DRAW_PARTITION_CELLS,
		DEBUG_DRAW_PARTITION_PADDED_CELLS,
		DEBUG_DRAW_AXIS,
		DEBUG_DRAW_DIAGONALS,
		DEBUG_DRAW_MODEL_BOUNDS,
		DEBUG_DRAW_WORLD_BOUNDS,
		TOTAL_DEBUG_LINE_STYLES,
	};


	enum {
		FLAGS_OVERRIDE_BOUNDS		= 0x01,
		FLAGS_PAD_BOUNDS			= 0x02,
		FLAGS_EXPAND_FOR_SORT		= 0x04,
		FLAGS_PARTITION_GLOBAL		= 0x08,
	};

	//----------------------------------------------------------------//
	void				DrawDebug					();
	ZLBounds			GetModelBounds				(); // get the prop bounds in model space
	ZLMatrix4x4			GetWorldDrawingMtx			() const;
	bool				InsideModelBounds			( const ZLVec3D& vec, float pad );
						MOAIAbstractProp			( ZLContext& context );
	virtual				~MOAIAbstractProp			();
};

#endif
