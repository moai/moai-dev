// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIENVIRONMENT_H
#define	MOAIENVIRONMENT_H

#include <moaicore/MOAIEventSource.h>
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
#define MOAI_ENV_devPlatform				"devPlatform"
#define MOAI_ENV_devProduct					"devProduct"
#define MOAI_ENV_documentDirectory			"documentDirectory"
#define MOAI_ENV_iosRetinaDisplay			"iosRetinaDisplay"
#define MOAI_ENV_languageCode				"languageCode"
#define MOAI_ENV_numProcessors				"numProcessors"
#define MOAI_ENV_osBrand					"osBrand"
#define MOAI_ENV_osVersion					"osVersion"
#define MOAI_ENV_resourceDirectory			"resourceDirectory"
#define MOAI_ENV_screenDpi					"screenDpi"
#define MOAI_ENV_verticalResolution			"verticalResolution"
#define MOAI_ENV_horizontalResolution		"horizontalResolution"
#define MOAI_ENV_udid						"udid"
#define MOAI_ENV_openUdid					"openUdid"

//================================================================//
// MOAIEnvironment
//================================================================//
/**	@name	MOAIEnvironment
	@text	<p>Table of key/value pairs containing information about the current
			environment. Also contains the generateGUID (), which will move to
			MOAIUnique in a future release.</p>
			
			<p>If a given key is not supported in the current environment it will
			not exist (it's value will be nil).</p>
			
			<p>The keys are:</p>
			
			<p>
			<ul>
			<li>appDisplayName</li>
			<li>appID</li>
			<li>appVersion</li>
			<li>cacheDirectory</li>
			<li>carrierISOCountryCode</li>
			<li>carrierMobileCountryCode</li>
			<li>carrierMobileNetworkCode</li>
			<li>carrierName</li>
			<li>connectionType</li>
			<li>countryCode</li>
			<li>cpuabi</li>
			<li>devBrand</li>
			<li>devName</li>
			<li>devManufacturer</li>
			<li>devModel</li>
			<li>devPlatform</li>
			<li>devProduct</li>
			<li>documentDirectory</li>
			<li>iosRetinaDisplay</li>
			<li>languageCode</li>
			<li>numProcessors</li>
			<li>osBrand</li>
			<li>osVersion</li>
			<li>resourceDirectory</li>
			<li>screenDpi</li>
			<li>verticalResolution</li>
			<li>horizontalResolution</li>
			<li>udid</li>
			<li>openUdid</li>
			</ul>
			</p>
	
	@const CONNECTION_TYPE_NONE		Signifies that there is no active connection
	@const CONNECTION_TYPE_WIFI		Signifies that the current connection is via WiFi
	@const CONNECTION_TYPE_WWAN		Signifies that the current connection is via WWAN
	
	@const OS_BRAND_ANDROID			Signifies that Moai is currently running on Android
	@const OS_BRAND_IOS				Signifies that Moai is currently running on iOS
	@const OS_BRAND_OSX				Signifies that Moai is currently running on OSX
	@const OS_BRAND_LINUX			Signifies that Moai is currently running on Linux
	@const OS_BRAND_WINDOWS			Signifies that Moai is currently running on Windows
	@const OS_BRAND_UNAVAILABLE		Signifies that the operating system cannot be determined
*/
class MOAIEnvironment :
	public MOAIGlobalClass < MOAIEnvironment, MOAIGlobalEventSource > {
private:
	
	MOAILuaRef			mListeners;
	
	//----------------------------------------------------------------//
	static int			_generateGUID				( lua_State* L );
	static int			_getMACAddress				( lua_State* L );
	static int			_setValue					( lua_State* L );

	//----------------------------------------------------------------//
	void				SetValue					( lua_State* L );

public:	

	enum {
		EVENT_VALUE_CHANGED,
	};

	enum {
		CONNECTION_TYPE_NONE,
		CONNECTION_TYPE_WIFI,
		CONNECTION_TYPE_WWAN,
	};
	
	enum {	
		OS_BRAND_ANDROID,
		OS_BRAND_IOS,
		OS_BRAND_OSX,
		OS_BRAND_LINUX,
		OS_BRAND_WINDOWS,
		OS_BRAND_UNAVAILABLE
	};
	
	DECL_LUA_SINGLETON ( MOAIEnvironment )

	//----------------------------------------------------------------//
	void				DetectEnvironment			();
						MOAIEnvironment				();
						~MOAIEnvironment			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				SetValue					( cc8* key );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetValue ( cc8* key, TYPE value ) {
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		
		state.Push ( key );
		state.Push ( value );
		
		this->SetValue ( state );
	}
};

#endif
