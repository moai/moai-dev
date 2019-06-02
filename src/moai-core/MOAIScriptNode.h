// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCRIPTNODE_H
#define	MOAISCRIPTNODE_H

#include <moai-core/MOAINode.h>

//================================================================//
// MOAIScriptNode
//================================================================//
/**	@lua	MOAIScriptNode
	@text	User scriptable dependency node. User may specify Lua
			callback to handle node updating as well as custom floating
			point attributes.
*/
class MOAIScriptNode :
	public virtual MOAINode {
private:

	MOAILuaMemberRef mOnUpdate;
	ZLLeanArray < float >	mAttributes;
	ZLLeanArray < cc8* >	mAttrNames;

	//----------------------------------------------------------------//
	static int		_reserveAttrs			( lua_State* L );
	static int		_setCallback			( lua_State* L );
	static int		_setAttrName			( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	bool			MOAINode_ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void			MOAINode_Update						();

public:
	
	DECL_LUA_FACTORY ( MOAIScriptNode )
	
	//----------------------------------------------------------------//
					MOAIScriptNode			();
					~MOAIScriptNode			();
	void			NamedAttrAdd			( ZLIndex attrID, ZLAttribute& attr );
	void			NamedAttrGet			( ZLIndex attrID, ZLAttribute& attr );
	void			NamedAttrSet			( ZLIndex attrID, ZLAttribute& attr );
};

#endif
