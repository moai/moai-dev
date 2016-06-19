// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-assimp/MOAIAssimpMesh.h>
#include <moai-assimp/MOAIAssimpUtil.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpMesh::_countBones ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	lua_pushinteger ( state , self->mMesh && self->mMesh->HasBones () ? self->mMesh->mNumBones : 0 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpMesh::_countColorChannels ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	lua_pushinteger ( state , self->mMesh ? self->mMesh->GetNumColorChannels () : 0 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpMesh::_countFaces ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	lua_pushinteger ( state , self->mMesh && self->mMesh->HasFaces () ? self->mMesh->mNumFaces : 0 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpMesh::_countUVChannels ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	lua_pushinteger ( state , self->mMesh ? self->mMesh->GetNumUVChannels () : 0 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpMesh::_countUVs ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	u32 channel  = state.GetValue < u32 >( 2, 0 );
	lua_pushinteger ( state, self->mMesh && channel < self->mMesh->GetNumUVChannels () ? self->mMesh->mNumUVComponents [ channel ] : 0 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getNumvertices
	@text	Return the number of vertices in a mesh

	@in		MOAIAssimpMesh self
	@out	number of vertices in a mesh or nil
*/
int MOAIAssimpMesh::_countVertices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	lua_pushinteger ( state , self->mMesh && self->mMesh->HasPositions () ? self->mMesh->mNumVertices : 0 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getBitangentsData
	@text	Returns a table that contains bitangents vectors

	@in		MOAIImportFactory self
	@out	a Table that contain bitangent vectors
*/
int MOAIAssimpMesh::_getBitangentsData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	if (( self->mMesh != NULL ) && ( self->mMesh->HasTangentsAndBitangents ())) {
		MOAIAssimpUtil::PushBitangentsArray ( state , self->mMesh );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	gethFacesData
	@text	Returns a table that contains vertex indices

	@in		MOAIImportFactory self
	@out	a Table that contain vertex indices
*/
int MOAIAssimpMesh::_getFacesData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	if (( self->mMesh != NULL ) && ( self->mMesh->HasNormals ())) {
		MOAIAssimpUtil::PushFace ( state , self->mMesh);
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getMaterialIndex
	@text	Returns a mesh's index to a scene material table

	@in		MOAIImportFactory self
	@out	an integer index to a scene material table
*/
int MOAIAssimpMesh::_getMaterialIndex ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	if (( self->mMesh != NULL )) {
		lua_pushinteger ( state , self->mMesh->mMaterialIndex );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getMeshData
	@text	Returns a table that contains position vertices

	@in		MOAIImportFactory self
	@out	a Table that contain position vertices
*/
int MOAIAssimpMesh::_getMeshData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	if ( self->mMesh != NULL && self->mMesh->HasPositions ()) {
		MOAIAssimpUtil::PushMesh ( state, self->mMesh );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getName
	@text	Return mesh's name

	@in		MOAIAssimpMesh self
	@out	string containing a mesh's name or nil
*/
int MOAIAssimpMesh::_getName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	if (( self->mMesh != NULL ) && ( self->mName != NULL )) {
		if ( std::strlen ( self->mName ) != 0 ) {
			lua_pushstring ( state , self->GetName ());
		}
		else {
			char indexString [ 33 ];
			sprintf ( indexString,"mesh_%d", self->mIndex );
			lua_pushstring ( state , indexString );
		}
	}
	else {
		lua_pushnil ( state );
	}
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getNormalsData
	@text	Returns a table that contains normals vectors

	@in		MOAIImportFactory self
	@out	a Table that contain normal vectors
*/
int MOAIAssimpMesh::_getNormalsData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	if (( self->mMesh ) != NULL && ( self->mMesh->HasNormals ())) {
		MOAIAssimpUtil::PushNormalsArray ( state, self->mMesh );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpMesh::_getPrimitiveType ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	unsigned int primitiveTyoe = self->mMesh->mPrimitiveTypes;

	if ( self->mMesh != NULL ) {
		switch( primitiveTyoe) {
			case aiPrimitiveType_POINT:
				lua_pushinteger ( state, ( u32 )ZGL_PRIM_POINTS );
				break;
			case aiPrimitiveType_LINE:
				lua_pushinteger ( state, ( u32 )ZGL_PRIM_LINES );
				break;
			case aiPrimitiveType_TRIANGLE:
				lua_pushinteger ( state, ( u32 )ZGL_PRIM_TRIANGLES );
				break;
			case aiPrimitiveType_POLYGON:
				lua_pushnil ( state );
				break;
		}
	}
	else {
		lua_pushnil ( state );
	}
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTangentsData
	@text	Returns a table that contains tangents vectors

	@in		MOAIImportFactory self
	@out	a Table that contain tangent vectors
*/
int MOAIAssimpMesh::_getTangentsData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	if (( self->mMesh != NULL ) && ( self->mMesh->HasTangentsAndBitangents ())) {
		MOAIAssimpUtil::PushTangentsArray ( state , self->mMesh );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getUVData
	@text	Returns a table that contains UV Coordinates

	@in		MOAIImportFactory self
	@out	a Table that contain UV Coordniates Tables
*/
int MOAIAssimpMesh::_getUVData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "UN" )

	u32 channel  = state.GetValue < u32 >( 2, 0 );

	if (( self->mMesh != NULL ) && ( channel < self->mMesh->GetNumUVChannels ()) && ( self->mMesh->HasTextureCoords ( channel ))) {
		MOAIAssimpUtil::PushUVChannelArray ( state, self->mMesh->mTextureCoords [ channel ], self->mMesh->mNumVertices, self->mMesh->mNumUVComponents [ channel ]);
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getVertexColorData
	@text	Returns a table that contains color data vertex 4d vector

	@in		MOAIAssimpMesh self
	@in     Color Vertex channel number
	@out	a Table that color data vertex 4d vectors
*/
int MOAIAssimpMesh::_getVertexColorData ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "UN" )
	u32 channel  = state.GetValue < u32 >( 2, 0 );

	if (( self->mMesh != NULL ) && ( channel < self->mMesh->GetNumColorChannels ()) && ( self->mMesh->HasVertexColors ( channel ))) {
		MOAIAssimpUtil::PushColorsChannelArray ( state , self->mMesh->mColors [ channel ], self->mMesh->mNumVertices );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIAssimpMesh
//================================================================//

//----------------------------------------------------------------//
MOAIAssimpMesh::MOAIAssimpMesh () :
	mMesh ( 0 ),
	mIndex (( uint )-1 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAssimpMesh::~MOAIAssimpMesh () {
}

//----------------------------------------------------------------//
void MOAIAssimpMesh::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAssimpMesh::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getNumBones",				_countBones },
		{ "getNumFaces",				_countFaces },
		{ "getNumColorChannels",		_countColorChannels },
		{ "getNumUVChannels",			_countUVChannels },
		{ "getNumUVs",					_countUVs },
		{ "getNumVertices",				_countVertices },
		{ "getBitangentsData",			_getBitangentsData },
		{ "getFacesData",				_getFacesData },
		{ "getMaterialIndex",			_getMaterialIndex },
		{ "getMeshData",				_getMeshData },
		{ "getName",					_getName },
		{ "getNormalsData",				_getNormalsData },
		{ "getPrimitiveType",			_getPrimitiveType },
		{ "getTangentsData",			_getTangentsData },
		{ "getUVData",					_getUVData },
		{ "getVertexColorData",			_getVertexColorData },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
