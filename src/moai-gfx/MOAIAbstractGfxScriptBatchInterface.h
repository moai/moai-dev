// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPTBATCHINTERFACE_H
#define	MOAIABSTRACTGFXSCRIPTBATCHINTERFACE_H

#include <moai-gfx/MOAIAbstractGfxScriptInterface.h>

class MOAIGfxScriptBatch;

//================================================================//
// MOAIAbstractGfxScriptBatchInterface
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScriptBatchInterface :
	public virtual MOAIAbstractGfxScriptInterface {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int			_affirmComposer				( lua_State* L );
	static int			_getIndexBatchSize			( lua_State* L );
	static int			_getComposer				( lua_State* L );
	static int			_reserveComposers			( lua_State* L );
	static int			_setIndexBatchSize			( lua_State* L );
	
	//----------------------------------------------------------------//
	void							_RegisterLuaClass								( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs								( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxScript&			MOAIAbstractGfxScriptInterface_AffirmComposer	();
	MOAIAbstractGfxScript*			MOAIAbstractGfxScriptInterface_GetComposer		();
	
	//----------------------------------------------------------------//
	virtual MOAIAbstractGfxScript&	MOAIAbstractGfxScriptBatchInterface_AffirmComposer		( ZLIndex index ) = 0;
	virtual MOAIGfxScriptBatch&		MOAIAbstractGfxScriptBatchInterface_AffirmComposerBatch	() = 0;
	virtual MOAIAbstractGfxScript*	MOAIAbstractGfxScriptBatchInterface_GetComposer			( ZLIndex index ) = 0;
	virtual MOAIGfxScriptBatch*		MOAIAbstractGfxScriptBatchInterface_GetComposerBatch	() = 0;
	
public:

	//----------------------------------------------------------------//
	MOAIAbstractGfxScript&			AffirmComposer									();
	MOAIAbstractGfxScript&			AffirmComposer									( ZLIndex index );
	MOAIGfxScriptBatch&				AffirmComposerBatch								();
	MOAIAbstractGfxScript*			GetComposer										();
	MOAIAbstractGfxScript*			GetComposer										( ZLIndex index );
	MOAIGfxScriptBatch*				GetComposerBatch								();
									MOAIAbstractGfxScriptBatchInterface				();
	virtual							~MOAIAbstractGfxScriptBatchInterface			();
};

#endif
