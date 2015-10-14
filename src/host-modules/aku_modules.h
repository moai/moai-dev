// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_H
#define	AKU_MODULES_H

#include <host-modules/aku_modules_config.h>
#include <host-modules/aku_modules_util.h>

#include <moai-core/host.h>

#if AKU_WITH_ANDROID
	#include <host-modules/aku_modules_android.h>
	#include <moai-android/host.h>
#endif


#if AKU_WITH_APPLE
	#include <moai-apple/host.h>
#endif

#if AKU_WITH_AUDIO_SAMPLER
	#include <moai-audio-sampler/host.h>
#endif

#if AKU_WITH_BOX2D
	#include <moai-box2d/host.h>
#endif

#if AKU_WITH_CRYPTO
	#include <moai-crypto/host.h>
#endif

#if AKU_WITH_FMOD_DESIGNER
	#include <moai-fmod-designer/host.h>
#endif

#if AKU_WITH_FMOD_EX
	#include <moai-fmod-ex/host.h>
#endif

#if AKU_WITH_HARNESS
	#include <moai-harness/host.h>
#endif

#if AKU_WITH_HTTP_CLIENT
	#include <moai-http-client/host.h>
#endif

#if AKU_WITH_HTTP_SERVER
	#include <moai-http-server/host.h>
#endif

#if AKU_WITH_LUAEXT
	#include <moai-luaext/host.h>
#endif

#if AKU_WITH_SDL
	#include <moai-sdl/host.h>
#endif

#if AKU_WITH_SIM
	#include <moai-sim/host.h>
#endif

#if AKU_WITH_SPINE
    #include <moai-spine/host.h>
#endif

#if AKU_WITH_UNTZ
	#include <moai-untz/host.h>
#endif

#if AKU_WITH_UTIL
	#include <moai-util/host.h>
#endif

//----------------------------------------------------------------//
void		AKUModulesAppFinalize					();
void		AKUModulesAppInitialize					();
void		AKUModulesContextInitialize				();
void		AKUModulesPause							( bool pause );
void		AKUModulesUpdate						();

#endif
