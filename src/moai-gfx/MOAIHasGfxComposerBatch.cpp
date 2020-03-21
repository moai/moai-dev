// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxComposerBatch.h>
#include <moai-gfx/MOAIHasGfxComposerBatch.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxComposerBatch::_affirmComposerBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxComposerBatch, "U" )
	
	state.Push ( &self->AffirmComposerBatch ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getComposerBatch
	@text	Return the material batch attached to the prop.
	
	@in		MOAIHasGfxComposerBatch self
	@out	MOAIGfxComposerBatch materialBatch
*/
int MOAIHasGfxComposerBatch::_getComposerBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxComposerBatch, "U" )
	
	if ( self->mComposerBatch ) {
		self->mComposerBatch.PushRef ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setComposerBatch
	@text	Sets the prop's material batch.
	
	@in		MOAIHasGfxComposerBatch self
	@opt	MOAIGfxComposerBatch materialBatch
	@out	nil
*/
int MOAIHasGfxComposerBatch::_setComposerBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxComposerBatch, "U" )
	
	self->mComposerBatch.Set ( *self, state.GetLuaObject < MOAIGfxComposerBatch >( 2, true ));
	
	return 0;
}

//================================================================//
// MOAIHasGfxComposerBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHasGfxComposerBatch::MOAIHasGfxComposerBatch () {

	RTTI_BEGIN ( MOAIHasGfxComposerBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxComposerBatch >)
		RTTI_EXTEND ( MOAIGfxComposerBatchInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxComposerBatch::~MOAIHasGfxComposerBatch () {

	this->mComposerBatch.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasGfxComposerBatch::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHasGfxComposerBatch::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
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
MOAIGfxComposer& MOAIHasGfxComposerBatch::MOAIGfxComposerBatchInterface_AffirmComposer ( ZLIndex index ) {

	return this->AffirmComposerBatch ().AffirmComposer ( index );
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch& MOAIHasGfxComposerBatch::MOAIGfxComposerBatchInterface_AffirmComposerBatch () {

	if ( !this->mComposerBatch ) {
		this->mComposerBatch.Set ( *this, new MOAIGfxComposerBatch ());
	}
	return *this->mComposerBatch;
}

//----------------------------------------------------------------//
MOAIGfxComposer* MOAIHasGfxComposerBatch::MOAIGfxComposerBatchInterface_GetComposer ( ZLIndex index ) {

	return this->mComposerBatch ? this->mComposerBatch->GetComposer ( index ) : NULL;
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch* MOAIHasGfxComposerBatch::MOAIGfxComposerBatchInterface_GetComposerBatch () {

	return this->mComposerBatch;
}

