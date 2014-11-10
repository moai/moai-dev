// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOTIONMANAGERIOS_H
#define	MOAIMOTIONMANAGERIOS_H

#include <moai-core/headers.h>
#include <moai-sim/MOAIInputQueue.h>

@class CMMotionManager;
@class NSOperationQueue;

//================================================================//
// MOAIMotionMgrIOS
//================================================================//
class MOAIMotionMgrIOS :
	public MOAIGlobalClass < MOAIMotionMgrIOS, MOAIInputQueue > {
private:

	CMMotionManager*	mMotionMgr;
	NSOperationQueue*	mOperationQueue;

	//----------------------------------------------------------------//
	static int	_init									( lua_State* L );
	static int	_isAccelerometerAvailable				( lua_State* L );
	static int	_isDeviceMotionAvailable				( lua_State* L );
	static int	_isGyroAvailable						( lua_State* L );
	static int	_isMagnetometerAvailable				( lua_State* L );
	static int	_setAccelerometerActive					( lua_State* L );
	static int	_setAccelerometerUpdateInterval			( lua_State* L );
	static int	_setGyroActive							( lua_State* L );
	static int	_setGyroUpdateInterval					( lua_State* L );
	static int	_setDeviceMotionActive					( lua_State* L );
	static int	_setDeviceMotionUpdateInterval			( lua_State* L );
	static int	_setMagnetometerActive					( lua_State* L );
	static int	_setMagnetometerUpdateInterval			( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIMotionMgrIOS )
	
	//----------------------------------------------------------------//
	void				Init							();
						MOAIMotionMgrIOS			();
						~MOAIMotionMgrIOS			();
	void				RegisterLuaClass				( MOAILuaState& state );
};

#endif
