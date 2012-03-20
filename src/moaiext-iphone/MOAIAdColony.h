//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIADCOLONY_H
#define MOAIADCOLONY_H

#include <moaicore/moaicore.h>
#import <AdColonyPublic.h>

@class MoaiAdColonyTakeoverDelegate;

//================================================================//
// MOAIAdColony
//================================================================//
class MOAIAdColony :
public MOAIGlobalClass < MOAIAdColony, MOAILuaObject >,
public MOAIGlobalEventSource {
private:
    
	id < AdColonyDelegate >			mAdColonyDelegate;
	MoaiAdColonyTakeoverDelegate*	mTakeoverDelegate;
	
	//----------------------------------------------------------------//
	static int		_initAdColony                   ( lua_State* L );
	static int		_playVideo						( lua_State* L );
	static int		_videoReadyForZone				( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIAdColony );
	
	enum {
	
		VIDEO_BEGAN_IN_ZONE,
		VIDEO_ENDED_IN_ZONE,
		VIDEO_FAILED_IN_ZONE,
		VIDEO_PAUSED_IN_ZONE,
		VIDEO_RESUMED_IN_ZONE
	};
	
	void			FireZoneListenerEvent			( int event, cc8* zone );
                    MOAIAdColony					();
					~MOAIAdColony                   ();
	void			RegisterLuaClass				( MOAILuaState& state );
	void			SetAdColonyDelegate				( id < AdColonyDelegate > delegate );
};

//================================================================//
// MoaiAdColonyTakeoverDelegate
//================================================================//
@interface MoaiAdColonyTakeoverDelegate : NSObject < AdColonyTakeoverAdDelegate > {
@private
}
@end

#endif
