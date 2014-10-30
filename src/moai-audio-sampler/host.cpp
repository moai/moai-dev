// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-audio-sampler/host.h>

#if __APPLE__
	extern void RegisterMOAIAudioSamplerCocoa ();
#endif

//================================================================//
// aku-audio
//================================================================//

//----------------------------------------------------------------//
void AKUAudioSamplerAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAudioSamplerAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAudioSamplerContextInitialize () {

	#if __APPLE__
		RegisterMOAIAudioSamplerCocoa ();
	#endif
}
