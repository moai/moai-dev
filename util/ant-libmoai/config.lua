CONFIG_NAME = 'MOAI_MODULES'

SETTINGS = {
	MY_ARM_MODE = 'arm',
	MY_ARM_ARCH = 'armeabi-v7a',
	MY_APP_PLATFORM = 'android-10',
}

MODULES = {
	
	----------------------------------------------------------------
	BOX2D = {
		
		MODULE_DEFINE = 'AKU_WITH_BOX2D',
		
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
	
		INCLUDES = {
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-box2d.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/moai-box2d.mk',
		},
		
		STATIC_LIBRARIES = 'libbox2d libmoai-box2d',
	},
	
	----------------------------------------------------------------
	CRYPTO = {
		
		MODULE_DEFINE = 'AKU_WITH_CRYPTO',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include-android',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto',
		},
		
		INCLUDES = {
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-crypto-a.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-crypto-b.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-crypto-c.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-crypto-d.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/zl-crypto.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/moai-crypto.mk',
		},
		
		STATIC_LIBRARIES = 'libmoai-crypto libzl-crypto libcrypto-a libcrypto-b libcrypto-c libcrypto-d',
	},
	
	----------------------------------------------------------------
	FMOD_EX = {
	
		MODULE_DEFINE = 'AKU_WITH_FMOD_EX',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/src/moai-fmod-ex',
			'$(FMOD_ANDROID_SDK_ROOT)/api/inc',
		},
		
		INCLUDES = {
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/moai-fmod-ex.mk',
		},
		
		STATIC_LIBRARIES = 'libmoai-fmod-ex',
		SHARED_LIBRARIES = 'fmodex',
	},
	
	----------------------------------------------------------------
	HTTP_CLIENT = {
		
		MODULE_DEFINE = 'AKU_WITH_HTTP_CLIENT',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5',
			'$(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/include-android',
			'$(MOAI_SDK_HOME)/3rdparty/curl-7.19.7/include-android',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include-android',
			'$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include',
		},
		
		INCLUDES = {
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-c-ares.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-curl.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-ssl.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/moai-http-client.mk',
		},
		
		STATIC_LIBRARIES = 'libcares libcurl libssl libmoai-http-client',
	},
	
	----------------------------------------------------------------
	LUAEXT = {
		
		MODULE_DEFINE = 'AKU_WITH_LUAEXT',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/3rdparty/luacrypto-0.2.0/src',
			'$(MOAI_SDK_HOME)/3rdparty/luacurl-1.2.1',
			'$(MOAI_SDK_HOME)/3rdparty/luafilesystem-1.5.0/src',
			'$(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src',
			'$(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2-embed/src',
			'$(MOAI_SDK_HOME)/3rdparty/luasql-2.2.0/src',
		},
		
		INCLUDES = {
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/moai-luaext.mk',
		},
		
		STATIC_LIBRARIES = 'libmoai-luaext',
	},
	
	----------------------------------------------------------------
	SIM = {
		
		MODULE_DEFINE = 'AKU_WITH_SIM',
		
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
			},
		
		INCLUDES = {
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-freetype.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-jpg.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-png.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-tess.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/moai-sim.mk',
		},
		
		STATIC_LIBRARIES = 'libmoai-sim libfreetype libjpg libpng libtess',
	},
	
	----------------------------------------------------------------
	UNTZ = {
		
		MODULE_DEFINE = 'AKU_WITH_UNTZ',
		
		HEADER_SEARCH_PATHS = {
			'$(MOAI_SDK_HOME)/src/moai-untz',
			'$(MOAI_SDK_HOME)/3rdparty/untz/include',
			'$(MOAI_SDK_HOME)/3rdparty/untz/src',
			'$(MOAI_SDK_HOME)/3rdparty/untz/src/native/android',
			'$(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/include',
			'$(MOAI_SDK_HOME)/3rdparty/libvorbis-1.3.2/lib',
			'$(MOAI_SDK_HOME)/3rdparty/libogg-1.2.2/include',
		},
		
		INCLUDES = {
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-ogg.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-vorbis.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-untz.mk',
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/moai-untz.mk',
		},
		
		STATIC_LIBRARIES = 'libogg libvorbis libuntz libmoai-untz',
	},
	
	----------------------------------------------------------------
	UTIL = {
		
		MODULE_DEFINE = 'AKU_WITH_UTIL',
		
		INCLUDES = {
			'$(MOAI_SDK_HOME)/ant/libmoai/modules/moai-util.mk',
		},
		
		STATIC_LIBRARIES = 'libmoai-util',
	},
}

PLUGINS = {
	
	----------------------------------------------------------------
	--FOO_PLUGIN = {
	--	HEADER_SEARCH_PATHS		= '$(MOAI_SDK_HOME)/3rdparty/plugins/',
	--	INCLUDES				= '<foo/plugin.h>',
	--	PREFIX					= 'FOOPlugin',
	--},

	----------------------------------------------------------------
	--BAR_PLUGIN = {
	--	HEADER_SEARCH_PATHS		= '$(MOAI_SDK_HOME)/3rdparty/plugins/',
	--	INCLUDES				= '<bar/plugin.h>',
	--	PREFIX					= 'BARPlugin',
	--},
}

EXTERNAL_LIBRARIES = {
	'libcontrib',
	'libexpat',
	'libjson',
	'liblua',
	'libmoai-android',
	'libmoai-core',
	'libsfmt',
	'libsqlite',
	'libtinyxml',
	'libzl-core',
	'libzl-gfx',
	'libzl-vfs',
	'libzlib',
}

STATIC_LIBRARIES = {

	-- moai
	'libmoai-box2d',
	'libmoai-http-client',
	'libmoai-fmod-ex',
	'libmoai-luaext',
	'libmoai-untz',
	'libmoai-sim',
	'libmoai-crypto',
	'libmoai-util',
	'libmoai-core',
	
	--zl
	'libzl-gfx',
	'libzl-crypto',
	'libzl-core',
	
	-- 3rd party
	'libbox2d',
	
	'libuntz',
	'libvorbis',
	'libogg',
	
	'libcontrib',
	'libexpat',
	'libjson',
	'liblua',
	'libsfmt',
	'libsqlite',
	'libtinyxml',
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
	'libzl-vfs',
	'libzlib',
}

WHOLE_STATIC_LIBRARIES = {
	'libmoai-android',
	'libmoai-sim',
	'libmoai-core',
	'libcrypto-a',
	'libcrypto-b',
	'libcrypto-c',
	'libcrypto-d',
}
