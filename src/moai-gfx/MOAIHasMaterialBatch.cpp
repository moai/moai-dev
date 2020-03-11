// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIMaterialBatch.h>
#include <moai-gfx/MOAIHasMaterialBatch.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasMaterialBatch::_affirmMaterialBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasMaterialBatch, "U" )
	
	state.Push ( &self->AffirmMaterialBatch ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMaterialBatch
	@text	Return the material batch attached to the prop.
	
	@in		MOAIHasMaterialBatch self
	@out	MOAIMaterialBatch materialBatch
*/
int MOAIHasMaterialBatch::_getMaterialBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasMaterialBatch, "U" )
	
	if ( self->mMaterialBatch ) {
		self->mMaterialBatch.PushRef ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMaterialBatch
	@text	Sets the prop's material batch.
	
	@in		MOAIHasMaterialBatch self
	@opt	MOAIMaterialBatch materialBatch
	@out	nil
*/
int MOAIHasMaterialBatch::_setMaterialBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasMaterialBatch, "U" )
	
	self->mMaterialBatch.Set ( *self, state.GetLuaObject < MOAIMaterialBatch >( 2, true ));
	
	return 0;
}

//================================================================//
// MOAIHasMaterialBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHasMaterialBatch::MOAIHasMaterialBatch () {

	RTTI_BEGIN ( MOAIHasMaterialBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasMaterialBatch >)
		RTTI_EXTEND ( MOAIAbstractMaterialBatchInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasMaterialBatch::~MOAIHasMaterialBatch () {

	this->mMaterialBatch.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasMaterialBatch::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHasMaterialBatch::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "affirmMaterialBatch",	_affirmMaterialBatch },
		{ "getMaterialBatch",		_getMaterialBatch },
		{ "setMaterialBatch",		_setMaterialBatch },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIMaterial& MOAIHasMaterialBatch::MOAIAbstractMaterialBatchInterface_AffirmMaterial ( ZLIndex index ) {

	return this->AffirmMaterialBatch ().AffirmMaterial ( index );
}

//----------------------------------------------------------------//
MOAIMaterialBatch& MOAIHasMaterialBatch::MOAIAbstractMaterialBatchInterface_AffirmMaterialBatch () {

	if ( !this->mMaterialBatch ) {
		this->mMaterialBatch.Set ( *this, new MOAIMaterialBatch ());
	}
	return *this->mMaterialBatch;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIHasMaterialBatch::MOAIAbstractMaterialBatchInterface_GetMaterial ( ZLIndex index ) {

	return this->mMaterialBatch ? this->mMaterialBatch->GetMaterial ( index ) : NULL;
}

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIHasMaterialBatch::MOAIAbstractMaterialBatchInterface_GetMaterialBatch () {

	return this->mMaterialBatch;
}

