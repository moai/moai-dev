// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALINTERFACE_H
#define	MOAIMATERIALINTERFACE_H

class MOAIImage;
class MOAIMaterial;
class MOAIShaderGL;
class MOAITextureBaseGL;

//================================================================//
// MOAIMaterialInterface
//================================================================//
// TODO: doxygen
class MOAIMaterialInterface :
	public virtual MOAILuaObject {
protected:
	
	//----------------------------------------------------------------//
	static int			_getBlendMode				( lua_State* L );
	static int			_getCullMode				( lua_State* L );
	static int			_getDepthMask				( lua_State* L );
	static int			_getDepthTest				( lua_State* L );
	static int			_getLight					( lua_State* L );
	static int			_getShader					( lua_State* L );
	static int			_getTexture					( lua_State* L );
	static int			_setBlendMode				( lua_State* L );
	static int			_setCullMode				( lua_State* L );
	static int			_setDepthMask				( lua_State* L );
	static int			_setDepthTest				( lua_State* L );
	static int			_setLight					( lua_State* L );
	static int			_setShader					( lua_State* L );
	static int			_setTexture					( lua_State* L );

	//----------------------------------------------------------------//
	virtual MOAIMaterial&		MOAIMaterialInterface_AffirmMaterial	() = 0;
	virtual MOAIMaterial*		MOAIMaterialInterface_GetMaterial		() = 0;
	
	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	
public:

	//----------------------------------------------------------------//
	MOAIMaterial&		AffirmMaterial					();
	MOAIMaterial*		GetMaterial						();
						MOAIMaterialInterface			();
	virtual				~MOAIMaterialInterface			();
};

#endif
