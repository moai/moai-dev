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
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:

	//----------------------------------------------------------------//
						MOAIHasDeckAndGrid		();
	virtual				~MOAIHasDeckAndGrid		();
};

#endif
