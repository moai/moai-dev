// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-assimp/MOAIAssimpUtil.h>

//================================================================//
// MOAIAssimpUtil
//================================================================//

//----------------------------------------------------------------//
void MOAIAssimpUtil::PushBitangentsArray( lua_State* L , const aiMesh* currentMesh ) {

	MOAIAssimpUtil::PushVectorArray ( L, currentMesh->mBitangents, currentMesh->mNumVertices, true );
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
void MOAIAssimpUtil::PushMesh ( lua_State* L , const aiMesh* currentMesh ) {

	MOAIAssimpUtil::PushVectorArray ( L, currentMesh->mVertices, currentMesh->mNumVertices, false );
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
