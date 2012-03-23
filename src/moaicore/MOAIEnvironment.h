// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIENVIRONMENT_H
#define	MOAIENVIRONMENT_H

#include <moaicore/MOAILua.h>

#define MOAI_ENV_appDisplayName				"appDisplayName"
#define MOAI_ENV_appID						"appID"
#define MOAI_ENV_appVersion					"appVersion"
#define MOAI_ENV_cacheDirectory				"cacheDirectory"
#define MOAI_ENV_carrierISOCountryCode		"carrierISOCountryCode"
#define MOAI_ENV_carrierMobileCountryCode	"carrierMobileCountryCode"
#define MOAI_ENV_carrierMobileNetworkCode	"carrierMobileNetworkCode"
#define MOAI_ENV_carrierName				"carrierName"
#define MOAI_ENV_connectionType				"connectionType"
#define MOAI_ENV_countryCode				"countryCode"
#define MOAI_ENV_cpuabi						"cpuabi"
#define MOAI_ENV_devBrand					"devBrand"
#define MOAI_ENV_devName					"devName"
#define MOAI_ENV_devManufacturer			"devManufacturer"
#define MOAI_ENV_devModel					"devModel"
#define MOAI_ENV_devProduct					"devProduct"
#define MOAI_ENV_documentDirectory			"documentDirectory"
#define MOAI_ENV_iosRetinaDisplay			"iosRetinaDisplay"
#define MOAI_ENV_languageCode				"languageCode"
#define MOAI_ENV_osBrand					"osBrand"
#define MOAI_ENV_osVersion					"osVersion"
#define MOAI_ENV_resourceDirectory			"resourceDirectory"
#define MOAI_ENV_screenHeight				"screenHeight"
#define MOAI_ENV_screenWidth				"screenWidth"
#define MOAI_ENV_udid						"udid"

//================================================================//
// MOAIEnvironment
//================================================================//
/**	@name	MOAIEnvironment
	@text	Holds info about the device.
	
	@const CONNECTION_TYPE_NONE		Signifies that there is no active connection
	@const CONNECTION_TYPE_WIFI		Signifies that the current connection is via WiFi
	@const CONNECTION_TYPE_WWAN		Signifies that the current connection is via WWAN
	
	@const OS_BRAND_ANDROID			Signifies that Moai is currently running on Android
	@const OS_BRAND_IOS				Signifies that Moai is currently running on iOS
	@const OS_BRAND_UNAVAILABLE		Signifies that the operating system cannot be determined
*/
class MOAIEnvironment :
	public MOAIGlobalClass < MOAIEnvironment, MOAILuaObject > {
private:
	
	MOAILuaRef			mListeners;
	
	//----------------------------------------------------------------//
	static int			_generateGUID				( lua_State* L );
	static int			_setListener				( lua_State* L );

	//----------------------------------------------------------------//
	void				CallListener				( cc8* key );

public:	

	//----------------------------------------------------------------//
	enum {
		CONNECTION_TYPE_NONE,
		CONNECTION_TYPE_WIFI,
		CONNECTION_TYPE_WWAN,
	};
	
	enum {	
		OS_BRAND_ANDROID,
		OS_BRAND_IOS,
		OS_BRAND_UNAVAILABLE
	};
	
	DECL_LUA_SINGLETON ( MOAIEnvironment )

	//----------------------------------------------------------------//
	long				GetConnectivity				( );
	STLString			GetResourceDirectory		( );
						MOAIEnvironment				( );
						~MOAIEnvironment			( );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				SetValue					( cc8* key );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetValue ( cc8* key, TYPE value ) {
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLuaClassTable ( state );
		state.SetField < TYPE >( -1, key, value );
		state.Pop ( 1 );
	}
};

#endif
