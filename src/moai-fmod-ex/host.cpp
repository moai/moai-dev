// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-ex/host.h>
#include <moai-fmod-ex/MOAIFmodEx.h>
#include <moai-fmod-ex/MOAIFmodExChannel.h>
#include <moai-fmod-ex/MOAIFmodExSound.h>

#ifdef MOAI_OS_IPHONE
	#include <fmodiphone.h>
#endif

//================================================================//
// aku-fmod-ex
//================================================================//

//----------------------------------------------------------------//
void AKUFmodExInit () {

	MOAIFmodEx::Affirm ();
	
	REGISTER_LUA_CLASS ( MOAIFmodEx )
	REGISTER_LUA_CLASS ( MOAIFmodExChannel )
	REGISTER_LUA_CLASS ( MOAIFmodExSound )

	#ifdef MOAI_OS_NACL
		MOAIFmodEx::Get ().OpenSoundSystem ();
	#endif
}

//----------------------------------------------------------------//
void AKUFmodMuteSystem ( bool mute ) {
	
	MOAIFmodEx::Get().MuteChannels ( mute );
}

//----------------------------------------------------------------//
void AKUFmodExRelease () {
	MOAIFmodEx::Get ().CloseSoundSystem ();
}

//----------------------------------------------------------------//
void AKUFmodExRestoreSession () {
	#ifdef MOAI_OS_IPHONE
		FMOD_IPhone_RestoreAudioSession ();
	#endif
}

//----------------------------------------------------------------//
void AKUFmodExUpdate () {

	MOAIFmodEx::Get ().Update ();
}
