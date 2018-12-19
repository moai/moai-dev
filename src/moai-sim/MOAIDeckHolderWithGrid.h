// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECKHOLDERWITHGRID_H
#define	MOAIDECKHOLDERWITHGRID_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIDeckHolder.h>
#include <moai-sim/MOAIDrawable.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAITransform.h>
#include <moai-core/MOAILuaSharedPtr.h>

class MOAICellCoord;
class MOAICollisionProp;
class MOAIDeck;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAISurfaceSampler2D;

//================================================================//
// MOAIDeckHolderWithGrid
//================================================================//
/**	@lua	MOAIDeckHolderWithGrid
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	@attr	ATTR_PARTITION
*/
class MOAIDeckHolderWithGrid :
	public virtual MOAIDeckHolder {
private:
	
	//----------------------------------------------------------------//
	static int			_getGrid					( lua_State* L );
	static int			_setGrid					( lua_State* L );
	static int			_setGridScale				( lua_State* L );

protected:
	
	MOAILuaSharedPtr < MOAIGrid >			mGrid;
	ZLVec2D									mGridScale;

public:

	//----------------------------------------------------------------//
						MOAIDeckHolderWithGrid		();
	virtual				~MOAIDeckHolderWithGrid		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
