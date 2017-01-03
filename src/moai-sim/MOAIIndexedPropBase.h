// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXEDPROPBASE_H
#define	MOAIINDEXEDPROPBASE_H

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

class MOAIProp;

//================================================================//
// MOAIProp
//================================================================//
/**	@lua	MOAIProp
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	@attr	ATTR_PARTITION
*/
class MOAIIndexedPropBase :
	public virtual MOAIDeckPropBase {
private:
	
	//----------------------------------------------------------------//
	static int		_getIndex					( lua_State* L );
	static int		_setIndex					( lua_State* L );

protected:

	u32				mIndex;

	//----------------------------------------------------------------//
	bool			ApplyAttrOp					( u32 attrID, MOAIAttribute& attr, u32 op );

public:

	DECL_ATTR_HELPER ( MOAIIndexedPropBase )

	enum {
		ATTR_INDEX,
		TOTAL_ATTR,
	};

	GET_SET ( u32, Index, mIndex )

	//----------------------------------------------------------------//
					MOAIIndexedPropBase			();
					~MOAIIndexedPropBase		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
