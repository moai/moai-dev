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
*/
class MOAIEnvironment :
	public USGlobalClass < MOAIEnvironment, USLuaObject > {
private:
	STLString			mAppDisplayName;
	STLString			mAppID;
	STLString			mAppVersion;
	STLString			mCPUABI;
	STLString			mDevBrand;
	STLString			mDevName;
	STLString			mDevManufacturer;
	STLString			mDevModel;
	STLString			mDevProduct;
	STLString			mOSBrand;
	STLString			mOSVersion;
	STLString			mUDID;
	
	cc8* ( *getConnectivityFunc )( void );
	cc8* ( *getGUIDfunc ) ( void );
	int ( *getHeightFunc ) ( void );
	int ( *getWidthFunc ) ( void );
	

	//----------------------------------------------------------------//
	static int			_generateGUID			( lua_State* L );
	static int			_getAppDisplayName		( lua_State* L );
	static int			_getAppID				( lua_State* L );
	static int			_getAppVersion			( lua_State* L );
	static int			_getConnectionType 		( lua_State* L );
	static int			_getCPUABI				( lua_State* L );
	static int			_getDevBrand			( lua_State* L );
	static int			_getDevName				( lua_State* L );
	static int			_getDevManufacturer		( lua_State* L );
	static int			_getDevModel			( lua_State* L );
	static int			_getDevProduct			( lua_State* L );
	static int			_getOSBrand				( lua_State* L );
	static int			_getOSVersion			( lua_State* L );
	static int			_getUDID				( lua_State* L );
	static int			_getViewSize			( lua_State* L );
	
	
	//----------------------------------------------------------------//
	enum {
		CONNECTION_TYPE_NONE,
		CONNECTION_TYPE_WIFI,
		CONNECTION_TYPE_WWAN,
		OS_BRAND_ANDROID,
		OS_BRAND_IOS
	};

public:
	
	DECL_LUA_SINGLETON ( MOAIEnvironment )

	//----------------------------------------------------------------//
						MOAIEnvironment				( );
						~MOAIEnvironment				( );
	void				RegisterLuaClass			( USLuaState& state );
	void				SetAppDisplayName			( cc8* displayName );
	void				SetAppID					( cc8* appID );
	void				SetAppVersion				( cc8* appVersion );	
	void				SetConnectivityFunc			( cc8* (*connFunc) () );
	void				SetCPUABI					( cc8* abi );
	void				SetDevBrand					( cc8* devBrand );
	void				SetDevName					( cc8* devName );
	void				SetDevManufacturer			( cc8* devMan );
	void				SetDevModel					( cc8* devModel );
	void				SetDevProduct				( cc8* devProduct );
	void				SetGUIDFunc					( cc8* (*guidFunc) () );
	void				SetHeightFunc				( int (*heightFunc) () );
	void				SetOSBrand					( cc8* osBrand );
	void				SetOSVersion				( cc8* osVersion );
	void				SetUDID						( cc8* udid );
	void				SetWidthFunc				( int (*widthFunc) () );
	STLString			ToString					();
};

#endif