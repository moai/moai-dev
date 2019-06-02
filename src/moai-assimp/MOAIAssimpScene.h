// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#ifndef MOAIASSIMPSCENE_H
#define MOAIASSIMPSCENE_H

class MOAIAssimpAnimation;
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

	ZLLeanArray < MOAIAssimpAnimation* >	mAnimations;
	ZLLeanArray < MOAIAssimpCamera* >		mCameras;
	ZLLeanArray < MOAILuaMemberRef >		mMaterials;
	ZLLeanArray < MOAIAssimpMesh* >			mMeshes;
	
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
	static int			_getAnimations				( lua_State* L );
	static int			_getCameras					( lua_State* L );
	static int			_getMaterials				( lua_State* L );
	static int			_getMeshes					( lua_State* L );
	static int			_getRootNode				( lua_State* L );
	static int			_load						( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIAssimpScene )

	//----------------------------------------------------------------//
	void					Clear					();
	MOAIAssimpAnimation*	GetAnimation			( ZLIndex idx );
	MOAIAssimpCamera*		GetCamera				( ZLIndex idx );
	MOAILuaMemberRef*		GetMaterial				( ZLIndex idx );
	MOAIAssimpMesh*			GetMesh					( ZLIndex idx );
	ZLResultCode			Load					( cc8* filename, u32 postProcessingFlags = 0 );
	ZLResultCode			Load					( const MOAIAssimpScene& scene, u32 postProcessingFlags = 0 );
							MOAIAssimpScene			();
							~MOAIAssimpScene		();
};

#endif
