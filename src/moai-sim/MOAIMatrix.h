// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATRIX_H
#define	MOAIMATRIX_H

#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAIMatrix
//================================================================//
// TODO: doxygen
class MOAIMatrix :
	public MOAITransformBase,
	public ZLAffine3D {
protected:

	//----------------------------------------------------------------//
	static int		_getMatrix				( lua_State* L );
	static int		_invert					( lua_State* L );
	static int		_setMatrix				( lua_State* L );

	//----------------------------------------------------------------//
	bool			MOAINode_ApplyAttrOp						( u32 attrID, MOAIAttribute& attr, u32 op );
	void			MOAITransformBase_BuildLocalToWorldMtx		( ZLAffine3D& localToWorldMtx );

public:

	DECL_LUA_FACTORY ( MOAIMatrix )
	DECL_ATTR_HELPER ( MOAIMatrix )
	
	enum {
		ATTR_MATRIX,
	};
	
	//----------------------------------------------------------------//
					MOAIMatrix				();
					~MOAIMatrix				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
