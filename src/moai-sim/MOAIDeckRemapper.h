// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKREMAPPER_H
#define	MOAIDECKREMAPPER_H

#include <moai-sim/MOAIDeckProxyBase.h>
#include <moai-sim/MOAINode.h>

//	@lua	MOAIBoundsDeck
//	@text	<p>Deck of bounding boxes. Bounding boxes are allocated in
//			a separate array from that used for box indices. The index
//			array is used to map deck indices onto bounding boxes. In
//			other words there may be more indices then boxes thus
//			allowing for re-use of boxes over multiple indices.</p>
//			
//			<p>The main purpose of the bounds deck is to override the
//			default bounds of elements inside of another deck. The
//			bounds deck may be attached to any other type of deck
//			by using MOAIDeck's setBoundsDeck () method.</p>

//================================================================//
// MOAIDeckRemapper
//================================================================//
/**	@lua	MOAIDeckRemapper
	@text	Remap deck indices. Most useful for controlling animated tiles in tilemaps.
			All indices are exposed as attributes that may be connected by setAttrLink
			or driven using MOAIAnim or MOAIAnimCurve.
*/
class MOAIDeckRemapper :
	public virtual MOAIDeckProxyBase {
protected:
	
	u32						mBase;
	ZLLeanArray < u32 >		mRemap;
	
	ZLLeanArray < ZLBox >	mBoundsArray;	// array of bounding boxes
	ZLLeanArray < u32 >		mIndexMap;		// maps bounding boxes onto deck item indices
	
	//----------------------------------------------------------------//
//	static int	_reserveBounds			( lua_State* L );
//	static int	_reserveIndices			( lua_State* L );
//	static int	_setBounds				( lua_State* L );
//	static int	_setIndex				( lua_State* L );
	
	//----------------------------------------------------------------//
	static int			_reserve				( lua_State* L );
	static int			_setBase				( lua_State* L );
	static int			_setRemap				( lua_State* L );

	//----------------------------------------------------------------//
	u32					Remap					( u32 idx ) const;
	static u32			Remap					( const MOAIDeckRemapper* remapper, u32 idx );

	//----------------------------------------------------------------//
	bool						MOAINode_ApplyAttrOp			( u32 attrID, MOAIAttribute& attr, u32 op );
	MOAICollisionShape*			MOAIDeck_GetCollisionShape		( u32 idx );

public:
	
	//----------------------------------------------------------------//
	ZLBox				ComputeMaxBounds				();
	bool				Contains						( u32 idx, const ZLVec2D& vec );
	void				DrawIndex						( u32 idx, MOAIMaterialBatch* materials, ZLVec3D offset, ZLVec3D scale );
	ZLBox				GetBounds						();
	ZLBox				GetBounds						( u32 idx );
	ZLBox				GetItemBounds					( u32 idx );
	bool				Inside							( u32 idx, MOAIMaterialBatch* materials, u32 granularity, ZLVec3D vec, float pad );
						MOAIDeckRemapper				();
						~MOAIDeckRemapper				();
	void				RegisterLuaClass				( MOAILuaState& state );
	void				RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
