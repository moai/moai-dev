// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDEVICEINFO_H
#define	MOAIDEVICEINFO_H

//================================================================//
// MOAIDeviceInfo
//================================================================//
/**	@name	MOAIDeviceInfo
	@text	Holds info about the device.
*/
class MOAIDeviceInfo :
	public USGlobalClass < MOAIDeviceInfo, USLuaObject > {
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
	

	//----------------------------------------------------------------//
	static int			_generateUUID			( lua_State* L );
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

public:
	
	DECL_LUA_SINGLETON ( MOAIDeviceInfo )

	//----------------------------------------------------------------//
						MOAIDeviceInfo				( );
						~MOAIDeviceInfo				( );
	void				RegisterLuaClass			( USLuaState& state );
	void				SetAppDisplayName			( cc8* displayName );
	void				SetAppID					( cc8* appID );
	void				SetAppVersion				( cc8* appVersion );
	void				SetCPUABI					( cc8* abi );
	void				SetDevBrand					( cc8* devBrand );
	void				SetDevName					( cc8* devName );
	void				SetDevManufacturer			( cc8* devMan );
	void				SetDevModel					( cc8* devModel );
	void				SetDevProduct				( cc8* devProduct );
	void				SetOSBrand					( cc8* osBrand );
	void				SetOSVersion				( cc8* osVersion );
	void				SetUDID						( cc8* udid );
	STLString			ToString					();
};

#endif