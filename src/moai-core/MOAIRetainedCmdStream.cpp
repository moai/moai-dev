// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAIRetainedCmdStream.h>

//================================================================//
// MOAIAbstractCmdStreamFilter
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractCmdStreamFilter::FilterCommand ( u32 cmd, const void* param, ZLSize paramSize ) {

	return this->MOAIAbstractCmdStreamFilter_FilterCommand ( cmd, param, paramSize );
}

//================================================================//
// MOAIRetainedCmdStreamOp
//================================================================//
struct MOAIRetainedCmdStreamOp {

	u32								mOp;
	ZLSize							mParamSize;
	MOAIAbstractCmdHandler*			mHandler;
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRetainedCmdStream::_affirmBytecode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRetainedCmdStream, "U" )
	
	self->AffirmBytecode ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRetainedCmdStream::_executeBytecode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRetainedCmdStream, "U" )
	
	self->ExecuteBytecode ();
	return 0;
}

//================================================================//
// MOAIRetainedCmdStream
//================================================================//

//----------------------------------------------------------------//
void MOAIRetainedCmdStream::AffirmBytecode () {

	ZLSize size = this->mCommandStream.GetCursor ();
	if ( size > 0 ) {
		this->mBytecode.Init ( size );
		this->mCommandStream.Seek ( 0 );
		this->mCommandStream.ReadBytes ( this->mBytecode.GetBuffer (), size );
		this->mCommandStream.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIRetainedCmdStream::ExecuteBytecode ( MOAIAbstractCmdStreamFilter* filter ) {

	this->AffirmBytecode ();

	const void* bytecode = this->mBytecode.GetBuffer ();
	const void* top = ( const void* )(( uintptr )bytecode + this->mBytecode.Size ());

	while ( bytecode < top ) {

		const MOAIRetainedCmdStreamOp& command = *( const MOAIRetainedCmdStreamOp* )bytecode;
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( MOAIRetainedCmdStreamOp ));

		if ( filter && filter->FilterCommand ( command.mOp, bytecode, command.mParamSize )) continue;
		command.mHandler->HandleCommand ( command.mOp, bytecode );

		bytecode = ( const void* )(( uintptr )bytecode + command.mParamSize );
	}
}

//----------------------------------------------------------------//
bool MOAIRetainedCmdStream::HasContent () {

	return (( this->mCommandStream.GetCursor () > 0 ) || ( this->mBytecode.Size () > 0 ));
}

//----------------------------------------------------------------//
MOAIRetainedCmdStream::MOAIRetainedCmdStream () {

	RTTI_BEGIN ( MOAIRetainedCmdStream )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIRetainedCmdStream >)
		RTTI_EXTEND ( MOAIAbstractCmdStream )
	RTTI_END

	this->mCommandStream.SetChunkSize ( 256 );
}

//----------------------------------------------------------------//
MOAIRetainedCmdStream::~MOAIRetainedCmdStream () {

	this->Reset ();
}

//----------------------------------------------------------------//
void MOAIRetainedCmdStream::Reset () {

	STLSet < MOAILuaObject* >::iterator retainedIt = this->mRetainedObjects.begin ();
	for ( ; retainedIt != this->mRetainedObjects.end (); ++retainedIt ) {
		this->LuaRelease ( *retainedIt );
	}

	this->mCommandStream.Seek ( 0 );
	this->mBytecode.Clear ();
	this->mRetainedObjects.clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRetainedCmdStream::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIRetainedCmdStream::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "affirmBytecode",				_affirmBytecode },
		{ "executeBytecode",			_executeBytecode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRetainedCmdStream::MOAIAbstractCmdStream_RetainObject ( MOAILuaObject* object ) {

	if ( !this->mRetainedObjects.contains ( object )) {
		this->LuaRetain ( object );
		this->mRetainedObjects.insert ( object );
	}
}

//----------------------------------------------------------------//
void MOAIRetainedCmdStream::MOAIAbstractCmdStream_SubmitCommand ( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize ) {

	MOAIRetainedCmdStreamOp command;
	command.mOp = cmd;
	command.mParamSize = paramSize;
	command.mHandler = &handler;
	
	this->mCommandStream.Write < MOAIRetainedCmdStreamOp >( command );
	this->mCommandStream.WriteBytes ( param, paramSize );
}
