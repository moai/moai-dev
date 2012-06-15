// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSFORMBASE_H
#define	MOAITRANSFORMBASE_H

#include <moaicore/MOAINode.h>

//================================================================//
// MOAITransformBase
//================================================================//
/**	@name	MOAITransformBase
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
class MOAITransformBase :
	public virtual MOAINode {
protected:
	
	USAffine3D		mLocalToWorldMtx;
	USAffine3D		mWorldToLocalMtx;

	//----------------------------------------------------------------//
	static int	_getWorldDir	( lua_State* L );
	static int	_getWorldLoc	( lua_State* L );
	static int	_getWorldRot	( lua_State* L );
	static int	_getWorldScl	( lua_State* L );

public:
	
	DECL_ATTR_HELPER ( MOAITransformBase )
	
	enum {
		ATTR_WORLD_X_LOC,
		ATTR_WORLD_Y_LOC,
		ATTR_WORLD_Z_LOC,
		
		ATTR_WORLD_Z_ROT,
		
		ATTR_WORLD_X_SCL,
		ATTR_WORLD_Y_SCL,
		ATTR_WORLD_Z_SCL,
		
		TRANSFORM_TRAIT,
		
		TOTAL_ATTR,
	};
	
	GET ( USVec3D, WorldLoc, mLocalToWorldMtx.GetTranslation ())
	
	//----------------------------------------------------------------//
	bool					ApplyAttrOp					( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	const USAffine3D&		GetLocalToWorldMtx			();
	const USAffine3D*		GetLocTrait					();
	const USAffine3D*		GetTransformTrait			();
	const USAffine3D&		GetWorldToLocalMtx			();
							MOAITransformBase			();
							~MOAITransformBase			();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
