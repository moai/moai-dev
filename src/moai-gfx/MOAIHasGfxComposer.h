// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXCOMPOSER_H
#define	MOAIHASGFXCOMPOSER_H

#include <moai-gfx/MOAIAbstractGfxComposerInterface.h>

//================================================================//
// MOAIHasGfxComposer
//================================================================//
// TODO: doxygen
class MOAIHasGfxComposer :
	public virtual MOAIAbstractGfxComposerInterface {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLStrongPtr < MOAIAbstractGfxComposer > mComposer;
	
	//----------------------------------------------------------------//
	static int				_getComposer				( lua_State* L );
	static int				_setComposer				( lua_State* L );
	
	//----------------------------------------------------------------//
	void						_RegisterLuaClass									( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs									( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxComposer&	MOAIAbstractGfxComposerInterface_AffirmComposer		();
	MOAIAbstractGfxComposer*	MOAIAbstractGfxComposerInterface_GetComposer		();

public:

	//----------------------------------------------------------------//
	bool					HasComposer					();
							MOAIHasGfxComposer			();
							~MOAIHasGfxComposer			();
};

#endif
