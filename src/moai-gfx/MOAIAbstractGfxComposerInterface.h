// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXCOMPOSERINTERFACE_H
#define	MOAIABSTRACTGFXCOMPOSERINTERFACE_H

#include <moai-gfx/MOAIGfxComposerCommand.h>

class MOAIAbstractGfxComposerCallable;
class MOAIAbstractGfxComposer;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIAbstractGfxComposerInterface
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxComposerInterface :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int					_call							( lua_State* L );
	static int					_callFromShader					( lua_State* L );
	static int					_setShader						( lua_State* L );
	static int					_setTexture						( lua_State* L );
 	static int					_setUniform						( lua_State* L );

	//----------------------------------------------------------------//
	void						RetainObject					( ZLRefCountedObject* object );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );

	//----------------------------------------------------------------//
	virtual MOAIAbstractGfxComposer&	MOAIAbstractGfxComposerInterface_AffirmComposer		() = 0;
	virtual MOAIAbstractGfxComposer*	MOAIAbstractGfxComposerInterface_GetComposer		() = 0;

public:

	//----------------------------------------------------------------//
	MOAIAbstractGfxComposer&		AffirmComposer						();
	void							Call								();
	void							CallFromShader						();
	MOAIAbstractGfxComposer*		GetComposer							();
									MOAIAbstractGfxComposerInterface	();
									~MOAIAbstractGfxComposerInterface	();
	void							Optimize							();
	void							SetShader							( MOAIShader* shader );
	void							SetTexture							( MOAITexture* texture, ZLIndex textureUnit );
	void							SetUniform							( ZLIndex globalID, ZLIndex uniformID, ZLIndex index );
};

#endif
