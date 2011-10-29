// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIENVIRONMENT_H
#define	MOAIENVIRONMENT_H

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
	public USGlobalClass < MOAIEnvironment, USLuaObject > {
private:
	STLString			mAppDisplayName;
	STLString			mAppID;
	STLString			mAppVersion;
	STLString			mCacheDirectory;
	STLString			mCarrierISOCountryCode;
	STLString			mCarrierMobileCountryCode;
	STLString			mCarrierMobileNetworkCode;
	STLString			mCarrierName;
	long				mConnectionType;
	STLString			mCountryCode;
	STLString			mCPUABI;
	STLString			mDevBrand;
	STLString			mDevName;
	STLString			mDevManufacturer;
	STLString			mDevModel;
	STLString			mDevProduct;
	STLString			mDocumentDirectory;
	bool				mIsRetinaDisplay;
	STLString			mLanguageCode;
	STLString			mOSBrand;
	STLString			mOSVersion;
	STLString			mResourceDirectory;
	STLString			mUDID;
	long				mScreenWidth;
	long				mScreenHeight;
	
	long ( *getConnectivityFunc )( void );
	cc8* ( *getGUIDfunc ) ( void );
	

	//----------------------------------------------------------------//
	static int			_generateGUID					( lua_State* L );
	static int			_getAppDisplayName				( lua_State* L );
	static int			_getAppID						( lua_State* L );
	static int			_getAppVersion					( lua_State* L );
	static int			_getCacheDirectory				( lua_State* L );
	static int			_getCarrierISOCountryCode		( lua_State* L );
	static int			_getCarrierMobileCountryCode	( lua_State* L );
	static int			_getCarrierMobileNetworkCode	( lua_State* L );
	static int			_getCarrierName					( lua_State* L );
	static int			_getConnectionType				( lua_State* L );
	static int			_getCountryCode					( lua_State* L );
	static int			_getCPUABI						( lua_State* L );
	static int			_getDevBrand					( lua_State* L );
	static int			_getDevName						( lua_State* L );
	static int			_getDevManufacturer				( lua_State* L );
	static int			_getDevModel					( lua_State* L );
	static int			_getDevProduct					( lua_State* L );
	static int			_getDocumentDirectory			( lua_State* L );
	static int			_getLanguageCode				( lua_State* L );
	static int			_getOSBrand						( lua_State* L );
	static int			_getOSVersion					( lua_State* L );
	static int			_getResourceDirectory			( lua_State* L );
	static int			_getUDID						( lua_State* L );
	static int			_getViewSize					( lua_State* L );
	static int			_getDeviceSize					( lua_State* L );
	static int			_isRetinaDisplay				( lua_State* L );

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
	void				RegisterLuaClass			( USLuaState& state );
	void				SetAppDisplayName			( cc8* displayName );
	void				SetAppID					( cc8* appID );
	void				SetAppVersion				( cc8* appVersion );
	void				SetCacheDirectory			( cc8* cachDir );
	void				SetCarrierISOCountryCode	( cc8* isoCode );
	void				SetCarrierMobileCountryCode	( cc8* mobCountryCode );
	void				SetCarrierMobileNetworkCode	( cc8* mobNetworkCode );
	void				SetCarrierName				( cc8* name );
	void				SetConnectivityFunc			( long (*connFunc) () );
	void				SetConnectionType			( long connType );
	void				SetCountryCode				( cc8* countryCode );
	void				SetCPUABI					( cc8* abi );
	void				SetDevBrand					( cc8* devBrand );
	void				SetDevName					( cc8* devName );
	void				SetDevManufacturer			( cc8* devMan );
	void				SetDevModel					( cc8* devModel );
	void				SetDevProduct				( cc8* devProduct );
	void				SetDocumentDirectory		( cc8* docDir );
	void				SetIsRetinaDisplay			( bool );
	void				SetGUIDFunc					( cc8* (*guidFunc) () );
	void				SetLanguageCode				( cc8* langCode );
	void				SetOSBrand					( cc8* osBrand );
	void				SetOSVersion				( cc8* osVersion );
	void				SetResourceDirectory		( cc8* resDir );
	void				SetUDID						( cc8* udid );
	void				SetScreenSize				( long width, long height );
};

#endif
