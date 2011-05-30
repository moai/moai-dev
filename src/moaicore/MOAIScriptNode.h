// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCRIPTNODE_H
#define	MOAISCRIPTNODE_H

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

	USLuaRef mOnUpdate;
	USLeanArray < float > mAttributes;

	//----------------------------------------------------------------//
	static int		_reserveAttrs			( lua_State* L );
	static int		_setCallback			( lua_State* L );
	

protected:

	//----------------------------------------------------------------//
	void			OnDepNodeUpdate		();

public:
	
	DECL_LUA_FACTORY ( MOAIScriptNode )
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
					MOAIScriptNode			();
					~MOAIScriptNode			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
};

#endif
