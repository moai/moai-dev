// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASDECKANDGRID_H
#define	MOAIHASDECKANDGRID_H

#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIHasDeck.h>
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
// MOAIHasDeckAndGrid
//================================================================//
// TODO: doxygen
class MOAIHasDeckAndGrid :
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
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:

	//----------------------------------------------------------------//
						MOAIHasDeckAndGrid		();
	virtual				~MOAIHasDeckAndGrid		();
};

#endif
