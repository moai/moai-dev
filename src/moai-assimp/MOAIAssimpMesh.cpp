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
// TODO: doxygen
int MOAIAssimpMesh::_getIndices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	const aiMesh* mesh = self->mMesh;
	if ( !( mesh && ( mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE ))) return 0;

	u32 nIndices = mesh->mNumFaces * 3;

	if ( nIndices ) {
	
		u32 indexSize = state.GetValue < u32 >( 3, 4 );

		ZLStream* stream;

		MOAIIndexBuffer* idxBuffer = state.GetLuaObject < MOAIIndexBuffer >( 2, false );

		if ( idxBuffer ) {

			idxBuffer->SetIndexSize ( indexSize );
			idxBuffer->Reserve ( nIndices * indexSize );
			idxBuffer->Seek ( 0, SEEK_SET );
			stream = idxBuffer;
		}
		else {
		
			stream = state.GetLuaObject < MOAIStream >( 2, false );
		}
	
		if ( stream ) {

			self->ReadIndices ( *stream, indexSize );
		}
	}
	
	state.Push ( nIndices );
	return 1;
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

	unsigned int primitiveType = self->mMesh->mPrimitiveTypes;

	// TODO: mesh can contain multiple primitive types; see assimp docs
	if ( self->mMesh != NULL ) {
		switch ( primitiveType ) {
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

//----------------------------------------------------------------//
/**	@name	getVertexData
	@text	Returns a table that contains position vertices

	@in		MOAIImportFactory self
	@out	a Table that contain position vertices
*/
int MOAIAssimpMesh::_getVertexData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	if ( self->mMesh != NULL && self->mMesh->HasPositions ()) {
		MOAIAssimpUtil::PushMesh ( state, self->mMesh );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpMesh::_getVertices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpMesh, "U" )

	const aiMesh* mesh = self->mMesh;
	if ( !mesh ) return 0;

	u32 nVertices = self->mMesh->HasPositions () ? ( u32 )mesh->mNumVertices : 0;

	if ( nVertices ) {
	
		MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( -1, true );
		if ( !format ) return 0;

		u32 vertexSize = format->GetVertexSize ();
		u32 bufferSize = vertexSize * nVertices;

		ZLStream* stream;

		MOAIVertexBuffer* vtxBuffer = state.GetLuaObject < MOAIVertexBuffer >( 2, false );

		if ( vtxBuffer ) {
		
			vtxBuffer->Reserve ( bufferSize );
			vtxBuffer->Seek ( 0, SEEK_SET );
			stream = vtxBuffer;
		}
		else {
		
			stream = state.GetLuaObject < MOAIStream >( 2, false );
		}
	
		if ( stream ) {

			self->ReadVertices ( *format, *stream );
		}
	}
	
	state.Push ( nVertices );
	return 1;
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
u32 MOAIAssimpMesh::ReadIndices ( ZLStream& stream, u32 indexSize ) {

	const aiMesh* mesh = this->mMesh;
	if ( !( mesh && ( mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE ))) return 0;

	u32 nIndices = mesh->mNumFaces * 3;

	if ( nIndices > 0 ) {

		for ( u32 i = 0; i < mesh->mNumFaces; ++i ) {
				
			aiFace& face = mesh->mFaces [ i ];
			assert ( face.mNumIndices == 3 );

			for ( u32 j = 0; j < 3; ++j ) {
			
				u32 index = ( u32 )face.mIndices [ j ];

				if ( indexSize == 4 ) {
					stream.Write < u32 >( index );
				}
				else {
					stream.Write < u16 >(( u16 )index );
				}
			}
		}
	}

	return nIndices;
}

//----------------------------------------------------------------//
u32 MOAIAssimpMesh::ReadVertices ( const MOAIVertexFormat& format, ZLStream& stream ) {

	const aiMesh* mesh = this->mMesh;
	if ( !mesh ) return 0;

	u32 nVerts = ( u32 )mesh->mNumVertices;

	if ( nVerts > 0 ) {

		size_t base = stream.GetCursor ();

		for ( u32 i = 0; i < nVerts; ++i ) {
				
			format.SeekVertex ( stream, base, i );

			aiVector3D vertex = mesh->mVertices [ i ];
			format.WriteCoord ( stream, vertex.x, vertex.y, vertex.z, 1.0f );

			if ( mesh->HasVertexColors ( 0 )) {
				aiColor4D color = mesh->mColors [ 0 ][ i ];
				format.WriteColor ( stream, color.r, color.g, color.b, color.a );
			}
			else {
				format.WriteColor ( stream, 0xffffffff );
			}

			if ( mesh->HasTextureCoords ( 0 )) {
				aiVector3D uvw = mesh->mTextureCoords [ 0 ][ i ];
				format.WriteUV ( stream, uvw.x, uvw.y, uvw.z );
			}
		}

		//stream.SetCursor ( base );
	}

	return nVerts;
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
		{ "getIndices",					_getIndices },
		{ "getMaterialIndex",			_getMaterialIndex },
		{ "getName",					_getName },
		{ "getNormalsData",				_getNormalsData },
		{ "getPrimitiveType",			_getPrimitiveType },
		{ "getTangentsData",			_getTangentsData },
		{ "getUVData",					_getUVData },
		{ "getVertexColorData",			_getVertexColorData },
		{ "getVertexData",				_getVertexData },
		{ "getVertices",				_getVertices },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
