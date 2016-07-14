// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#ifndef MOAIASSIMPSCENE_H
#define MOAIASSIMPSCENE_H

class MOAIAssimpCamera;
class MOAIAssimpMesh;
class MOAIAssimpSceneMember;

//================================================================//
// MOAIAssimpScene
//================================================================//
// TODO: doxygen
class MOAIAssimpScene :
	public virtual MOAILuaObject {
private:

	const aiScene*		mScene;
	Assimp::Importer*	mImporter;

	ZLLeanArray < MOAILuaMemberRef >	mMaterials;
	ZLLeanArray < MOAIAssimpCamera* >	mCameras;
	ZLLeanArray < MOAIAssimpMesh* >		mMeshes;
	
	typedef STLList < MOAIAssimpSceneMember* >::iterator	SceneMembersIt;
	STLList < MOAIAssimpSceneMember* >						mSceneMembers;

	//----------------------------------------------------------------//
	static int			_clear						( lua_State* L );
	static int			_countAnimations			( lua_State* L );
	static int			_countCameras				( lua_State* L );
	static int			_countLights				( lua_State* L );
	static int			_countMaterials				( lua_State* L );
	static int			_countMeshes				( lua_State* L );
	static int			_countTextures				( lua_State* L );
	static int			_getCameras					( lua_State* L );
	static int			_getMaterials				( lua_State* L );
	static int			_getMeshes					( lua_State* L );
	static int			_getRootNode				( lua_State* L );
	static int			_load						( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIAssimpScene )

	//----------------------------------------------------------------//
	void					Clear					();
	MOAIAssimpCamera*		GetCamera				( size_t idx );
	MOAILuaMemberRef*		GetMaterial				( size_t idx );
	MOAIAssimpMesh*			GetMesh					( size_t idx );
	ZLResultCode			Load					( cc8* filename, u32 postProcessingFlags = 0 );
	ZLResultCode			Load					( const MOAIAssimpScene& scene, u32 postProcessingFlags = 0 );
							MOAIAssimpScene			();
							~MOAIAssimpScene		();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
