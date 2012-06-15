// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCRIPTNODE_H
#define	MOAISCRIPTNODE_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAINode.h>

//================================================================//
// MOAIScriptNode
//================================================================//
/**	@name	MOAIScriptNode
	@text	User scriptable dependency node. User may specify Lua
			callback to handle node updating as well as custom floating
			point attributes.
*/
class MOAIScriptNode :
	public MOAINode {
private:

	MOAILuaLocal mOnUpdate;
	USLeanArray < float > mAttributes;

	//----------------------------------------------------------------//
	static int		_reserveAttrs			( lua_State* L );
	static int		_setCallback			( lua_State* L );
	

protected:

	//----------------------------------------------------------------//
	void			OnDepNodeUpdate			();

public:
	
	DECL_LUA_FACTORY ( MOAIScriptNode )
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
					MOAIScriptNode			();
					~MOAIScriptNode			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
