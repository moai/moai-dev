// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#ifndef MOAIASSIMPUTIL_H
#define MOAIASSIMPUTIL_H

//================================================================//
// MOAIAssimpMesh
//================================================================//
// TODO: doxygen
namespace MOAIAssimpUtil {

	//----------------------------------------------------------------//
	cc8*			EnumToString_aiTextureMapMode		( aiTextureMapMode mode );
	cc8*			EnumToString_aiTextureMapping		( aiTextureMapping mapping );
	cc8*			EnumToString_aiTextureOp			( aiTextureOp op );
	void			PushBitangentsArray					( lua_State* L, const aiMesh* currentMesh );
	void			PushBonesArray						( lua_State* L, const aiMesh* currentMesh );
	void			PushColor							( lua_State* L, aiColor3D colorVector );
	void			PushColorsChannelArray				( lua_State* L, aiColor4D* colorCoords, size_t numColorVectors );
	void			PushColorVector						( lua_State* L, aiColor4D colorVector );
	void			PushFace							( lua_State* L, const aiMesh* currentMesh );
	void			PushMatrix							( lua_State* L, const aiMatrix4x4& matrix );
	void			PushMesh							( lua_State* L, const aiMesh* currentMesh );
	void			PushNode							( lua_State* L, const aiNode* node );
	void			PushNormalsArray					( lua_State* L, const aiMesh* currentMesh );
	void			PushTexture							( lua_State* L, aiMaterial* material, aiTextureType type, size_t idx );
	void			PushTextureStack					( lua_State* L, aiMaterial* material, aiTextureType type );
	void			PushTangentsArray					( lua_State* L, const aiMesh* currentMesh );
	void			PushUVChannelArray					( lua_State* L, aiVector3D* textureCoords, size_t numUVVectors, size_t numUVComponents );
	void			PushUVVector						( lua_State* L, aiVector3D uvVector, size_t numUVComponents );
	void			PushVector							( lua_State* L, aiVector3D vector, bool normalize = false );
	void			PushVectorArray						( lua_State* L, aiVector3D* vectors, size_t nVectors, bool normalize = false );
	void			PushVertexIndex						( lua_State* L, unsigned int* currentVertexFaceArray , aiFace* currentFace );
}

#endif
