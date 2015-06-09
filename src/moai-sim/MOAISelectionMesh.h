// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
	
	u32					mSetID;
	
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

	MOAIMesh* mMesh;

	//----------------------------------------------------------------//
	static int			_addSelection				( lua_State* L );
	static int			_clearSelection				( lua_State* L );
	static int			_mergeSelections			( lua_State* L );
	static int			_printSelection				( lua_State* L );
	static int			_reserveSelections			( lua_State* L );
	static int			_setMesh					( lua_State* L );

	//----------------------------------------------------------------//
	MOAISelectionSpan*	AllocSpan					( u32 set, size_t base, size_t top );
	void				ChangeSpanSet				( MOAISelectionSpan* span, u32 set );
	void				Clear						();
	void				DrawIndex					( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	MOAISelectionSpan*	FreeSpanAndGetNext			( MOAISelectionSpan* span );

public:
	
	DECL_LUA_FACTORY ( MOAISelectionMesh )
	
	//----------------------------------------------------------------//
	void				AddSelection				( u32 set, size_t base, size_t top );
	void				ClearSelection				( u32 set );
	void				MergeSelections				( u32 set, u32 merge );
						MOAISelectionMesh			();
						~MOAISelectionMesh			();
	void				PrintSelection				( u32 idx );
	void				PrintSelections				();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveSelections			( u32 total );
	void				Select						( u32 set, size_t base, size_t size );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
