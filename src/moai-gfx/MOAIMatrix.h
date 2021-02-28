// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATRIX_H
#define	MOAIMATRIX_H

#include <moai-gfx/MOAIAbstractChildTransform.h>

//================================================================//
// MOAIMatrix
//================================================================//
// TODO: doxygen
class MOAIMatrix :
	public MOAIAbstractChildTransform,
	public ZLAffine3D {
protected:

	//----------------------------------------------------------------//
	static int		_getMatrix				( lua_State* L );
	static int		_invert					( lua_State* L );
	static int		_setMatrix				( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass									( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs									( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAbstractBaseTransform_BuildLocalToWorldMtx		( ZLAffine3D& localToWorldMtx );
	bool			MOAINode_ApplyAttrOp								( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	DECL_LUA_FACTORY ( MOAIMatrix )
	DECL_ATTR_HELPER ( MOAIMatrix )
	
	enum {
		ATTR_MATRIX,
	};
	
	//----------------------------------------------------------------//
					MOAIMatrix				( ZLContext& context );
					~MOAIMatrix				();
};

#endif
