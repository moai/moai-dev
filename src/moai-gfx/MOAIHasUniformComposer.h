// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASUNIFORMCOMPOSER_H
#define	MOAIHASUNIFORMCOMPOSER_H

#include <moai-gfx/MOAIUniformComposerInterface.h>

//================================================================//
// MOAIHasUniformComposer
//================================================================//
// TODO: doxygen
class MOAIHasUniformComposer :
	public virtual MOAIUniformComposerInterface {
protected:

	ZLStrongPtr < MOAIUniformComposer >		mComposer;
	
	//----------------------------------------------------------------//
	static int					_getComposer				( lua_State* L );
	static int					_setComposer				( lua_State* L );
	
	//----------------------------------------------------------------//
	void						MOAILuaObject_RegisterLuaClass					( MOAIComposer& composer, MOAILuaState& state );
	void						MOAILuaObject_RegisterLuaFuncs					( MOAIComposer& composer, MOAILuaState& state );
	MOAIUniformComposer&		MOAIUniformComposerInterface_AffirmComposer		();
	MOAIUniformComposer*		MOAIUniformComposerInterface_GetComposer		();

public:

	//----------------------------------------------------------------//
	bool			HasComposer						();
					MOAIHasUniformComposer			();
					~MOAIHasUniformComposer			();
};

#endif
