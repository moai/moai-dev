// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXCOMPOSERBATCHINTERFACE_H
#define	MOAIABSTRACTGFXCOMPOSERBATCHINTERFACE_H

#include <moai-gfx/MOAIAbstractGfxComposerInterface.h>

class MOAIGfxComposerBatch;

//================================================================//
// MOAIAbstractGfxComposerBatchInterface
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxComposerBatchInterface :
	public virtual MOAIAbstractGfxComposerInterface {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int			_affirmComposer				( lua_State* L );
	static int			_getIndexBatchSize			( lua_State* L );
	static int			_getComposer				( lua_State* L );
	static int			_reserveComposers			( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );
	
	//----------------------------------------------------------------//
	void							_RegisterLuaClass									( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs									( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxComposer&		MOAIAbstractGfxComposerInterface_AffirmComposer		();
	MOAIAbstractGfxComposer*		MOAIAbstractGfxComposerInterface_GetComposer		();
	
	//----------------------------------------------------------------//
	virtual MOAIAbstractGfxComposer&	MOAIAbstractGfxComposerBatchInterface_AffirmComposer		( ZLIndex index ) = 0;
	virtual MOAIGfxComposerBatch&		MOAIAbstractGfxComposerBatchInterface_AffirmComposerBatch	() = 0;
	virtual MOAIAbstractGfxComposer*	MOAIAbstractGfxComposerBatchInterface_GetComposer			( ZLIndex index ) = 0;
	virtual MOAIGfxComposerBatch*		MOAIAbstractGfxComposerBatchInterface_GetComposerBatch		() = 0;
	
public:

	//----------------------------------------------------------------//
	MOAIAbstractGfxComposer&		AffirmComposer									();
	MOAIAbstractGfxComposer&		AffirmComposer									( ZLIndex index );
	MOAIGfxComposerBatch&			AffirmComposerBatch								();
	MOAIAbstractGfxComposer*		GetComposer										();
	MOAIAbstractGfxComposer*		GetComposer										( ZLIndex index );
	MOAIGfxComposerBatch*			GetComposerBatch								();
									MOAIAbstractGfxComposerBatchInterface			();
	virtual							~MOAIAbstractGfxComposerBatchInterface			();
};

#endif
