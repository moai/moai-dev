// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCRIPTNODE_H
#define	MOAISCRIPTNODE_H

#include <moai-sim/MOAINode.h>


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
	bool			MOAINode_ApplyAttrOp	( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );
	void			MOAINode_Update			();

public:
	
	DECL_LUA_FACTORY ( MOAIScriptNode )
	
	//----------------------------------------------------------------//
					MOAIScriptNode			();
					~MOAIScriptNode			();
	void			NamedAttrAdd			( ZLIndex attrID, MOAIAttribute& attr );
	void			NamedAttrGet			( ZLIndex attrID, MOAIAttribute& attr );
	void			NamedAttrSet			( ZLIndex attrID, MOAIAttribute& attr );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
