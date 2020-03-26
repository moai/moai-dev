// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTHASGFXSCRIPT_H
#define	MOAIABSTRACTHASGFXSCRIPT_H

#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIDrawingCommand.h>

class MOAIAbstractGfxScript;
class MOAIBlendMode;
class MOAIGfxScriptRetained;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIAbstractHasGfxScript
//================================================================//
// TODO: doxygen
class MOAIAbstractHasGfxScript :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int					_getGfxScript				( lua_State* L );
	static int					_setBlendMode				( lua_State* L );
	static int					_setCullFunc				( lua_State* L );
	static int					_setDepthFunc				( lua_State* L );
	static int					_setDepthMask				( lua_State* L );
	static int					_setGfxScript				( lua_State* L );
	static int					_setShader					( lua_State* L );
	static int					_setTexture					( lua_State* L );
 	static int					_setUniform					( lua_State* L );
	
	//----------------------------------------------------------------//
	void								_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void								_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual MOAIGfxScriptRetained&		MOAIAbstractHasGfxScript_AffirmGfxScript	() = 0;
	virtual MOAIAbstractGfxScript*		MOAIAbstractHasGfxScript_GetGfxScript		() = 0;
	virtual void						MOAIAbstractHasGfxScript_SetGfxScript		( MOAIAbstractGfxScript* gfxScript ) = 0;

public:

	//----------------------------------------------------------------//
	MOAIGfxScriptRetained&		AffirmGfxScript				();
	MOAIAbstractGfxScript*		GetGfxScript				();
								MOAIAbstractHasGfxScript	();
								~MOAIAbstractHasGfxScript	();
	void						SetBlendMode				( const MOAIBlendMode& blendMode );
	void						SetCullFunc					( MOAICullFuncEnum::_ cullFunc );
	void						SetDepthFunc				( MOAIDepthFuncEnum::_ depthFunc );
	void						SetDepthMask				( bool depthMask );
	void						SetGfxScript				( MOAIAbstractGfxScript* gfxScript );
	void						SetShader					( MOAIShader* shader );
	void						SetTexture					( MOAITexture* texture, ZLIndex textureUnit );
	void						SetUniform					( ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
};

#endif
