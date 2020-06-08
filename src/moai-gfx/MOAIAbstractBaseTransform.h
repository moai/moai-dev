// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTPARENTTRANSFORM_H
#define	MOAIABSTRACTPARENTTRANSFORM_H

//================================================================//
// MOAIAbstractBaseTransform
//================================================================//
/**	@lua	MOAIAbstractBaseTransform
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
class MOAIAbstractBaseTransform :
	public virtual MOAINode {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	ZLAffine3D		mLocalToWorldMtx;
	ZLAffine3D		mWorldToLocalMtx;

	//----------------------------------------------------------------//
	static int	_getWorldDir		( lua_State* L );
	static int	_getWorldLoc		( lua_State* L );
	static int	_getWorldRot		( lua_State* L );
	static int	_getWorldScl		( lua_State* L );
	static int	_getWorldXAxis		( lua_State* L );
	static int	_getWorldYAxis		( lua_State* L );
	static int	_getWorldZAxis		( lua_State* L );
	static int	_getWorldXNormal	( lua_State* L );
	static int	_getWorldYNormal	( lua_State* L );
	static int	_getWorldZNormal	( lua_State* L );
	static int	_modelToWorld		( lua_State* L );
	static int	_worldToModel		( lua_State* L );

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractBaseTransform_BuildLocalToWorldMtx		( ZLAffine3D& localToWorldMtx ) = 0;

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	bool				MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void				MOAINode_Update				();

public:
	
	DECL_ATTR_HELPER ( MOAIAbstractBaseTransform )
	
	GET ( ZLVec3D, WorldLoc, mLocalToWorldMtx.GetTranslation ())
	
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
	
	DECL_ATTR_ID ( ATTR_WORLD_X_LOC )
	DECL_ATTR_ID ( ATTR_WORLD_Y_LOC )
	DECL_ATTR_ID ( ATTR_WORLD_Z_LOC )
	
	DECL_ATTR_ID ( ATTR_WORLD_Z_ROT )
	
	DECL_ATTR_ID ( ATTR_WORLD_X_SCL )
	DECL_ATTR_ID ( ATTR_WORLD_Y_SCL )
	DECL_ATTR_ID ( ATTR_WORLD_Z_SCL )
	
	DECL_ATTR_ID ( TRANSFORM_TRAIT )
	
	//----------------------------------------------------------------//
	const ZLAffine3D&		GetLocalToWorldMtx			() const;
	const ZLAffine3D*		GetLocTrait					() const;
	const ZLAffine3D*		GetTransformTrait			() const;
	const ZLAffine3D&		GetWorldToLocalMtx			() const;
							MOAIAbstractBaseTransform		();
							~MOAIAbstractBaseTransform		();
};

#endif
