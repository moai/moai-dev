// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxScriptBatch.h>
#include <moai-gfx/MOAIHasGfxScriptBatch.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatch::_affirmComposerBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	state.Push ( &self->AffirmComposerBatch ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getComposerBatch
	@text	Return the material batch attached to the prop.
	
	@in		MOAIHasGfxScriptBatch self
	@out	MOAIGfxScriptBatch materialBatch
*/
int MOAIHasGfxScriptBatch::_getComposerBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	if ( self->mComposerBatch ) {
		self->mComposerBatch.PushRef ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setComposerBatch
	@text	Sets the prop's material batch.
	
	@in		MOAIHasGfxScriptBatch self
	@opt	MOAIGfxScriptBatch materialBatch
	@out	nil
*/
int MOAIHasGfxScriptBatch::_setComposerBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	self->mComposerBatch.Set ( *self, state.GetLuaObject < MOAIGfxScriptBatch >( 2, true ));
	
	return 0;
}

//================================================================//
// MOAIHasGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHasGfxScriptBatch::MOAIHasGfxScriptBatch () {

	RTTI_BEGIN ( MOAIHasGfxScriptBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxScriptBatch >)
		RTTI_EXTEND ( MOAIAbstractGfxScriptBatchInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScriptBatch::~MOAIHasGfxScriptBatch () {

	this->mComposerBatch.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "affirmComposerBatch",	_affirmComposerBatch },
		{ "getComposerBatch",		_getComposerBatch },
		{ "setComposerBatch",		_setComposerBatch },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatchInterface_AffirmComposer ( ZLIndex index ) {

	return this->AffirmComposerBatch ().AffirmComposer ( index );
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch& MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatchInterface_AffirmComposerBatch () {

	if ( !this->mComposerBatch ) {
		this->mComposerBatch.Set ( *this, new MOAIGfxScriptBatch ());
	}
	return *this->mComposerBatch;
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatchInterface_GetComposer ( ZLIndex index ) {

	return this->mComposerBatch ? this->mComposerBatch->GetComposer ( index ) : NULL;
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch* MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatchInterface_GetComposerBatch () {

	return this->mComposerBatch;
}
