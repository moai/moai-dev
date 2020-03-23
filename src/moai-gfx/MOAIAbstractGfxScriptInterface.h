// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPTINTERFACE_H
#define	MOAIABSTRACTGFXSCRIPTINTERFACE_H

#include <moai-gfx/MOAIGfxScriptCommand.h>

class MOAIAbstractGfxScriptCallable;
class MOAIAbstractGfxScript;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIAbstractGfxScriptInterface
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScriptInterface :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int					_call								( lua_State* L );
	static int					_callFromShader						( lua_State* L );
	static int					_setShader							( lua_State* L );
	static int					_setTexture							( lua_State* L );
 	static int					_setUniform							( lua_State* L );

	//----------------------------------------------------------------//
	void						RetainObject						( ZLRefCountedObject* object );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );

	//----------------------------------------------------------------//
	virtual MOAIAbstractGfxScript&	MOAIAbstractGfxScriptInterface_AffirmComposer		() = 0;
	virtual MOAIAbstractGfxScript*	MOAIAbstractGfxScriptInterface_GetComposer			() = 0;

public:

	//----------------------------------------------------------------//
	MOAIAbstractGfxScript&		AffirmComposer						();
	void						Call								();
	void						CallFromShader						();
	MOAIAbstractGfxScript*		GetComposer							();
								MOAIAbstractGfxScriptInterface		();
								~MOAIAbstractGfxScriptInterface		();
	void						Optimize							();
	void						SetShader							( MOAIShader* shader );
	void						SetTexture							( MOAITexture* texture, ZLIndex textureUnit );
	void						SetUniform							( ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
};

#endif
