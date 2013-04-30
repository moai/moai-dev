// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-untz/host.h>
#include <moai-untz/MOAIUntzSampleBuffer.h>
#include <moai-untz/MOAIUntzSound.h>
#include <moai-untz/MOAIUntzSystem.h>
#include <UntzSound.h>

//================================================================//
// aku-untz
//================================================================//

//----------------------------------------------------------------//
void AKUInitializeUntz () {
	
	MOAIUntzSystem::Affirm ();
	
	REGISTER_LUA_CLASS ( MOAIUntzSampleBuffer )
	REGISTER_LUA_CLASS ( MOAIUntzSound )
	REGISTER_LUA_CLASS ( MOAIUntzSystem )
}

//----------------------------------------------------------------//
void AKUUntzResume () {
	
	MOAIUntzSystem::Get ().Resume ();
}

//----------------------------------------------------------------//
void AKUUntzSuspend () {

	MOAIUntzSystem::Get ().Suspend ();
}
