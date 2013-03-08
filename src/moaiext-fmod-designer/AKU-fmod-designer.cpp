// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-fmod-designer.h>
#include <moaiext-fmod-designer/MOAIFmodEventMgr.h>
#include <moaiext-fmod-designer/MOAIFmodEventInstance.h>
#include <moaiext-fmod-designer/MOAIFmodMicrophone.h>
#include <moaiext-fmod-designer/Source/EventManager.h>

#ifdef MOAI_OS_IPHONE
    #include <fmodiphone.h>
#endif

//================================================================//
// AKU-fmod-designer
//================================================================//

//----------------------------------------------------------------//
void AKUFmodDesignerInit () {

    MOAIFmodEventMgr::Affirm ();

    REGISTER_LUA_CLASS ( MOAIFmodEventMgr )    
    REGISTER_LUA_CLASS ( MOAIFmodEventInstance )
    REGISTER_LUA_CLASS ( MOAIFmodMicrophone )
}

//----------------------------------------------------------------//
void AKUFmodDesignerMuteSystem ( bool mute ) {
    FMODDesigner::tEventManager.MuteAllEvents( mute );
}

//----------------------------------------------------------------//
void AKUFmodDesignerRelease () {    
    FMODDesigner::tEventManager.Shutdown();
}

//----------------------------------------------------------------//
void AKUFmodDesignerRestoreSession () {
#ifdef MOAI_OS_IPHONE
    FMOD_IPhone_RestoreAudioSession ();
#endif
}

//----------------------------------------------------------------//
void AKUFmodDesignerUpdate ( float fDeltaTime ) {

    FMODDesigner::tEventManager.Update( fDeltaTime );
}
