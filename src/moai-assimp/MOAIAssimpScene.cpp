// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#include "pch.h"

#include <moai-assimp/MOAIAssimpCamera.h>
#include <moai-assimp/MOAIAssimpMesh.h>
#include <moai-assimp/MOAIAssimpScene.h>
#include <moai-assimp/MOAIAssimpUtil.h>

#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIAnim.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	clear
	@text	Frees Memory and Data from a Scene File

	@in		MOAIAssimpScene self
	@out	nil
*/
int MOAIAssimpScene::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	countAnimations
	@text	Return the number of animations in an scene
	@in		MOAIAssimpScene self

	@out	number of animations or nil
*/
int MOAIAssimpScene::_countAnimations ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	lua_pushinteger ( state , self->mScene ? self->mScene->mNumAnimations : 0 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	countCameras
	@text	Return the number of Cameras in an scene

	@in		MOAIAssimpScene self
	@out	number of cameras or nil
*/
int MOAIAssimpScene::_countCameras ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	lua_pushinteger( state , self->mScene ? self->mScene->mNumCameras : 0 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	countLights
	@text	Return the number of lights in an scene

	@in		MOAIAssimpScene self
	@out	number of lights or nil
*/
int MOAIAssimpScene::_countLights ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	lua_pushinteger( state , self->mScene ? self->mScene->mNumLights : 0 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	countMaterials
	@text	Return the number of materials in an scene

	@in		MOAIAssimpScene self
	@out	number of materials or nil
*/
int MOAIAssimpScene::_countMaterials ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	lua_pushinteger ( state , self->mScene ? self->mScene->mNumMaterials : 0 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	countMeshes
	@text	Return the number of meshes in an scene

	@in		MOAIAssimpScene self
	@out	number of meshes or nil
*/
int MOAIAssimpScene::_countMeshes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	lua_pushinteger ( state, self->mScene ? self->mScene->mNumMeshes : 0 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	countTextures
	@text	Return the number of textures in an scene

	@in		MOAIAssimpScene self
	@out	number of textures or nil
*/
int MOAIAssimpScene::_countTextures ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	lua_pushinteger ( state , self->mScene ? self->mScene->mNumTextures : 0 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpScene::_getCameras ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	if ( !self->mScene ) return 0;

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		state.Push ( self->GetCamera ( state.GetValue < u32 >( 2, 0 )));
	}
	else {

		lua_newtable ( state );
		for ( size_t i = 0; i < self->mScene->mNumCameras; ++i ) {
			state.SetFieldByIndex ( -1, ( int )i + 1, self->GetCamera ( i ));
		}
	}
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpScene::_getMaterials ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	if ( !self->mScene ) return 0;

	if ( state.IsType ( 2, LUA_TNUMBER )) {
	
		MOAILuaMemberRef* material = self->GetMaterial ( state.GetValue < u32 >( 2, 0 ));
		if ( material ) {
			state.Push ( *material );
			return 1;
		}
	}
	else {
	
		lua_newtable ( state );
		for ( size_t i = 0; i < self->mScene->mNumMaterials; ++i ) {

			MOAILuaMemberRef* material = self->GetMaterial ( i );
			assert ( material );
			
			state.SetFieldByIndex ( -1, ( int )i + 1, *material );
		}
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpScene::_getMeshes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	if ( !self->mScene ) return 0;

	if ( state.IsType ( 2, LUA_TNUMBER )) {
		state.Push ( self->GetMesh ( state.GetValue < u32 >( 2, 0 )));
	}
	else {

		lua_newtable ( state );
		for ( size_t i = 0; i < self->mScene->mNumMeshes; ++i ) {
			state.SetFieldByIndex ( -1, ( int )i + 1, self->GetMesh ( i ));
		}
	}
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAssimpScene::_getRootNode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "U" )

	if ( !self->mScene ) return 0;

	if ( self->mScene->mRootNode ) {
		MOAIAssimpUtil::PushNode ( L, self->mScene->mRootNode );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads a 3D scene file. May be passed a filename or an existing
			file to reload with new processing. The preprocessing flags
			may be OR'ed together and passed as a single flag or may be
			passed as a variable argument list.

	@overload
		@in		MOAIAssimpScene self
		@in		string filename
		@opt	...						List of preprocessing flags. Will be OR'ed together.
		@out	nil

	@overload
		@in		MOAIAssimpScene self
		@in		MOAIAssimpScene scene	An existing scene to reload. May be self.
		@opt	...						List of preprocessing flags. Will be OR'ed together.
		@out	nil
*/
int MOAIAssimpScene::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAssimpScene, "US" )

	u32 preprocessingFlags = 0;
	for ( int i = 3; i <= state.GetTop (); ++i ) {
		preprocessingFlags |= state.GetValue < u32 >( i, 0 );
	}

	cc8* filename = state.GetValue < cc8* >( 2, 0 );
	if ( filename ) {
		self->Load ( filename, preprocessingFlags );
	}
	else {
		MOAIAssimpScene* scene = state.GetLuaObject < MOAIAssimpScene >( 2, true );
		if ( scene ) {
			self->Load ( *scene, preprocessingFlags );
		}
	}

	state.Push ( self->mScene != NULL );
	return 1;
}

//================================================================//
// MOAIAssimpScene
//================================================================//

//----------------------------------------------------------------//
void MOAIAssimpScene::Clear () {

	// do this first; LuaRelease may delete scene members
	SceneMembersIt sceneMembersIt = this->mSceneMembers.begin ();
	for ( ; sceneMembersIt != this->mSceneMembers.end (); ++sceneMembersIt ) {
		( *sceneMembersIt )->DetachFromScene ();
	}
	this->mSceneMembers.clear ();

	// now safe to release the scene members
	for ( size_t i = 0; i < this->mCameras.Size (); ++i ) {
		this->LuaRelease ( this->mCameras [ i ]);
	}
	this->mCameras.Clear ();

	for ( size_t i = 0; i < this->mMaterials.Size (); ++i ) {
		this->mMaterials [ i ].Clear ();
	}
	this->mMaterials.Clear ();

	for ( size_t i = 0; i < this->mMeshes.Size (); ++i ) {
		this->LuaRelease ( this->mMeshes [ i ]);
	}
	this->mMeshes.Clear ();

	if ( this->mImporter ) {
		this->mImporter->FreeScene ();
		delete ( this->mImporter );
	}
	this->mImporter = 0;
	this->mScene = 0;
}

//----------------------------------------------------------------//
MOAIAssimpCamera* MOAIAssimpScene::GetCamera ( size_t idx ) {

	if ( !this->mScene ) return 0;
	if ( this->mScene->mNumCameras <= idx ) return 0;
	
	this->mCameras.Resize ( this->mScene->mNumCameras, 0 );
	
	MOAIAssimpCamera* camera = this->mCameras [ idx ];
	if ( !camera ) {
	
		aiCamera* currentCamera = this->mScene->mCameras [ idx ];
	
		camera = new MOAIAssimpCamera ();
		camera->SetNode ( this->mScene->mRootNode->FindNode ( currentCamera->mName.C_Str ()));
		camera->SetCamera ( currentCamera );
		camera->SetIndex (( uint )idx );
		camera->SetName ( currentCamera->mName.C_Str ());
		
		this->LuaRetain ( camera );
		this->mCameras [ idx ] = camera;
		
		this->mSceneMembers.push_back ( camera );
	}
	return camera;
}

//----------------------------------------------------------------//
MOAIAssimpMesh* MOAIAssimpScene::GetMesh ( size_t idx ) {

	if ( !this->mScene ) return 0;
	if ( this->mScene->mNumMeshes <= idx ) return 0;
	
	this->mMeshes.Resize ( this->mScene->mNumMeshes, 0 );
	
	MOAIAssimpMesh* mesh = this->mMeshes [ idx ];
	if ( !mesh ) {
	
		aiMesh* currentMesh = this->mScene->mMeshes [ idx ];

		mesh = new MOAIAssimpMesh ();
		mesh->SetScene ( this->mScene );
		mesh->SetMesh ( currentMesh );
		mesh->SetIndex (( uint )idx );
		mesh->SetName ( currentMesh->mName.C_Str ());

		this->LuaRetain ( mesh );
		this->mMeshes [ idx ] = mesh;
		
		this->mSceneMembers.push_back ( mesh );
	}
	return mesh;
}

//----------------------------------------------------------------//
MOAILuaMemberRef* MOAIAssimpScene::GetMaterial ( size_t idx ) {

	if ( !this->mScene ) return 0;
	if ( this->mScene->mNumMaterials <= idx ) return 0;
	
	if ( this->mMaterials.Size () == 0 ) {
		this->mMaterials.Init ( this->mScene->mNumMaterials );
	}
	assert ( this->mMaterials.Size () == this->mScene->mNumMaterials );
	
	MOAILuaMemberRef& ref = this->mMaterials [ idx ];
	
	if ( ref == false ) {
	
		aiMaterial* currentMaterial = this->mScene->mMaterials [ idx ];
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		
		lua_newtable ( state );
		
		aiString	name;
		aiColor3D	color;
		int			intVal;
		float		floatVal;
		
		currentMaterial->Get ( AI_MATKEY_NAME, name );
		state.SetField ( -1, "NAME", name.C_Str ());
	
		currentMaterial->Get ( AI_MATKEY_TWOSIDED, intVal );
		state.SetField ( -1, "TWOSIDED", intVal == 1 ? true : false );
		
		currentMaterial->Get ( AI_MATKEY_SHADING_MODEL, intVal );
		state.SetField ( -1, "SHADING_MODEL", ( u32 )intVal );
		
		currentMaterial->Get ( AI_MATKEY_ENABLE_WIREFRAME, intVal );
		state.SetField ( -1, "ENABLE_WIREFRAME", intVal == 1 ? true : false );
	
		currentMaterial->Get ( AI_MATKEY_BLEND_FUNC, intVal );
		state.SetField ( -1, "BLEND_MODE", ( u32 )intVal );

		currentMaterial->Get ( AI_MATKEY_OPACITY, floatVal );
		state.SetField ( -1, "OPACITY", floatVal );
		
		currentMaterial->Get ( AI_MATKEY_BUMPSCALING, floatVal );
		state.SetField ( -1, "BUMPSCALING", floatVal );

		currentMaterial->Get ( AI_MATKEY_SHININESS, floatVal );
		state.SetField ( -1, "SHININESS", floatVal );

		currentMaterial->Get ( AI_MATKEY_SHININESS_STRENGTH, floatVal );
		state.SetField ( -1, "SHININESS_STRENGTH", floatVal );

		currentMaterial->Get ( AI_MATKEY_REFLECTIVITY, floatVal );
		state.SetField ( -1, "REFLECTIVITY", floatVal );

		currentMaterial->Get ( AI_MATKEY_REFRACTI, floatVal );
		state.SetField ( -1, "REFRACTION_INDEX", floatVal );

		currentMaterial->Get ( AI_MATKEY_COLOR_DIFFUSE, color );
		MOAIAssimpUtil::PushColor ( state, color );
		lua_setfield ( state, -2, "COLOR_DIFFUSE" );
		
		currentMaterial->Get ( AI_MATKEY_COLOR_AMBIENT, color );
		MOAIAssimpUtil::PushColor ( state, color );
		lua_setfield ( state, -2, "COLOR_AMBIENT" );

		currentMaterial->Get ( AI_MATKEY_COLOR_SPECULAR, color );
		MOAIAssimpUtil::PushColor ( state, color );
		lua_setfield ( state, -2, "COLOR_SPECULAR" );

		currentMaterial->Get ( AI_MATKEY_COLOR_EMISSIVE, color );
		MOAIAssimpUtil::PushColor ( state, color );
		lua_setfield ( state, -2, "COLOR_EMISSIVE" );

		currentMaterial->Get ( AI_MATKEY_COLOR_TRANSPARENT, color );
		MOAIAssimpUtil::PushColor ( state, color );
		lua_setfield ( state, -2, "COLOR_TRANSPARENT" );
		
		currentMaterial->Get ( AI_MATKEY_COLOR_REFLECTIVE, color );
		MOAIAssimpUtil::PushColor ( state, color );
		lua_setfield ( state, -2, "COLOR_REFLECTIVE" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_DIFFUSE );
		lua_setfield ( state, -2, "TEXTURE_DIFFUSE" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_SPECULAR );
		lua_setfield ( state, -2, "TEXTURE_SPECULAR" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_AMBIENT );
		lua_setfield ( state, -2, "TEXTURE_AMBIENT" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_EMISSIVE );
		lua_setfield ( state, -2, "TEXTURE_EMISSIVE" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_HEIGHT );
		lua_setfield ( state, -2, "TEXTURE_HEIGHT" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_NORMALS );
		lua_setfield ( state, -2, "TEXTURE_NORMALS" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_SHININESS );
		lua_setfield ( state, -2, "TEXTURE_SHININESS" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_OPACITY );
		lua_setfield ( state, -2, "TEXTURE_OPACITY" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_DISPLACEMENT );
		lua_setfield ( state, -2, "TEXTURE_DISPLACEMENT" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_LIGHTMAP );
		lua_setfield ( state, -2, "TEXTURE_LIGHTMAP" );
		
		MOAIAssimpUtil::PushTextureStack ( state, currentMaterial, aiTextureType_REFLECTION );
		lua_setfield ( state, -2, "TEXTURE_REFLECTION" );
		
		ref.SetRef ( *this, state, -1 );
		state.Pop ();
	}
	return &ref;
}

//----------------------------------------------------------------//
ZLResultCode MOAIAssimpScene::Load ( cc8* filename, u32 postProcessingFlags ) {

	this->Clear ();

	if ( !filename ) return ZL_ERROR;

	STLString fullpath = ZLFileSys::GetAbsoluteFilePath ( filename );
	this->mImporter = new Assimp::Importer ();

	this->mImporter->ReadFile ( fullpath, postProcessingFlags );
	this->mScene = this->mImporter->GetScene ();

	// If the import failed, report it
	if ( !this->mScene ) {
		printf ( "%s", this->mImporter->GetErrorString ());
		this->Clear ();
		return ZL_ERROR;
	}
	return ZL_OK;
}

//----------------------------------------------------------------//
ZLResultCode MOAIAssimpScene::Load ( const MOAIAssimpScene& scene, u32 postProcessingFlags ) {

	if ( !scene.mImporter ) {
		this->Clear ();
		return ZL_OK;
	}

	Assimp::Importer* prevImporter = this->mImporter;

	this->mImporter = new Assimp::Importer ( *scene.mImporter );
	this->mScene = this->mImporter->ApplyPostProcessing ( postProcessingFlags );

	if ( prevImporter ) {
		prevImporter->FreeScene ();
		delete ( prevImporter );
	}

	if ( !this->mScene ) {
		printf ( "%s", this->mImporter->GetErrorString ());
		this->Clear ();
		return ZL_ERROR;
	}
	return ZL_OK;
}

//----------------------------------------------------------------//
MOAIAssimpScene::MOAIAssimpScene () :
	mScene ( 0 ),
	mImporter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAssimpScene::~MOAIAssimpScene () {
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAssimpScene::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "CONVERT_TO_LEFT_HANDED",			( u32 )aiProcess_ConvertToLeftHanded );
	state.SetField ( -1, "TARGET_REALTIME_FAST",			( u32 )aiProcessPreset_TargetRealtime_Fast );
	state.SetField ( -1, "TARGET_REALTIME_QUALITY",			( u32 )aiProcessPreset_TargetRealtime_MaxQuality );
	state.SetField ( -1, "TARGET_REALTIME_MAX_QUALITY",		( u32 )aiProcessPreset_TargetRealtime_Quality );
	state.SetField ( -1, "CALCULATE_TANGENT_SPACE",			( u32 )aiProcess_CalcTangentSpace );
	state.SetField ( -1, "JOINT_IDENTICAL_VERTICES",		( u32 )aiProcess_JoinIdenticalVertices );
	state.SetField ( -1, "MAKE_LEFT_HANDED",				( u32 )aiProcess_MakeLeftHanded );
	state.SetField ( -1, "TRIANGULATE",						( u32 )aiProcess_Triangulate );
	state.SetField ( -1, "REMOVE_COMPONENT",				( u32 )aiProcess_RemoveComponent );
	state.SetField ( -1, "GENERATE_NORMALS",				( u32 )aiProcess_GenNormals );
	state.SetField ( -1, "GENERATE_SMOOTH_NORMALS",			( u32 )aiProcess_GenSmoothNormals );
	state.SetField ( -1, "SPLIT_LARGE_MESHES",				( u32 )aiProcess_SplitLargeMeshes );
	state.SetField ( -1, "PRE_TRANSFORM_VERTICES",			( u32 )aiProcess_PreTransformVertices );
	state.SetField ( -1, "LIMIT_BONE_WEIGHTS",				( u32 )aiProcess_LimitBoneWeights );
	state.SetField ( -1, "VALIDATE_DATA_STRUCTURE",			( u32 )aiProcess_ValidateDataStructure );
	state.SetField ( -1, "IMPROVE_CACHE_LOCALITY",			( u32 )aiProcess_ImproveCacheLocality );
	state.SetField ( -1, "REMOVE_REDUNDANT_MATERIALS",		( u32 )aiProcess_RemoveRedundantMaterials );
	state.SetField ( -1, "FIX_INFANCING_NORMALS",			( u32 )aiProcess_FixInfacingNormals );
	state.SetField ( -1, "SORT_BY_POINT_TYPE",				( u32 )aiProcess_SortByPType );
	state.SetField ( -1, "FIND_DEGENERATES",				( u32 )aiProcess_FindDegenerates );
	state.SetField ( -1, "FIND_INVALID_DATA",				( u32 )aiProcess_FindInvalidData );
	state.SetField ( -1, "GENERATE_UV_COORDINATES",			( u32 )aiProcess_GenUVCoords );
	state.SetField ( -1, "TRANSFORM_UV_COORDINATES",		( u32 )aiProcess_TransformUVCoords );
	state.SetField ( -1, "FIND_INSTANCES",					( u32 )aiProcess_FindInstances );
	state.SetField ( -1, "OPTIMIZE_MESHES",					( u32 )aiProcess_OptimizeMeshes );
	state.SetField ( -1, "OPTIMIZE_GRAPH",					( u32 )aiProcess_OptimizeGraph );
	state.SetField ( -1, "FLIP_UVS",						( u32 )aiProcess_FlipUVs );
	state.SetField ( -1, "FLIP_WINDING_ORDER",				( u32 )aiProcess_FlipWindingOrder );
	state.SetField ( -1, "SPLIT_BONE_BY_COUNT",				( u32 )aiProcess_SplitByBoneCount );
	state.SetField ( -1, "DEBONE",							( u32 )aiProcess_Debone );
}

//----------------------------------------------------------------//
void MOAIAssimpScene::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "countAnimations",		_countAnimations },
		{ "countCameras",			_countCameras },
		{ "countLights",			_countLights },
		{ "countMaterials",			_countMaterials },
		{ "countMeshes",			_countMeshes },
		{ "countTextures",			_countTextures },
		{ "getCameras",				_getCameras },
		{ "getMaterials",			_getMaterials },
		{ "getMeshes",				_getMeshes },
		{ "getRootNode",			_getRootNode },
		{ "load",					_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
