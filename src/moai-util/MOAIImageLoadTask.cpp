// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIDataBuffer.h>
#include <moai-util/MOAIImage.h>
#include <moai-util/MOAIImageLoadTask.h>

//================================================================//
// MOAIImageLoadTask
//================================================================//

//----------------------------------------------------------------//
void MOAIImageLoadTask::Execute () {

	if ( this->mFilename.size() ) {
		this->mImage.Load ( this->mFilename, this->mTransform );
	}
	else if ( this->mDataBuffer ) {

		void* bytes = 0;
		size_t size = 0;
		ZLByteStream stream;

		this->mDataBuffer->Lock ( &bytes, &size );
		
		stream.SetBuffer ( bytes, size );
		stream.SetLength ( size );

		this->mImage.Load ( stream, this->mTransform );

		this->mDataBuffer->Unlock();
	}
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::Init ( cc8* filename, MOAIImage& target, u32 transform ) {

	this->mTarget = &target;

	this->mFilename = filename;
	this->mTransform = transform;
	this->mTarget->LuaRetain ();
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::Init ( MOAIDataBuffer& data, MOAIImage& target, u32 transform ) {

	this->mDataBuffer = &data;
	this->mTarget = &target;

	this->mTransform = transform;
	this->mDataBuffer->LuaRetain ();
	this->mTarget->LuaRetain ();
}

//----------------------------------------------------------------//
MOAIImageLoadTask::MOAIImageLoadTask () :
	mDataBuffer ( 0 ),
	mTarget ( 0 ) {
}

//----------------------------------------------------------------//
MOAIImageLoadTask::~MOAIImageLoadTask () {

	if ( this->mDataBuffer ) {
		this->mDataBuffer->LuaRelease ();
	}
	
	if ( this->mTarget ) {
		this->mTarget->LuaRelease ();
	}
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::Publish () {

	// This provides some degree of thread-safety
	this->mTarget->Take ( this->mImage );

	if ( this->mOnFinish ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnFinish.PushRef ( state )) {
			this->mTarget->PushLuaUserdata ( state );
			state.DebugCall ( 1, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::SetCallback ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	this->mOnFinish.SetRef ( state, idx );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImageLoadTask::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIImageLoadTask::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}
