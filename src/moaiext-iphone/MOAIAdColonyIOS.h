//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIADCOLONYIOS_H
#define MOAIADCOLONYIOS_H

#ifndef DISABLE_ADCOLONY

#include <moaicore/moaicore.h>

#import <AdColonyPublic.h>

@class MOAIAdColonyIOSDelegate;
@class MOAIAdColonyIOSTakeoverDelegate;

//================================================================//
// MOAIAdColonyIOS
//================================================================//
class MOAIAdColonyIOS :
public MOAIGlobalClass < MOAIAdColonyIOS, MOAILuaObject >,
public MOAIGlobalEventSource {
private:
    
	MOAIAdColonyIOSDelegate*			mAdColonyDelegate;
	MOAIAdColonyIOSTakeoverDelegate*	mTakeoverDelegate;
	
	//----------------------------------------------------------------//
	static int	_getDeviceID		( lua_State* L );
	static int	_init				( lua_State* L );
	static cc8*	_luaParseTable 		( lua_State* L, int idx );
	static int	_playVideo			( lua_State* L );
	static int	_videoReadyForZone	( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIAdColonyIOS );
	
	enum {
		VIDEO_BEGAN_IN_ZONE,
		VIDEO_ENDED_IN_ZONE,
		VIDEO_FAILED_IN_ZONE,
		VIDEO_PAUSED_IN_ZONE,
		VIDEO_RESUMED_IN_ZONE,
		TOTAL
	};

	NSString * 		mAppId;
	NSDictionary * 	mZones;
	
            MOAIAdColonyIOS				();
			~MOAIAdColonyIOS			();
	void	NotifyTakeoverEventOccurred	( int event, cc8* zone );
	void	RegisterLuaClass			( MOAILuaState& state );
};

//================================================================//
// MOAIAdColonyIOSDelegate
//================================================================//
@interface MOAIAdColonyIOSDelegate : NSObject < AdColonyDelegate > {
@private
}
@end

//================================================================//
// MOAIAdColonyIOSTakeoverDelegate
//================================================================//
@interface MOAIAdColonyIOSTakeoverDelegate : NSObject < AdColonyTakeoverAdDelegate > {
@private
}
@end

#endif // MOAIADCOLONY_H

#endif // DISABLE_ADCOLONY