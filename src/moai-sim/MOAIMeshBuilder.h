// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHBUILDER_H
#define	MOAIMESHBUILDER_H

#include <moai-sim/MOAIVertexFormat.h>

class MOAIGfxBuffer;

//================================================================//
// MOAIMeshBuilder
//================================================================//
class MOAIMeshBuilder :
	public virtual MOAIMemStream {
private:
	
	friend class VertexComparator;
	
	MOAILuaSharedPtr < MOAIVertexFormat > mVertexFormat;
	
	ZLMemStream			mIndices;
	
	void*				mVertices;
	u32					mTotalVertices;
	u32					mTotalInputVertices;
	
	ZLLeanArray < u32 > mIndexMap;
	
	//----------------------------------------------------------------//
	static int		_build					( lua_State* L );
	static int		_getMesh				( lua_State* L );
	static int		_setVertexFormat		( lua_State* L );
	static int		_writeIndex				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			ClearVertices			();
	void*			GetVertexBuffer			( void* vertices, size_t i );
	void			PrepareRawVertices		();
	
public:
	
	DECL_LUA_FACTORY ( MOAIMeshBuilder )
	
	GET ( ZLStream&, IndexStream, mIndices )
	GET ( MOAIVertexFormat*, VertexFormat, mVertexFormat );
	
	//----------------------------------------------------------------//
	void			Build					();
	u32				GetMesh					( MOAIGfxBuffer* vtxBuffer, MOAIGfxBuffer* idxBuffer, u32 idxSizeInBytes );
					MOAIMeshBuilder			();
					~MOAIMeshBuilder		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SnapCoords				( float xSnap, float ySnap, float zSnap );
};

#endif
