// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAINODEMGR_H
#define MOAINODEMGR_H

#include <moai-core/MOAILua.h>

class MOAINode;

//================================================================//
// MOAINodeMgr
//================================================================//
class MOAINodeMgr :
	public virtual ZLContextClass,
	public virtual MOAILuaObject {
private:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	static const u32 DEFAULT_MAX_ITERATIONS = 3; // arbitrary number

	MOAINode* mUpdateListHead;
	MOAINode* mUpdateListTail;

	bool mScheduled;
	u32 mMaxIterations;

	//----------------------------------------------------------------//
	static int		_reset				( lua_State* L );
	static int		_setMaxIterations	( lua_State* L );
	static int		_update				( lua_State* L );

	//----------------------------------------------------------------//
	void			InsertAfter			( MOAINode& cursor, MOAINode& node );
	void			InsertBefore		( MOAINode& cursor, MOAINode& node );
	void			PushBack			( MOAINode& node );
	void			PushFront			( MOAINode& node );
	void			Remove				( MOAINode& node );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	friend class MOAINode;

	DECL_LUA_SINGLETON ( MOAINodeMgr )

	GET_SET ( u32, MaxIterations, mMaxIterations )

	//----------------------------------------------------------------//
	void			Reset				();
					MOAINodeMgr			();
					~MOAINodeMgr		();
	void			Update				();
};

#endif
