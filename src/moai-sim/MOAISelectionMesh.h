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
	static int			_addSelection				( lua_State* L );
	static int			_clearSelection				( lua_State* L );
	static int			_mergeSelection				( lua_State* L );
	static int			_printSelection				( lua_State* L );
	static int			_reserveSelections			( lua_State* L );
	static int			_setMesh					( lua_State* L );

	//----------------------------------------------------------------//
	ZLResult < u32 >	AffirmSpanSet				();
	MOAISelectionSpan*	AllocSpan					( u32 set, size_t base, size_t top );
//	void				BooleanAnd					( u32 set, u32 a, u32 b );
//	void				BooleanOr					( u32 set, u32 a, u32 b );
//	void				BooleanNot					( u32 set, u32 a, u32 b );
//	void				BooleanXor					( u32 set, u32 a, u32 b );
	void				ChangeSpanSet				( MOAISelectionSpan* span, u32 set );
	void				Clear						();
	void				DrawIndex					( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale );
	void				FixOverlaps					( MOAISelectionSpan* span );
	void				FreeSpan					( MOAISelectionSpan* span );
	MOAISelectionSpan*	InsertSpan					( MOAISelectionSpan* span, MOAISelectionSpan* prevInMaster, MOAISelectionSpan* prevInSet );

public:
	
//	enum {
//		AND,
//		OR,
//		NOT,
//		XOR,
//	};
	
	DECL_LUA_FACTORY ( MOAISelectionMesh )
	
	//----------------------------------------------------------------//
	void				AddSelection				( u32 set, size_t base, size_t top );
	void				ClearSelection				( u32 set );
	void				ClearSelection				( u32 set, size_t base, size_t top );
	void				MergeSelection				( u32 set, u32 merge );
						MOAISelectionMesh			();
						~MOAISelectionMesh			();
	void				PrintSelection				( u32 set );
	void				PrintSelections				();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveSelections			( u32 total );
	void				Select						( u32 set, size_t base, size_t size );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
