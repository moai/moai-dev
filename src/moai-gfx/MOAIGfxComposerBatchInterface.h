// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCOMPOSERBATCHINTERFACE_H
#define	MOAIGFXCOMPOSERBATCHINTERFACE_H

#include <moai-gfx/MOAIGfxComposerInterface.h>

class MOAIGfxComposerBatch;

//================================================================//
// MOAIGfxComposerBatchInterface
//================================================================//
// TODO: doxygen
class MOAIGfxComposerBatchInterface :
	public virtual MOAIGfxComposerInterface {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int			_affirmComposer				( lua_State* L );
	static int			_getIndexBatchSize			( lua_State* L );
	static int			_getComposer				( lua_State* L );
	static int			_reserveComposers			( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );

	//----------------------------------------------------------------//
	virtual MOAIGfxComposer&		MOAIGfxComposerBatchInterface_AffirmComposer			( ZLIndex index ) = 0;
	virtual MOAIGfxComposerBatch&	MOAIGfxComposerBatchInterface_AffirmComposerBatch		() = 0;
	virtual MOAIGfxComposer*		MOAIGfxComposerBatchInterface_GetComposer				( ZLIndex index ) = 0;
	virtual MOAIGfxComposerBatch*	MOAIGfxComposerBatchInterface_GetComposerBatch			() = 0;
	
	//----------------------------------------------------------------//
	void							_RegisterLuaClass								( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs								( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIGfxComposer&				MOAIGfxComposerInterface_AffirmComposer			();
	MOAIGfxComposer*				MOAIGfxComposerInterface_GetComposer			();
	
public:

	//----------------------------------------------------------------//
	MOAIGfxComposer&				AffirmComposer									();
	MOAIGfxComposer&				AffirmComposer									( ZLIndex index );
	MOAIGfxComposerBatch&			AffirmComposerBatch								();
	MOAIGfxComposer*				GetComposer										();
	MOAIGfxComposer*				GetComposer										( ZLIndex index );
	MOAIGfxComposerBatch*			GetComposerBatch								();
									MOAIGfxComposerBatchInterface					();
	virtual							~MOAIGfxComposerBatchInterface					();
};

#endif
