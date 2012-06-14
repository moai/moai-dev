// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-fmod-ex.h>
#include <moaiext-fmod-ex/MOAIFmodEx.h>
#include <moaiext-fmod-ex/MOAIFmodExChannel.h>
#include <moaiext-fmod-ex/MOAIFmodExSound.h>

#ifdef MOAI_OS_IPHONE
#include <fmodiphone.h>
#endif

//================================================================//
// AKU-fmod-ex
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
