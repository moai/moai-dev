// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTCHILDTRANSFORM_H
#define	MOAIABSTRACTCHILDTRANSFORM_H

#include <moai-sim/MOAIAbstractBaseTransform.h>

//================================================================//
// MOAIAbstractChildTransform
//================================================================//
/**	@lua	MOAIAbstractChildTransform
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
class MOAIAbstractChildTransform :
	public virtual MOAIAbstractBaseTransform {
protected:

	//----------------------------------------------------------------//
	static int	_setParent							( lua_State* L );

	//----------------------------------------------------------------//
	void		MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void		MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	bool		MOAINode_ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void		MOAINode_Update						();

public:
	
	DECL_ATTR_HELPER ( MOAIAbstractChildTransform )
	
	enum {
		
		INHERIT_LOC,
		INHERIT_TRANSFORM,
		
		TOTAL_ATTR,
	};
	
	GET ( ZLVec3D, WorldLoc, mLocalToWorldMtx.GetTranslation ())
	
	//----------------------------------------------------------------//
				MOAIAbstractChildTransform			();
				~MOAIAbstractChildTransform			();
};

#endif
