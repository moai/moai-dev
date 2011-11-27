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
	@attr	ATTR_WORLD_Z_ROT
	@attr	ATTR_WORLD_X_SCL
	@attr	ATTR_WORLD_Y_SCL
*/
class MOAITransformBase :
	public virtual MOAINode {
protected:
	
	USAffine2D		mLocalToWorldMtx;
	USAffine2D		mWorldToLocalMtx;

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
		ATTR_WORLD_Z_ROT,
		ATTR_WORLD_X_SCL,
		ATTR_WORLD_Y_SCL,
		
		TRANSFORM_TRAIT,
		
		TOTAL_ATTR,
	};
	
	GET ( float, WorldXLoc, mLocalToWorldMtx.m [ USAffine2D::C2_R0 ])
	GET ( float, WorldYLoc, mLocalToWorldMtx.m [ USAffine2D::C2_R1 ])
	
	//----------------------------------------------------------------//
	bool					ApplyAttrOp					( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	const USAffine2D&		GetLocalToWorldMtx			();
	const USAffine2D*		GetLocTrait					();
	const USAffine2D*		GetTransformTrait			();
	const USAffine2D&		GetWorldToLocalMtx			();
							MOAITransformBase			();
							~MOAITransformBase			();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
