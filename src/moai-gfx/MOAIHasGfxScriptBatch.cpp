// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIDraw.h>
#include <moai-gfx/MOAIHasGfxScriptBatch.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
 
	@in		MOAIGfxScriptBatch self
	@out	number indexBatchSize
*/
int MOAIHasGfxScriptBatch::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" );
	
	state.Push ( MOAILuaSize ( self->GetIndexBatchSize ()));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatch::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" );
	state.Push ( self->GetGfxScript ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatch::_gfx ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	MOAIGfxScript& gfxScript = self->AffirmGfxScript ( 0 );
	gfxScript.Reset ();
	gfxScript.AffirmMedium ().PushCmdInterfaceWithHandler ( state, MOAIDraw::Get ());
	
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatch::_gfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" );
	state.Push ( &self->AffirmGfxScript ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveGfxScripts
	@text	Reserve material indices.
 
	@in		MOAIGfxScriptBatch self
	@opt	number count
	@out	nil
*/
int MOAIHasGfxScriptBatch::_reserveGfxScripts ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	self->ReserveGfxScripts ( state.GetValue ( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexBatchSize
	@text	Set the index batch size. When a prop or deck is drawing,
			the index is divided by the material batch's index batch size
			to get the material index. In this way sets of deck indices
			may be assigned to material indices. For example, an index
			batch size of 256 for a set of 4 materials would distribute
			1024 deck indices across materials 1 though 4. An index batch
			size of 1 would create a 1 to 1 mapping between deck indices
			and materials.
 
	@in		MOAIGfxScriptBatch self
	@opt	number indexBatchSize		Default value is 1.
	@out	nil
*/
int MOAIHasGfxScriptBatch::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	self->SetIndexBatchSize ( state.GetValue < u32 >( 2, 1 ));
	return 0;
}

//================================================================//
// MOAIHasGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScript& MOAIHasGfxScriptBatch::AffirmGfxScript ( ZLIndex index ) {

	this->mGfxScripts.Grow (( ZLSize )index + 1 );
	MOAIGfxScript* gfxScript = this->mGfxScripts [ index ];
	
	if ( !gfxScript ) {
		gfxScript = new MOAIGfxScript ();
		this->mGfxScripts [ index ] = gfxScript;
	}
	return *gfxScript;
}

//----------------------------------------------------------------//
MOAIGfxScript* MOAIHasGfxScriptBatch::GetGfxScript ( ZLIndex index ) {

	index = this->WrapIndex ( index );
	return ( index < this->mGfxScripts.Size ()) ? ( MOAIGfxScript* )this->mGfxScripts [ index ] : NULL;
}

//----------------------------------------------------------------//
MOAIHasGfxScriptBatch::MOAIHasGfxScriptBatch () :
	mIndexBatchSize ( 1 ) {

	RTTI_BEGIN ( MOAIHasGfxScriptBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxScriptBatch >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScriptBatch::~MOAIHasGfxScriptBatch () {
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::ReserveGfxScripts ( ZLSize size ) {

	this->mGfxScripts.Init ( size );
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::SetGfxScript ( ZLIndex index, MOAIGfxScript* gfxScript ) {

	this->mGfxScripts.Grow ( index + 1 );
	this->mGfxScripts [ index ] = gfxScript;
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
		{ "getGfxScript",			_getGfxScript },
		{ "getIndexBatchSize",		_getIndexBatchSize },
		{ "gfx",					_gfx },
		{ "gfxScript",				_gfxScript },
		{ "reserveGfxScripts",		_reserveGfxScripts },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
