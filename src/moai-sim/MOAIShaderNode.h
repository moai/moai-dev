// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADERNODE_H
#define	MOAISHADERNODE_H

//================================================================//
// MOAIShaderNode
//================================================================//
/**	@lua	MOAIShaderNode
	@text	This represents the "instance" of a shader program. Its purpose
			is to enable sharing of a single program across multiple sets of
			uniform values and to expose uniform values as MOAINode attributes.
*/
class MOAIShaderNode :
	public virtual MOAIShader {
protected:

	//----------------------------------------------------------------//
	bool						MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	DECL_LUA_FACTORY ( MOAIShaderNode )

	//----------------------------------------------------------------//
								MOAIShaderNode				();
								~MOAIShaderNode				();
	void						RegisterLuaClass			( MOAILuaState& state );
	void						RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
