// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISELECTIONMESH_H
#define	MOAISELECTIONMESH_H

#include <moai-sim/MOAIDeckProxy.h>
#include <moai-sim/MOAIMesh.h>

//================================================================//
// MOAISelectionSpan
//================================================================//
class MOAISelectionSpan :
	public MOAIMeshSpan {
private:
	
	friend class MOAISelectionMesh;
	
	ZLIndex				mSetID;
	
	MOAISelectionSpan*	mPrevInMaster;
	MOAISelectionSpan*	mNextInMaster;
};

//================================================================//
// MOAISelectionMesh
//================================================================//
// TODO: doxygen
class MOAISelectionMesh :
	public MOAIDeckProxy {
protected:

	ZLLeanPool < MOAISelectionSpan, 16 >	mSpanPool;
	ZLLeanArray < MOAIMeshSpan* >			mSets;

	MOAISelectionSpan*	mSpanListHead;
	MOAIMesh*			mMesh;

	//----------------------------------------------------------------//
	static int				_addSelection				( lua_State* L );
	static int				_clearSelection				( lua_State* L );
	static int				_mergeSelection				( lua_State* L );
	static int				_printSelection				( lua_State* L );
	static int				_reserveSelections			( lua_State* L );
	static int				_setMesh					( lua_State* L );

	//----------------------------------------------------------------//
	ZLResult < ZLIndex >	AffirmSpanSet				();
	MOAISelectionSpan*		AllocSpan					( ZLIndex set, ZLIndex base, ZLIndex top );
//	void					BooleanAnd					( u32 set, u32 a, u32 b );
//	void					BooleanOr					( u32 set, u32 a, u32 b );
//	void					BooleanNot					( u32 set, u32 a, u32 b );
//	void					BooleanXor					( u32 set, u32 a, u32 b );
	void					ChangeSpanSet				( MOAISelectionSpan* span, ZLIndex set );
	void					Clear						();
	void					FixOverlaps					( MOAISelectionSpan* span );
	void					FreeSpan					( MOAISelectionSpan* span );
	MOAISelectionSpan*		InsertSpan					( MOAISelectionSpan* span, MOAISelectionSpan* prevInMaster, MOAISelectionSpan* prevInSet );

	//----------------------------------------------------------------//
	void					MOAIDeck_Draw						( ZLIndex idx );
	void					MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:

//	enum {
//		AND,
//		OR,
//		NOT,
//		XOR,
//	};

	DECL_LUA_FACTORY ( MOAISelectionMesh )

	//----------------------------------------------------------------//
	void					AddSelection				( ZLIndex set, ZLIndex base, ZLIndex top );
	void					ClearSelection				( ZLIndex set );
	void					ClearSelection				( ZLIndex set, ZLIndex base, ZLIndex top );
	void					MergeSelection				( ZLIndex set, ZLIndex merge );
							MOAISelectionMesh			();
							~MOAISelectionMesh			();
	void					PrintSelection				( ZLIndex set );
	void					PrintSelections				();
	void					ReserveSelections			( ZLSize total );
};

#endif
