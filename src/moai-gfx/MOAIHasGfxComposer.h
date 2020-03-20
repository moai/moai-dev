// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXCOMPOSER_H
#define	MOAIHASGFXCOMPOSER_H

#include <moai-gfx/MOAIGfxComposerInterface.h>

//================================================================//
// MOAIHasGfxComposer
//================================================================//
// TODO: doxygen
class MOAIHasGfxComposer :
	public virtual MOAIGfxComposerInterface {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLStrongPtr < MOAIGfxComposer >		mComposer;
	
	//----------------------------------------------------------------//
	static int				_getComposer				( lua_State* L );
	static int				_setComposer				( lua_State* L );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass								( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs								( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIGfxComposer&		MOAIGfxComposerInterface_AffirmComposer			();
	MOAIGfxComposer*		MOAIGfxComposerInterface_GetComposer			();

public:

	//----------------------------------------------------------------//
	bool					HasComposer					();
							MOAIHasGfxComposer			();
							~MOAIHasGfxComposer			();
};

#endif
