// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOBILEAPPTRACKERIOS_H
#define	MOAIMOBILEAPPTRACKERIOS_H

#import <moai-core/headers.h>
#import <MobileAppTracker/MobileAppTracker.h>

@class MOAIMobileAppTrackerDelegate;

//================================================================//
// MOAIMobileAppTrackerIOS
//================================================================//
/**	@lua	MOAIMobileAppTrackerIOS
	@text	Wrapper for WebAppTracker interface.
*/
class MOAIMobileAppTrackerIOS :
	public MOAIGlobalClass < MOAIMobileAppTrackerIOS, MOAILuaObject > {
private:

	MOAIMobileAppTrackerDelegate* mDelegate;

	//----------------------------------------------------------------//
	static int	_setDebugMode			( lua_State* L );
	static int	_setIFA					( lua_State* L );
	static int	_setIFV					( lua_State* L );
	static int	_setLocation			( lua_State* L );
	static int	_setMACAddress			( lua_State* L );
	static int	_setOpenUDID			( lua_State* L );
	static int	_setUserId				( lua_State* L );
	static int	_startTracker			( lua_State* L );
	static int	_trackAction			( lua_State* L );
	static int	_trackInstall			( lua_State* L );
	static int	_trackUpdate			( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIMobileAppTrackerIOS )
	
			MOAIMobileAppTrackerIOS			();
			~MOAIMobileAppTrackerIOS		();
	void	RegisterLuaClass				( MOAILuaState& state );
};

//================================================================//
// MOAIMobileAppTrackerDelegate
//================================================================//
@interface MOAIMobileAppTrackerDelegate : NSObject < MobileAppTrackerDelegate > {
@private
}
@end

#endif
