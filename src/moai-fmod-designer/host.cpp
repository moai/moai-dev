// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sim/host.h>

#include <moai-fmod-designer/host.h>
#include <moai-fmod-designer/MOAIFmodEventMgr.h>
#include <moai-fmod-designer/MOAIFmodEventInstance.h>
#include <moai-fmod-designer/MOAIFmodMicrophone.h>
#include <moai-fmod-designer/Source/EventManager.h>

#ifdef MOAI_OS_IPHONE
    #include <fmodiphone.h>
#endif

//================================================================//
// aku-fmod-designer
//================================================================//

//----------------------------------------------------------------//
void AKUFmodDesignerAppFinalize () {

	FMODDesigner::tEventManager.Shutdown ();
}

//----------------------------------------------------------------//
void AKUFmodDesignerAppInitialize () {
}

//----------------------------------------------------------------//
void AKUFmodDesignerContextInitialize () {

	MOAIFmodEventMgr::Affirm ();

    REGISTER_LUA_CLASS ( MOAIFmodEventMgr )    
    REGISTER_LUA_CLASS ( MOAIFmodEventInstance )
    REGISTER_LUA_CLASS ( MOAIFmodMicrophone )
}

//----------------------------------------------------------------//
void AKUFmodDesignerUpdate () {

    FMODDesigner::tEventManager.Update (( float )AKUGetSimStep ());
}
