// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISCRIPTNODE_H
#define	MOAISCRIPTNODE_H

#include <moaicore/MOAINode.h>

//================================================================//
// MOAIScriptNode
//================================================================//
/**	@brief User scriptable dependency node.
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
	
	DECL_LUA_DATA ( MOAIScriptNode )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32				CountAttributes			();
					MOAIScriptNode			();
					~MOAIScriptNode			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
};

#endif
