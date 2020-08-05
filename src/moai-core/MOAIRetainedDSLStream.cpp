// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractDSLInterface.h>
#include <moai-core/MOAIRetainedDSLStream.h>

//================================================================//
// MOAIRetainedDSLStreamOp
//================================================================//
struct MOAIRetainedDSLStreamOp {

	u32								mOp;
	ZLSize							mParamSize;
	MOAIAbstractDSLInterface*		mHandler;
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRetainedDSLStream::_affirmBytecode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRetainedDSLStream, "U" )
	
	self->AffirmBytecode ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRetainedDSLStream::_executeBytecode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRetainedDSLStream, "U" )
	
	self->ExecuteBytecode ();
	return 0;
}

//================================================================//
// MOAIRetainedDSLStream
//================================================================//

//----------------------------------------------------------------//
void MOAIRetainedDSLStream::AffirmBytecode () {

	ZLSize size = this->mCommandStream.GetCursor ();
	if ( size > 0 ) {
		this->mBytecode.Init ( size );
		this->mCommandStream.Seek ( 0 );
		this->mCommandStream.ReadBytes ( this->mBytecode.GetBuffer (), size );
		this->mCommandStream.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIRetainedDSLStream::ExecuteBytecode () {

	this->AffirmBytecode ();

//	bool didCall = false;

	const void* bytecode = this->mBytecode.GetBuffer ();
	const void* top = ( const void* )(( uintptr )bytecode + this->mBytecode.Size ());

	while ( bytecode < top ) {

		const MOAIRetainedDSLStreamOp& command = *( const MOAIRetainedDSLStreamOp* )bytecode;
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( MOAIRetainedDSLStreamOp ));

		command.mHandler->SubmitCommand ( command.mOp, bytecode, command.mParamSize );
//		this->Execute ( callable, command.mType, bytecode );
		bytecode = ( const void* )(( uintptr )bytecode + command.mParamSize );

//		didCall = didCall || ( command.mType == CALL ) || ( command.mType == CALL_FROM_SHADER );
	}

//	if ( !didCall ) {
//		this->Execute ( callable, callCommand, NULL );
//	}
}

//----------------------------------------------------------------//
bool MOAIRetainedDSLStream::HasContent () {

	return (( this->mCommandStream.GetCursor () > 0 ) || ( this->mBytecode.Size () > 0 ));
}

//----------------------------------------------------------------//
MOAIRetainedDSLStream::MOAIRetainedDSLStream () {

	RTTI_BEGIN ( MOAIRetainedDSLStream )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIRetainedDSLStream >)
		RTTI_EXTEND ( MOAIAbstractDSLStream )
	RTTI_END

	this->mCommandStream.SetChunkSize ( 256 );
}

//----------------------------------------------------------------//
MOAIRetainedDSLStream::~MOAIRetainedDSLStream () {

	this->Reset ();
}

//----------------------------------------------------------------//
void MOAIRetainedDSLStream::Reset () {

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
void MOAIRetainedDSLStream::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIRetainedDSLStream::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "affirmBytecode",				_affirmBytecode },
		{ "executeBytecode",			_executeBytecode },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRetainedDSLStream::MOAIAbstractDSLHandler_RetainObject ( MOAILuaObject* object ) {

	if ( !this->mRetainedObjects.contains ( object )) {
		this->LuaRetain ( object );
		this->mRetainedObjects.insert ( object );
	}
}

//----------------------------------------------------------------//
void MOAIRetainedDSLStream::MOAIAbstractDSLHandler_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {

	assert ( this->mDSLInterface );

	MOAIRetainedDSLStreamOp command;
	command.mOp = cmd;
	command.mParamSize = paramSize;
	command.mHandler = this->mDSLInterface;
	
	this->mCommandStream.Write < MOAIRetainedDSLStreamOp >( command );
	this->mCommandStream.WriteBytes ( param, paramSize );
}
