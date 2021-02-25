// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <string.h>
#include <host-modules/aku_modules.h>
#include <lua-headers/moai_lua.h>
#include <lua-headers/moai_test_mgr_lua.h>

#include <zl-core/zl_replace.h>
#include <zl-core/zl_types.h>

#include <zl-core/ZLLog.h>
#include <zl-core/ZLResult.h>

// TODO: Gfx
#if AKU_WITH_SIM
	#include <moai-gfx-gl/host.h>
#endif

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

    #if AKU_WITH_AR
		AKUArAppFinalize ();
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

	#if AKU_WITH_GFX_GL
		AKUGfxGLAppFinalize ();
	#endif

	#if AKU_WITH_GFX_VK
		AKUGfxVKAppFinalize ();
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

	#if AKU_WITH_UNTZ
		AKUUntzAppFinalize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilAppFinalize ();
	#endif
	
	#if AKU_WITH_VR
		AKUVrAppFinalize ();
	#endif
	
	#if AKU_WITH_IOS
		AKUModulesIosAppFinalize ();
	#endif
	
	#if AKU_WITH_PLUGINS
		AKUPluginsAppFinalize ();
	#endif
}

//----------------------------------------------------------------//
int AKUModulesAppInitialize () {

	#if AKU_WITH_ANDROID
		AKUModulesAndroidAppInitialize ();
	#endif

    #if AKU_WITH_APPLE
		AKUAppleAppInitialize ();
	#endif

    #if AKU_WITH_AR
		AKUArAppInitialize ();
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

	#if AKU_WITH_GFX_GL
		AKUGfxGLAppInitialize ();
	#endif

	#if AKU_WITH_GFX_VK
		AKUGfxVKAppInitialize ();
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

	#if AKU_WITH_UNTZ
		AKUUntzAppInitialize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilAppInitialize ();
	#endif

	#if AKU_WITH_VR
		AKUVrAppInitialize ();
	#endif

	#if AKU_WITH_IOS
		AKUModulesIosAppInitialize ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsAppInitialize ();
	#endif
	
	return AKU_OK;
}

//----------------------------------------------------------------//
int AKUModulesContextInitialize () {

	ZLResultCodeAccumulator result;

	#if AKU_WITH_ANDROID
		AKUModulesAndroidContextInitialize ();
	#endif

    #if AKU_WITH_APPLE
		AKUAppleContextInitialize ();
	#endif

    #if AKU_WITH_AR
		AKUArContextInitialize ();
	#endif

	#if AKU_WITH_ASSIMP
		AKUAssimpContextInitialize ();
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

	#if AKU_WITH_GFX_GL
		AKUGfxGLContextInitialize ();
	#endif

	#if AKU_WITH_GFX_VK
		AKUGfxVKContextInitialize ();
	#endif

	#if AKU_WITH_HARFBUZZ
		AKUHarfBuzzContextInitialize ();
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

	#if AKU_WITH_UNTZ
		AKUUntzContextInitialize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilContextInitialize ();
	#endif
	
	#if AKU_WITH_VR
		AKUVrContextInitialize ();
	#endif
	
	#if AKU_WITH_IOS
		AKUModulesIosContextInitialize ();
	#endif
	
	#if AKU_WITH_PLUGINS
		AKUPluginsContextInitialize ();
	#endif
	
	// have to do this *after* initializing Sim module
	
	#if AKU_WITH_IMAGE_JPG
		AKUImageJpgContextInitialize ();
	#endif

	#if AKU_WITH_IMAGE_PNG
		AKUImagePngContextInitialize ();
	#endif

	#if AKU_WITH_IMAGE_PVR
		AKUImagePvrContextInitialize ();
	#endif

	#if AKU_WITH_IMAGE_TGA
		AKUImageTgaContextInitialize ();
	#endif

	#if AKU_WITH_IMAGE_WEBP
		AKUImageWebPContextInitialize ();
	#endif
	
	result.Reset ();
	result = AKULoadFuncFromBuffer ( moai_lua, moai_lua_SIZE, "moai.lua", AKU_DATA_ZIPPED );
	result = AKUCallFunc ();
	
	if ( result != AKU_OK ) {
		ZLLog_ErrorF ( ZLLog::CONSOLE, "ERROR: Failed to load Moai's embedded Lua wrapper. Some functions may be unavailable.\n" );
	}

	result.Reset ();
	result = AKULoadFuncFromBuffer ( moai_test_mgr_lua, moai_test_mgr_lua_SIZE, "moai_test_mgr.lua", AKU_DATA_ZIPPED );
	result = AKUCallFunc ();
	
	if ( result != AKU_OK ) {
		ZLLog_ErrorF ( ZLLog::CONSOLE, "ERROR: Failed to load Moai's embedded test manager. Some functions may be unavailable.\n" );
	}
	
	return result;
}

//----------------------------------------------------------------//
void AKUModulesPause ( bool pause ) {
	
	#if AKU_WITH_ANDROID
		AKUModulesAndroidPause ( pause );
	#endif

    #if AKU_WITH_AR
		AKUArPause ( pause );
	#endif

	#if AKU_WITH_SIM
		AKUPause ( pause );
	#endif
	
	#if AKU_WITH_UNTZ
		AKUUntzPause ( pause );
	#endif
	
	#if AKU_WITH_VR
		AKUVrPause ( pause );
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
	
	#if AKU_WITH_IOS
		AKUModulesIosUpdate ();
	#endif
	
	#if AKU_WITH_PLUGINS
		AKUPluginsUpdate ();
	#endif
	
	#if AKU_WITH_VR
		AKUVrUpdate ();
	#endif
	
	// do this last
	#if AKU_WITH_SIM
		AKUUpdate ();
	#endif
}
