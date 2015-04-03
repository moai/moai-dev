CONFIG_NAME = 'MOAI_MODULES' -- this will create a variable in Android.mk containing the relative path to this config file

-- Additional config files maybe specified on the command line. moaiutil libmoai-ant will
-- also look in its invoke directory for a config file that will be applied before additional
-- config's specified on the command line.

-- config file contents are composed one of top of another. See additional comments below.

SETTINGS = {
	LIB_NAME			= 'moai',
	MY_ARM_MODE			= 'arm',
	MY_ARM_ARCH			= 'armeabi-v7a x86',
	MY_APP_PLATFORM		= 'android-10',
}

MODULES = {
	
	-- Modules in subsequent config will be added to the total set of modules. Later modules
	-- with the same name as previous modules will *replace* those modules.

	-- Module contents:
	--   PREPROCESSOR_FLAG		- This is the preprocessor flag used to #ifdef the module. (string)
	--   NAMESPACE				- Java namespace (if Java files to be included). (string)
	--   PLUGIN
	--      INCLUDES            - Include(s) files for plugin initializer. (table or string)
	--      PREFIX				- Generates name of plugin functions (if any). (string)
	--   HEADER_SEARCH_PATHS	- Added to main Android.mk. (table)
	--   MAKE					- One or more makefiles to include. (table)
	--   JAVA					- Imports Java source files or library components. (table)
	--   STATIC_LIBRARIES		- Static library modules. (string)

	----------------------------------------------------------------
	ADCOLONY = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_ADCOLONY',
		NAMESPACE = 'com.ziplinegames.moai',
		
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-adcolony.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/adcolony-2.0.6',
			MOAI_SDK_HOME .. 'src/moai-android-adcolony',
		},

		STATIC_LIBRARIES = 'libmoai-adcolony',
	},

	----------------------------------------------------------------
	ANDROID = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-android.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/contrib',
			MOAI_SDK_HOME .. 'src/moai-android',
		},

		STATIC_LIBRARIES = 'libmoai-android',
	},

	----------------------------------------------------------------
	BOX2D = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_BOX2D',
		NAMESPACE = 'com.ziplinegames.moai',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/',
			'$(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D',
			'$(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Collision/Shapes',
			'$(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Collision',
			'$(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Common',
			'$(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Dynamics',
			'$(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Dynamics/Contacts',
			'$(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Dynamics/Joints',
		},
	
		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-box2d.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-box2d.mk',
		},
		
		JAVA = {
		},

		STATIC_LIBRARIES = 'libbox2d libmoai-box2d',
	},
	
	----------------------------------------------------------------
	CHARTBOOST = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_CHARTBOOST',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-chartboost.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/chartboost-5.1.1',
			MOAI_SDK_HOME .. 'src/moai-android-chartboost',
		},

		STATIC_LIBRARIES = 'libmoai-chartboost',
	},

	----------------------------------------------------------------
	CRITTERCISM = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_CRITTERCISM',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-crittercism.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/crittercism-4.4.0',
			MOAI_SDK_HOME .. 'src/moai-android-crittercism',
		},

		STATIC_LIBRARIES = 'libmoai-crittercism',
	},

	----------------------------------------------------------------
	CRYPTO = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_CRYPTO',
		NAMESPACE = 'com.ziplinegames.moai',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include-android',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto',
		},
		
		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-crypto-a.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-crypto-b.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-crypto-c.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-crypto-d.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/zl-crypto.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-crypto.mk',
		},
		
		JAVA = {
		},

		STATIC_LIBRARIES = 'libmoai-crypto libzl-crypto libcrypto-a libcrypto-b libcrypto-c libcrypto-d',
	},
	
	----------------------------------------------------------------
	FACEBOOK = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_FACEBOOK',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-facebook.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/facebook-v3',
			MOAI_SDK_HOME .. 'src/moai-android-facebook',
		},

		STATIC_LIBRARIES = 'libmoai-facebook',
	},

	----------------------------------------------------------------
	FLURRY = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_FLURRY',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-flurry.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/flurry-3.3.4',
			MOAI_SDK_HOME .. 'src/moai-android-flurry',
		},

		STATIC_LIBRARIES = 'libmoai-flurry',
	},

	----------------------------------------------------------------
	--[[
	FMOD_EX = {
	
		PREPROCESSOR_FLAG = 'AKU_WITH_FMOD_EX',
		NAMESPACE = 'com.ziplinegames.moai',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/src/moai-fmod-ex',
			'$(FMOD_ANDROID_SDK_ROOT)/api/inc',
		},
		
		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-fmod-ex.mk',
		},
		
		JAVA = {
		},

		STATIC_LIBRARIES = 'libmoai-fmod-ex',
	},
	]]--

	----------------------------------------------------------------
	GOOGLE_PLAY_SERVICES = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_GOOGLE_PLAY',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-google-play-services.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/google-play-services',
			MOAI_SDK_HOME .. 'src/moai-android-google-play-services',
		},

		STATIC_LIBRARIES = 'libmoai-google-play-services',
	},

	----------------------------------------------------------------
	--[[
	GOOGLE_PUSH = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_GOOGLE_PUSH',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/google-push',
			MOAI_SDK_HOME .. 'src/moai-android-google-push',
		},

		STATIC_LIBRARIES = 'libmoai-google-push',
	},
	]]--

	----------------------------------------------------------------
	HTTP_CLIENT = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_HTTP_CLIENT',
		NAMESPACE = 'com.ziplinegames.moai',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5',
			'$(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/include-android',
			'$(MOAI_SDK_HOME)/3rdparty/curl-7.19.7/include-android',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include-android',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include',
		},
		
		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-c-ares.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-curl.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-ssl.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-http-client.mk',
		},
		
		JAVA = {
		},

		STATIC_LIBRARIES = 'libcares libcurl libssl libmoai-http-client',
	},
	
	----------------------------------------------------------------
	LUAEXT = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_LUAEXT',
		NAMESPACE = 'com.ziplinegames.moai',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/3rdparty/luacrypto-0.2.0/src',
			'$(MOAI_SDK_HOME)/3rdparty/luacurl-1.2.1',
			'$(MOAI_SDK_HOME)/3rdparty/luafilesystem-1.5.0/src',
			'$(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src',
			'$(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2-embed/src',
			'$(MOAI_SDK_HOME)/3rdparty/luasql-2.2.0/src',
		},
		
		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-luaext.mk',
		},
		
		JAVA = {
		},

		STATIC_LIBRARIES = 'libmoai-luaext',
	},
	
	----------------------------------------------------------------
	SIM = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_SIM',
		NAMESPACE = 'com.ziplinegames.moai',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/include',
			'$(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/include/freetype',
			'$(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/include/freetype2',
			'$(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/builds',
			'$(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/src',
			'$(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/config',
			'$(MOAI_SDK_HOME)/3rdparty/libtess2/Include',
			'$(MOAI_SDK_HOME)/3rdparty/jpeg-8c',
			'$(MOAI_SDK_HOME)/3rdparty/lpng140',
			'$(MOAI_SDK_HOME)/3rdparty/libpvr-3.4',
		},
		
		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-freetype.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-jpg.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-png.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-pvr.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-tess.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/zl-gfx.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-sim.mk',
		},
		
		JAVA = {
		},

		STATIC_LIBRARIES = 'libmoai-sim libfreetype libjpg libpng libpvr libtess libzl-gfx',
	},
	
	----------------------------------------------------------------
	TAPJOY = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_TAPJOY',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-tapjoy.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/tapjoy-9.1.5',
			MOAI_SDK_HOME .. 'src/moai-android-tapjoy',
		},

		STATIC_LIBRARIES = 'libmoai-tapjoy',
	},

	----------------------------------------------------------------
	TWITTER = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_TWITTER',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-twitter.mk',
		},
		
		JAVA = {
		},

		STATIC_LIBRARIES = 'libmoai-twitter',
	},

	----------------------------------------------------------------
	UNTZ = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_UNTZ',
		NAMESPACE = 'com.ziplinegames.moai',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/src/moai-untz',
			'$(MOAI_SDK_HOME)/3rdparty/untz/include',
			'$(MOAI_SDK_HOME)/3rdparty/untz/src',
			'$(MOAI_SDK_HOME)/3rdparty/untz/src/native/android',
			'$(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/include',
			'$(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib',
			'$(MOAI_SDK_HOME)/3rdparty/libogg-1.2.2/include',
		},
		
		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-ogg.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-vorbis.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-untz.mk',
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-untz.mk',
		},
		
		JAVA = {
		},

		STATIC_LIBRARIES = 'libogg libvorbis libuntz libmoai-untz',
	},
	
	----------------------------------------------------------------
	VUNGLE = {
		
		PREPROCESSOR_FLAG = 'AKU_WITH_ANDROID_VUNGLE',
		NAMESPACE = 'com.ziplinegames.moai',
	
		HEADER_SEARCH_PATHS = {
		},

		MAKE = {
			'$(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-vungle.mk',
		},
		
		JAVA = {
			MOAI_SDK_HOME .. '3rdparty-android/vungle-1.3.11',
			MOAI_SDK_HOME .. 'src/moai-android-vungle',
		},

		STATIC_LIBRARIES = 'libmoai-vungle',
	},
}

