// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNIFORMCOMPOSERINTERFACEGL_H
#define	MOAIUNIFORMCOMPOSERINTERFACEGL_H

class MOAIAbstractUniformBuffer;
class MOAIUniformComposer;
class MOAIUniformSchema;

//================================================================//
// MOAIUniformComposerInterface
//================================================================//
// TODO: doxygen
class MOAIUniformComposerInterface :
	public virtual MOAILuaObject {
protected:

	//----------------------------------------------------------------//
	static int					_reserveGlobals				( lua_State* L );
	static int					_reserveTextures			( lua_State* L );
	static int					_setGlobal					( lua_State* L );
	static int					_setTexture					( lua_State* L );

	//----------------------------------------------------------------//
	void						MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );

	//----------------------------------------------------------------//
	virtual MOAIUniformComposer&		MOAIUniformComposerInterface_AffirmComposer		() = 0;
	virtual MOAIUniformComposer*		MOAIUniformComposerInterface_GetComposer		() = 0;

public:

	//----------------------------------------------------------------//
	MOAIUniformComposer&		AffirmComposer						();
	MOAIUniformComposer*		GetComposer							();
								MOAIUniformComposerInterface		();
								~MOAIUniformComposerInterface		();
	void						ReserveGlobals						( ZLSize count );
	void						ReserveTextures						( ZLSize count );
	void						SetGlobal							( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index );
	void						SetTexture							( ZLIndex idx, u32 name, ZLIndex unit, MOAITexture* fallback );
	void						SetTexture							( ZLIndex idx, MOAITexture* texture, ZLIndex unit );
};

#endif
