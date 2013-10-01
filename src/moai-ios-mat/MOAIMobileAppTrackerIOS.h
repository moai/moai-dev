// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOBILEAPPTRACKERIOS_H
#define	MOAIMOBILEAPPTRACKERIOS_H

#ifndef DISABLE_MOBILEAPPTRACKER

#import <moai-core/headers.h>

//================================================================//
// MOAIMobileAppTrackerIOS
//================================================================//
/**	@name	MOAIMobileAppTrackerIOS
	@text	Wrapper for WebAppTracker interface.
*/
class MOAIMobileAppTrackerIOS :
	public MOAIGlobalClass < MOAIMobileAppTrackerIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_trackInstall			( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIMobileAppTrackerIOS )
	
			MOAIMobileAppTrackerIOS			();
			~MOAIMobileAppTrackerIOS		();
	void	RegisterLuaClass	( MOAILuaState& state );
};

#endif // DISABLE_MOBILEAPPTRACKER

#endif // MOAIMOBILEAPPTRACKERIOS_H
