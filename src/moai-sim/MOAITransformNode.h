// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORMNODE_H
#define	MOAITRANSFORMNODE_H

#include <moai-sim/MOAITransformNodeBase.h>

//================================================================//
// MOAITransformNode
//================================================================//
/**	@lua	MOAITransformNode
	@text	Base class for 2D affine transforms.
	
	@attr	ATTR_WORLD_X_LOC
	@attr	ATTR_WORLD_Y_LOC
	@attr	ATTR_WORLD_Z_LOC
	@attr	ATTR_WORLD_Z_ROT
	@attr	ATTR_WORLD_X_SCL
	@attr	ATTR_WORLD_Y_SCL
	@attr	ATTR_WORLD_Z_SCL
	@attr	TRANSFORM_TRAIT
*/
class MOAITransformNode :
	public virtual MOAITransformNodeBase {
protected:

	//----------------------------------------------------------------//
	static int	_setParent			( lua_State* L );

	//----------------------------------------------------------------//
	bool		MOAINode_ApplyAttrOp		( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );
	void		MOAINode_Update				();

public:
	
	DECL_ATTR_HELPER ( MOAITransformNode )
	
	enum {
		
		INHERIT_LOC,
		INHERIT_TRANSFORM,
		
		TOTAL_ATTR,
	};
	
	GET ( ZLVec3D, WorldLoc, mLocalToWorldMtx.GetTranslation ())
	
	//----------------------------------------------------------------//
				MOAITransformNode			();
				~MOAITransformNode			();
	void		RegisterLuaClass			( MOAILuaState& state );
	void		RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
