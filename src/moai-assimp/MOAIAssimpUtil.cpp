// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-assimp/MOAIAssimpUtil.h>

//================================================================//
// MOAIAssimpUtil
//================================================================//

//----------------------------------------------------------------//
cc8* MOAIAssimpUtil::EnumToString_aiTextureMapMode ( aiTextureMapMode mode ) {

	switch ( mode ) {
		case aiTextureMapMode_Wrap:			return "WRAP";
		case aiTextureMapMode_Clamp:		return "CLAMP";
		case aiTextureMapMode_Decal:		return "DECAL";
		case aiTextureMapMode_Mirror:		return "MIRROR";
		default:							return "UNKNOWN";
	}
}

//----------------------------------------------------------------//
cc8* MOAIAssimpUtil::EnumToString_aiTextureMapping ( aiTextureMapping mapping ) {

	switch ( mapping ) {
		case aiTextureMapping_UV:			return "UV";
		case aiTextureMapping_SPHERE:		return "SPHERE";
		case aiTextureMapping_CYLINDER:		return "CYLINDER";
		case aiTextureMapping_BOX:			return "BOX";
		case aiTextureMapping_PLANE:		return "PLANE";
		case aiTextureMapping_OTHER:		return "OTHER";
		default:							return "UNKNOWN";
	}
}

