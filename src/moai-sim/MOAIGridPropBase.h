// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRIDPROPBASE_H
#define	MOAIGRIDPROPBASE_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIDeckPropBase.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>
#include <moai-core/MOAILuaSharedPtr.h>

class MOAICellCoord;
class MOAICollisionProp;
class MOAIDeck;
class MOAIDeckRemapper;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAISurfaceSampler2D;

class MOAIGridPropBase;

//================================================================//
// MOAIGridPropBase
//================================================================//
/**	@lua	MOAIGridPropBase
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	@attr	ATTR_PARTITION
*/
class MOAIGridPropBase :
	public virtual MOAIDeckPropBase {
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
	void				GetGridBoundsInView		( MOAICellCoord& c0, MOAICellCoord& c1 ); // TODO: this shoudln't be here
						MOAIGridPropBase				();
	virtual				~MOAIGridPropBase				();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
