// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

// Adapted from moai-assimp by Leandro Pezzente
// https://bitbucket.org/The-Mad-Pirate/moai-assimp

#ifndef MOAIASSIMPSCENEMEMBER_H
#define MOAIASSIMPSCENEMEMBER_H

class MOAIAssimpScene;

//================================================================//
// MOAIAssimpSceneMember
//================================================================//
// TODO: doxygen
class MOAIAssimpSceneMember :
	public virtual MOAILuaObject {
protected:

	friend class MOAIAssimpScene;
	
	STLString			mName;
	aiNode*				mNode;
	const aiScene*		mScene;
	uint				mIndex;
	
	SET ( STLString, Name, mName )
	SET ( aiNode*, Node, mNode )
	SET ( const aiScene*, Scene, mScene )
	SET ( uint, Index, mIndex )
	
	//----------------------------------------------------------------//
	static int			_getIndex					( lua_State* L );
	static int			_getName					( lua_State* L );
	
	//----------------------------------------------------------------//
	void				DetachFromScene				();

public:
	
	//----------------------------------------------------------------//
						MOAIAssimpSceneMember		();
						~MOAIAssimpSceneMember		();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