TARGETS = {
	
	-- Each target produces a shared object named lib<NAME>.so (for example, the
	-- target below produced libmoai.so).

	-- Like modules, new targets may be added in subsequent config files and existing
	-- targets may be overwritten.

	-- Targets may be overwritten via the command line, or be specifying an empty/nameless
	-- target override in a subsequent config file.

	-- Target contents:
	--   NAME			- Name of the .so. If blank, target will be ignored. (string)
	--   MODULES		- Modules to link with the .so. (table)

	----------------------------------------------------------------
	MOAI = {
		
		NAME = 'moai',
		
		MODULES = {
			'ADCOLONY',
			'ANDROID',
			'BOX2D',
			'CHARTBOOST',
			'CRYPTO',
			'FACEBOOK',
			'FLURRY',
			'GOOGLE_PLAY_SERVICES',
			'HTTP_CLIENT',
			'LUAEXT',
			'SIM',
			'TAPJOY',
			'TWITTER',
			'UNTZ',
			'VUNGLE',
		},
	},
}

-- This controls the link order of any static libraries included via the modules.
-- Subsequent config files will *prepend* the contents of this table.

STATIC_LINK_ORDER = {

	-- integrations
	'libmoai-adcolony',
	'libmoai-chartboost',
	'libmoai-crittercism',
	'libmoai-facebook',
	'libmoai-flurry',
	'libmoai-google-play-services',
	'libmoai-tapjoy',
	'libmoai-twitter',
	'libmoai-vungle',

	'libmoai-chartboost',

	-- moai
	'libmoai-android',
	'libmoai-box2d',
	'libmoai-http-client',
	'libmoai-fmod-ex',
	'libmoai-luaext',
	'libmoai-untz',
	'libmoai-sim',
	'libmoai-crypto',
	
	--zl
	'libzl-gfx',
	'libzl-crypto',
	
	-- 3rd party
	'libbox2d',
	
	'libuntz',
	'libvorbis',
	'libogg',
	
	'libpvr',
	'libfreetype',
	'libjpg',
	'libpng',
	'libtess',
	
	'libcurl',
	'libcares',
	'libssl',
	
	'libcrypto-a',
	'libcrypto-b',
	'libcrypto-c',
	'libcrypto-d',
	
	--vfs
	--'libzl-vfs',
	--'libzlib',
}

-- Libraries to flag for symbol export.

WHOLE_STATIC_LIBRARIES = {
	'libmoai-android',
	'libmoai-sim',
	'libmoai-core',
	'libcrypto-a',
	'libcrypto-b',
	'libcrypto-c',
	'libcrypto-d',
}