//----------------------------------------------------------------//
cc8* MOAIAssimpUtil::EnumToString_aiTextureOp ( aiTextureOp op ) {

	switch ( op ) {
		case aiTextureOp_Multiply:			return "MULTIPLY";
		case aiTextureOp_Add:				return "ADD";
		case aiTextureOp_Subtract:			return "SUBTRACT";
		case aiTextureOp_Divide:			return "DIVIDE";
		case aiTextureOp_SmoothAdd:			return "SMOOTH_ADD";
		case aiTextureOp_SignedAdd:			return "SIGNED_ADD";
		default:							return "UNKNOWN";
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushBitangentsArray( lua_State* L , const aiMesh* currentMesh ) {

	MOAIAssimpUtil::PushVectorArray ( L, currentMesh->mBitangents, currentMesh->mNumVertices, true );
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushBonesArray ( lua_State* L, const aiMesh* currentMesh ) {

	MOAILuaState state ( L );

	lua_newtable ( L );
	
	for ( uint i = 0; i < currentMesh->mNumBones; ++i ) {
	
		aiBone* bone = currentMesh->mBones [ i ];
		assert ( bone );
	
		lua_pushnumber ( L, i + 1 );
	
		lua_newtable ( L );
		state.SetField ( -1, "name", bone->mName.C_Str ());
		MOAIAssimpUtil::PushMatrix ( L, bone->mOffsetMatrix );
		lua_setfield ( L, -2, "offsetMatrix" );
		
		lua_settable ( L, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushColor ( lua_State* L, aiColor3D colorVector ) {

	MOAILuaState state ( L );

	lua_newtable ( L );
	state.SetField ( -1, "r", colorVector.r );
	state.SetField ( -1, "g", colorVector.g );
	state.SetField ( -1, "b", colorVector.b );
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushColorsChannelArray ( lua_State* L , aiColor4D* colorCoords , size_t numColorVectors ) {

	lua_newtable ( L );
	for ( int i = 0; i < ( int )numColorVectors; i++ ) {
		MOAIAssimpUtil::PushColorVector ( L, colorCoords [ i ]);
		lua_rawseti ( L , -2 , i + 1 );
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushColorVector ( lua_State* L  , aiColor4D colorVector ) {

	lua_newtable ( L );
	lua_pushnumber ( L , colorVector.r );
	lua_rawseti ( L , -2 , 1 );
	lua_pushnumber ( L , colorVector.g );
	lua_rawseti ( L , -2 , 2 );
	lua_pushnumber ( L , colorVector.b );
	lua_rawseti ( L , -2 , 3 );
	lua_pushnumber( L , colorVector.a );
	lua_rawseti ( L , -2 , 4 );
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushFace ( lua_State* L , const aiMesh* currentMesh ) {

	lua_newtable ( L );
	for( int i = 0 ; i < ( int )currentMesh->mNumFaces ; i++ ) {
		aiFace* currentFace = &( currentMesh->mFaces [ i ]);
		MOAIAssimpUtil::PushVertexIndex ( L , currentFace->mIndices , currentFace );
		lua_rawseti ( L , -2 , i + 1 );
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushMatrix ( lua_State* L, const aiMatrix4x4& matrix ) {

	MOAILuaState state ( L );

	lua_newtable ( L );
	
	lua_pushnumber ( L, 1 );
	lua_newtable ( L );
	state.SetFieldByIndex ( -1, 1, matrix.a1 );
	state.SetFieldByIndex ( -1, 2, matrix.a2 );
	state.SetFieldByIndex ( -1, 3, matrix.a3 );
	state.SetFieldByIndex ( -1, 4, matrix.a4 );
	lua_settable ( L, -3 );
	
	lua_pushnumber ( L, 2 );
	lua_newtable ( L );
	state.SetFieldByIndex ( -1, 1, matrix.b1 );
	state.SetFieldByIndex ( -1, 2, matrix.b2 );
	state.SetFieldByIndex ( -1, 3, matrix.b3 );
	state.SetFieldByIndex ( -1, 4, matrix.b4 );
	lua_settable ( L, -3 );
	
	lua_pushnumber ( L, 3 );
	lua_newtable ( L );
	state.SetFieldByIndex ( -1, 1, matrix.c1 );
	state.SetFieldByIndex ( -1, 2, matrix.c2 );
	state.SetFieldByIndex ( -1, 3, matrix.c3 );
	state.SetFieldByIndex ( -1, 4, matrix.c4 );
	lua_settable ( L, -3 );
	
	lua_pushnumber ( L, 4 );
	lua_newtable ( L );
	state.SetFieldByIndex ( -1, 1, matrix.d1 );
	state.SetFieldByIndex ( -1, 2, matrix.d2 );
	state.SetFieldByIndex ( -1, 3, matrix.d3 );
	state.SetFieldByIndex ( -1, 4, matrix.d4 );
	lua_settable ( L, -3 );
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushMesh ( lua_State* L , const aiMesh* currentMesh ) {

	MOAIAssimpUtil::PushVectorArray ( L, currentMesh->mVertices, currentMesh->mNumVertices, false );
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushNode ( lua_State* L, const aiNode* node ) {

	int stackOK = lua_checkstack ( L, 4 );
	assert ( stackOK );

	if ( !node ) return;
	
	MOAILuaState state ( L );
	
	lua_newtable ( L );
	
	state.SetField ( -1, "name", node->mName.C_Str ());
	
	MOAIAssimpUtil::PushMatrix ( L, node->mTransformation );
	lua_setfield ( L, -2, "transformation" );
	
	if ( node->mNumChildren ) {
	
		lua_newtable ( L );
		for ( uint i = 0; i < node->mNumChildren; ++i ) {
		
			lua_pushnumber ( L, i + 1 ); // index for child
			MOAIAssimpUtil::PushNode ( L, node->mChildren [ i ]);
			
			//lua_pushvalue ( L, -4 ); // copy the parent to the top
			//lua_setfield ( L, -2, "parent" ); // set the parent field
			
			lua_settable ( L, -3 ); // add to table of children
		}
		lua_setfield ( L, -2, "children" );
	}

	if ( node->mNumMeshes ) {
	
		lua_newtable ( L );
		for ( uint i = 0; i < node->mNumMeshes; ++i ) {
		
			lua_pushnumber ( L, i + 1 ); // index for child
			lua_pushnumber ( L, node->mMeshes [ i ]); // index for child
			lua_settable ( L, -3 );
		}
		lua_setfield ( L, -2, "meshes" );
	}

	if ( node->mMetaData ) {
	
		aiMetadata* metadata = node->mMetaData;
		
		lua_newtable ( L );
		for ( uint i = 0; i < metadata->mNumProperties; ++i ) {
		
			aiString& key				= metadata->mKeys [ i ];
			aiMetadataEntry& value		= metadata->mValues [ i ];
		
			lua_pushstring ( L, key.C_Str ());
		
			switch ( value.mType ) {
			
				case AI_BOOL:
					state.Push ( *( bool* )value.mData );
					break;
					
				case AI_INT:
					state.Push ( *( int* )value.mData );
					break;
					
				case AI_UINT64:
					state.Push ( *( uint64_t* )value.mData );
					break;
					
				case AI_FLOAT:
				
					state.Push ( *( float* )value.mData );
					break;
					
				case AI_AISTRING:
				
					state.Push ((( aiString* )value.mData )->C_Str ());
					break;
					
				case AI_AIVECTOR3D:
					MOAIAssimpUtil::PushVector ( state, *( aiVector3D* )value.mData );
					break;
				
				default:
					break;
			}
			
			lua_settable ( L, -3 );
		}
		lua_setfield ( L, -2, "metadata" );
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushNormalsArray ( lua_State* L , const aiMesh* currentMesh ) {

	MOAIAssimpUtil::PushVectorArray ( L, currentMesh->mNormals, currentMesh->mNumVertices, true );
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushTangentsArray ( lua_State* L , const aiMesh* currentMesh ) {

	MOAIAssimpUtil::PushVectorArray ( L, currentMesh->mTangents, currentMesh->mNumVertices, true );
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushTexture ( lua_State* L, aiMaterial* material, aiTextureType type, size_t idx ) {

	aiString path;
	aiTextureMapping mapping;
	uint uvindex;
	float blend;
	aiTextureOp op;
	aiTextureMapMode mapmode [ 3 ];

	material->GetTexture (
		type,
		( uint )idx,
		&path,
		&mapping,
		&uvindex,
		&blend,
		&op,
		mapmode
	);

	MOAILuaState state ( L );

	lua_newtable ( L );
	state.SetField ( -1, "path",		path.C_Str ());
	state.SetField ( -1, "mapping",		MOAIAssimpUtil::EnumToString_aiTextureMapping ( mapping ));
	state.SetField ( -1, "uvindex",		uvindex );
	state.SetField ( -1, "blend",		blend );
	state.SetField ( -1, "op",			MOAIAssimpUtil::EnumToString_aiTextureOp ( op ));
	state.SetField ( -1, "mapmode_u",	MOAIAssimpUtil::EnumToString_aiTextureMapMode ( mapmode [ 0 ]));
	state.SetField ( -1, "mapmode_v",	MOAIAssimpUtil::EnumToString_aiTextureMapMode ( mapmode [ 1 ]));
	state.SetField ( -1, "mapmode_w",	MOAIAssimpUtil::EnumToString_aiTextureMapMode ( mapmode [ 2 ]));
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushTextureStack ( lua_State* L, aiMaterial* material, aiTextureType type ) {

	lua_newtable ( L );
	
	size_t nTextures = material->GetTextureCount ( type );
	for ( size_t i = 0; i < nTextures; ++i ) {
		lua_pushnumber ( L, ( lua_Number )( i + 1 ));
		MOAIAssimpUtil::PushTexture ( L, material, type, i );
		lua_settable ( L, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushUVChannelArray ( lua_State* L , aiVector3D* textureCoords , size_t numUVVectors, size_t numUVComponents ) {

	lua_newtable ( L );
	for ( int i = 0; i < ( int )numUVVectors; i++ ) {
		MOAIAssimpUtil::PushUVVector ( L, textureCoords [ i ], numUVComponents );
		lua_rawseti ( L , -2 , i + 1 );
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushUVVector ( lua_State* L, aiVector3D uvVector , size_t numUVComponents  ) {

	lua_newtable ( L );

	if ( numUVComponents >= 1 ) {
		lua_pushnumber ( L , uvVector.x );
		lua_rawseti(L , -2 , 1);

		if ( numUVComponents >= 2 ) {
			lua_pushnumber ( L , uvVector.y );
			lua_rawseti(L , -2 , 2);

			if ( numUVComponents >= 3 ) {
				lua_pushnumber ( L , uvVector.z );
				lua_rawseti(L , -2 , 3);
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushVector ( lua_State* L, aiVector3D vector, bool normalize ) {

	if ( normalize ) {
		vector = vector.Normalize ();
	}

	lua_newtable ( L );
	lua_pushnumber( L , vector.x );
	lua_rawseti( L , -2 , 1 );
	lua_pushnumber( L , vector.y );
	lua_rawseti( L , -2 , 2 );
	lua_pushnumber( L , vector.z );
	lua_rawseti( L , -2 , 3 );
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushVectorArray ( lua_State* L, aiVector3D* vectors, size_t nVectors, bool normalize ) {

	lua_newtable ( L );
	for ( int i = 0; i < ( int )nVectors; ++i ) {
		MOAIAssimpUtil::PushVector ( L , vectors [ i ], normalize );
		lua_rawseti ( L , -2 , i + 1 );
	}
}

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushVertexIndex ( lua_State* L, unsigned int* currentVertexFaceArray , aiFace* currentFace ) {

	for ( int i = 0; i < ( int )currentFace->mNumIndices; i++ ) {
		lua_pushnumber( L , currentVertexFaceArray [ i ] + 1 );
		lua_rawseti( L , -2 , i + 1 );
	}
}
