// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#ifndef MOAIASSIMPMESH_H
#define MOAIASSIMPMESH_H

#include <moai-assimp/MOAIAssimpSceneMember.h>

//================================================================//
// MOAIAssimpMesh
//================================================================//
// TODO: doxygen
class MOAIAssimpMesh :
	public MOAIAssimpSceneMember {
private:

	const aiMesh*		mMesh; // TODO: danger here; need to keep track of these in the scene and invalidate them

	//----------------------------------------------------------------//
	static int			_countBones					( lua_State* L );
	static int			_countColorChannels			( lua_State* L );
	static int			_countFaces					( lua_State* L );
	static int			_countUVChannels			( lua_State* L );
	static int			_countUVs					( lua_State* L );
	static int			_countVertices				( lua_State* L );
	static int			_getBitangentsData			( lua_State* L );
	static int			_getBones					( lua_State* L );
	static int			_getFacesData				( lua_State* L );
	static int			_getIndices					( lua_State* L );
	static int			_getMaterialIndex			( lua_State* L );
	static int			_getNormalsData				( lua_State* L );
	static int			_getPrimitiveType			( lua_State* L );
	static int			_getTangentsData			( lua_State* L );
	static int			_getUVData					( lua_State* L );
	static int			_getVertexColorData			( lua_State* L );
	static int			_getVertexData				( lua_State* L );
	static int			_getVertices				( lua_State* L );

public:

	GET_SET_CONST ( aiMesh*, Mesh, mMesh )

	DECL_LUA_FACTORY ( MOAIAssimpMesh )

	//----------------------------------------------------------------//
					MOAIAssimpMesh				();
					~MOAIAssimpMesh				();
	u32				ReadIndices					( ZLStream& stream, u32 indexSize );
	u32				ReadVertices				( const MOAIVertexFormat& format, ZLStream& stream, bool renormalizeBones );
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
