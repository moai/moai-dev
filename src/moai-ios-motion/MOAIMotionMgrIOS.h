// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOTIONMANAGERIOS_H
#define	MOAIMOTIONMANAGERIOS_H

#include <moai-core/headers.h>
#include <moai-sim/MOAIAction.h>

@class CLLocationManager;
@class CMMotionManager;
@class NSOperationQueue;

//================================================================//
// MOAIMotionMgrIOS
//================================================================//
class MOAIMotionMgrIOS :
	public MOAIGlobalClass < MOAIMotionMgrIOS, MOAIAction > {
private:

	bool				mIsHeadingActive;
	//bool				mIsLocationActive;

	u8					mLocationDeviceID;
	u8					mInputDeviceID;
	u8					mRawInputDeviceID;

	CLLocationManager*	mLocationMgr;
	CMMotionManager*	mMotionMgr;
	NSOperationQueue*	mOperationQueue;

	//----------------------------------------------------------------//
	static int	_init									( lua_State* L );
	static int	_isAccelerometerAvailable				( lua_State* L );
	static int	_isDeviceMotionAvailable				( lua_State* L );
	static int	_isGyroAvailable						( lua_State* L );
	static int	_isHeadingAvailable						( lua_State* L );
	static int	_isMagnetometerAvailable				( lua_State* L );
	static int	_setAccelerometerActive					( lua_State* L );
	static int	_setDeviceMotionActive					( lua_State* L );
	static int	_setGyroActive							( lua_State* L );
	static int	_setHeadingActive						( lua_State* L );
	static int	_setMagnetometerActive					( lua_State* L );

	//----------------------------------------------------------------//
	bool				IsDone							();
	void				OnUpdate						( double step );

public:
	
	DECL_LUA_SINGLETON ( MOAIMotionMgrIOS )
	
	//----------------------------------------------------------------//
	void				Init							();
						MOAIMotionMgrIOS				();
						~MOAIMotionMgrIOS				();
	void				RegisterLuaClass				( MOAILuaState& state );
};

#endif
