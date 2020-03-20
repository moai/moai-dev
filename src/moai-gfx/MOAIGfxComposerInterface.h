// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCOMPOSERINTERFACEGL_H
#define	MOAIGFXCOMPOSERINTERFACEGL_H

#include <moai-gfx/MOAIGfxComposerCmd.h>

class MOAIGfxComposer;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIGfxComposerInterface
//================================================================//
// TODO: doxygen
class MOAIGfxComposerInterface :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	// FROM: resource object; material - to: material; gfx state

	//----------------------------------------------------------------//
	static int					_pushCallCmd				( lua_State* L );
	static int					_pushCallFromShaderCmd		( lua_State* L );
	static int					_pushShaderCmd				( lua_State* L );
	static int					_pushTextureCmd				( lua_State* L );
 	static int					_pushUniformCmd				( lua_State* L );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

	//----------------------------------------------------------------//
	virtual MOAIGfxComposer&	MOAIGfxComposerInterface_AffirmComposer		() = 0;
	virtual MOAIGfxComposer*	MOAIGfxComposerInterface_GetComposer		() = 0;

public:

	//----------------------------------------------------------------//
	MOAIGfxComposer&			AffirmComposer						();
	MOAIGfxComposer*			GetComposer							();
								MOAIGfxComposerInterface			();
								~MOAIGfxComposerInterface			();
	void						PushCallCmd							();
	void						PushCallFromShaderCmd				();
	void						PushShaderCmd						( MOAIGfxComposerAddrModeEnum::_ from, MOAIGfxComposerAddrModeEnum::_ to, MOAIShader* shader );
	void						PushTextureCmd						( MOAIGfxComposerAddrModeEnum::_ from, MOAIGfxComposerAddrModeEnum::_ to, MOAITexture* texture, ZLIndex textureUnit );
	void						PushUniformCmd						( MOAIGfxComposerAddrModeEnum::_ from, MOAIGfxComposerAddrModeEnum::_ to, ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
};

#endif
