// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAINODEMGR_H
#define MOAINODEMGR_H

class MOAINode;

//================================================================//
// MOAINodeMgr
//================================================================//
class MOAINodeMgr :
	public MOAIGlobalClass < MOAINodeMgr, MOAILuaObject > {
private:

	MOAINode* mUpdateListHead;
	MOAINode* mUpdateListTail;

	//----------------------------------------------------------------//
	static int		_reset				( lua_State* L );
	static int		_update				( lua_State* L );

	//----------------------------------------------------------------//
	void			InsertAfter			( MOAINode& cursor, MOAINode& node );
	void			InsertBefore		( MOAINode& cursor, MOAINode& node );
	void			PushBack			( MOAINode& node );
	void			PushFront			( MOAINode& node );
	void			Remove				( MOAINode& node );

public:

	friend class MOAINode;

	DECL_LUA_SINGLETON ( MOAINodeMgr )

	//----------------------------------------------------------------//
	void			Reset				();
					MOAINodeMgr			();
					~MOAINodeMgr		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Update				();
};

#endif
