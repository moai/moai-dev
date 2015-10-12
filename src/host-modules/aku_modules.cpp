// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <string.h>
#include <host-modules/aku_modules.h>
#include <lua-headers/moai_lua.h>
#include <lua-headers/moai_test_mgr_lua.h>

//================================================================//
// objc modules
//================================================================//

#if AKU_WITH_IOS

	//----------------------------------------------------------------//
	extern void		AKUModulesIosAppFinalize			();
	extern void		AKUModulesIosAppInitialize			();
	extern void		AKUModulesIosContextInitialize		();
	extern void		AKUModulesIosPause					( bool pause );
	extern void		AKUModulesIosUpdate					();

#endif

#if AKU_WITH_PLUGINS

	//----------------------------------------------------------------//
	extern void		AKUPluginsAppFinalize				();
	extern void		AKUPluginsAppInitialize				();
	extern void		AKUPluginsContextInitialize			();
	extern void		AKUPluginsPause						( bool pause );
	extern void		AKUPluginsUpdate					();

#endif

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesAppFinalize () {

	#if AKU_WITH_ANDROID
		AKUModulesAndroidAppFinalize ();
	#endif

    #if AKU_WITH_APPLE
		AKUAppleAppFinalize ();
	#endif

    #if AKU_WITH_AUDIO_SAMPLER
		AKUAudioSamplerAppFinalize ();
	#endif

	#if AKU_WITH_BOX2D
		AKUBox2DAppFinalize ();
	#endif

	#if AKU_WITH_CRYPTO
		AKUCryptoAppFinalize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerAppFinalize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExAppFinalize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessAppFinalize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientAppFinalize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerAppFinalize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtAppFinalize ();
	#endif

	#if AKU_WITH_SDL
		AKUSdlAppFinalize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimAppFinalize ();
	#endif

	#if AKU_WITH_SPINE
		AKUSpineAppFinalize ();
	#endif 

	#if AKU_WITH_UNTZ
		AKUUntzAppFinalize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilAppFinalize ();
	#endif
	
	#if AKU_WITH_IOS
		AKUModulesIosAppFinalize ();
	#endif
	
	#if AKU_WITH_PLUGINS
		AKUPluginsAppFinalize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAppInitialize () {

	#if AKU_WITH_ANDROID
		AKUModulesAndroidAppInitialize ();
	#endif

    #if AKU_WITH_APPLE
		AKUAppleAppInitialize ();
	#endif

    #if AKU_WITH_AUDIO_SAMPLER
		AKUAudioSamplerAppInitialize ();
	#endif

	#if AKU_WITH_BOX2D
		AKUBox2DAppInitialize ();
	#endif

	#if AKU_WITH_CRYPTO
		AKUCryptoAppInitialize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerAppInitialize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExAppInitialize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessAppInitialize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientAppInitialize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerAppInitialize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtAppInitialize ();
	#endif

	#if AKU_WITH_SDL
		AKUSdlAppInitialize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimAppInitialize ();
	#endif

	#if AKU_WITH_SPINE
		AKUSpineAppInitialize ();
	#endif

	#if AKU_WITH_UNTZ
		AKUUntzAppInitialize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilAppInitialize ();
	#endif

	#if AKU_WITH_IOS
		AKUModulesIosAppInitialize ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsAppInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesContextInitialize () {

	#if AKU_WITH_ANDROID
		AKUModulesAndroidContextInitialize ();
	#endif

    #if AKU_WITH_APPLE
		AKUAppleContextInitialize ();
	#endif

    #if AKU_WITH_AUDIO_SAMPLER
		AKUAudioSamplerContextInitialize ();
	#endif

	#if AKU_WITH_BOX2D
		AKUBox2DContextInitialize ();
	#endif

	#if AKU_WITH_CRYPTO
		AKUCryptoContextInitialize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerContextInitialize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExContextInitialize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessContextInitialize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientContextInitialize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerContextInitialize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtContextInitialize ();
	#endif

	#if AKU_WITH_SDL
		AKUSdlContextInitialize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimContextInitialize ();
	#endif

	#if AKU_WITH_SPINE
		AKUSpineContextInitialize ();
	#endif

	#if AKU_WITH_UNTZ
		AKUUntzContextInitialize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilContextInitialize ();
	#endif
	
	#if AKU_WITH_IOS
		AKUModulesIosContextInitialize ();
	#endif
	
	#if AKU_WITH_PLUGINS
		AKUPluginsContextInitialize ();
	#endif
	
	AKULoadFuncFromBuffer ( moai_lua, moai_lua_SIZE, AKU_DATA_STRING, AKU_DATA_ZIPPED );
	AKUCallFunc ();
	
	AKULoadFuncFromBuffer ( moai_test_mgr_lua, moai_test_mgr_lua_SIZE, AKU_DATA_STRING, AKU_DATA_ZIPPED );
	AKUCallFunc ();
}

//----------------------------------------------------------------//
void AKUModulesPause ( bool pause ) {
	
	#if AKU_WITH_ANDROID
		AKUModulesAndroidPause ( pause );
	#endif

	#if AKU_WITH_SIM
		AKUPause ( pause );
	#endif
	
	#if AKU_WITH_UNTZ
		AKUUntzPause ( pause );
	#endif
	
	#if AKU_WITH_IOS
		AKUModulesIosPause ( pause );
	#endif
	
	#if AKU_WITH_PLUGINS
		AKUPluginsPause ( pause );
	#endif
}

//----------------------------------------------------------------//
void AKUModulesUpdate () {

	#if AKU_WITH_ANDROID
		AKUModulesAndroidUpdate ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientUpdate ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerUpdate ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExUpdate ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessUpdate ()
	#endif

	#if AKU_WITH_SIM
		AKUUpdate ();
	#endif
	
	#if AKU_WITH_IOS
		AKUModulesIosUpdate ();
	#endif
	
	#if AKU_WITH_PLUGINS
		AKUPluginsUpdate ();
	#endif
}
