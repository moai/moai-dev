// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIALINTERFACE_H
#define	MOAIMATERIALINTERFACE_H

#include <moai-gfx/MOAIGfxMgr.h>

class MOAIAbstractMaterialGlobalsContext;
class MOAIImage;
class MOAILight;
class MOAIMaterial;
class MOAIShader;
class MOAITexture;

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
	void									Clear					( u32 flags, bool force = false );
	MOAIAbstractMaterialGlobalsContext&		AffirmGlobalsContext	();

	//----------------------------------------------------------------//
	virtual MOAIMaterial&		MOAIMaterialInterface_AffirmMaterial	() = 0;
	virtual MOAIMaterial*		MOAIMaterialInterface_GetMaterial		() = 0;
	
	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	
public:

	//----------------------------------------------------------------//
	MOAIMaterial&		AffirmMaterial				();
	void				Clear						();
	void				Compose 					( MOAIMaterial& source );
	
	MOAIBlendMode		GetBlendMode				();
	int					GetCullMode					();
	int					GetDepthTest				();
	bool				GetDepthMask				();
	
	MOAILight*			GetLight					( u32 name );
	MOAIMaterial*		GetMaterial					();
	MOAIShader*			GetShader					();
	MOAITexture*		GetTexture					();
	MOAITexture*		GetTexture					( u32 name );
	void				LoadGfxState				();
						MOAIMaterialInterface		();
	virtual				~MOAIMaterialInterface		();
	void				SetBlendMode				();
	void				SetBlendMode				( const MOAIBlendMode& blendMode );
	void				SetCullMode					();
	void				SetCullMode					( int cullMode );
	void				SetDepthMask				();
	void				SetDepthMask				( bool depthMask );
	void				SetDepthTest				();
	void				SetDepthTest				( int depthTest );
	void				SetLight					( u32 name );
	void				SetLight					( u32 name, MOAILight* light );
	void				SetShader					();
	void				SetShader					( MOAIShaderPresetEnum shaderID );
	void				SetShader					( MOAIShader* shader );
	void				SetTexture					();
	void				SetTexture					( MOAITexture* texture );
	void				SetTexture					( u32 name );
	void				SetTexture					( u32 name, MOAITexture* texture );
};

#endif
