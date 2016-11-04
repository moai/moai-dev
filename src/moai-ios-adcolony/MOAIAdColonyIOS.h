//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIADCOLONYIOS_H
#define MOAIADCOLONYIOS_H

#include <moai-core/headers.h>

#import <AdColony/AdColony.h>

@class MOAIAdColonyIOSDelegate;
@class MOAIAdColonyIOSTakeoverDelegate;

//================================================================//
// MOAIAdColonyIOS
//================================================================//
class MOAIAdColonyIOS :
public ZLContextClass < MOAIAdColonyIOS, MOAILuaObject >,
public MOAIGlobalEventSource {
private:
	
	MOAIAdColonyIOSDelegate*			mAdColonyDelegate;
	
	//----------------------------------------------------------------//
	static int		_init					( lua_State* L );
	static int		_playVideo				( lua_State* L );
	static int		_videoReadyForZone		( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIAdColonyIOS );
	
	enum {
		VIDEO_STARTED,
		VIDEO_SHOWN,
		VIDEO_FAILED,
	};
	
	//----------------------------------------------------------------//
					MOAIAdColonyIOS					();
					~MOAIAdColonyIOS				();
	void			RegisterLuaClass				( MOAILuaState& state );
};

//================================================================//
// MOAIAdColonyIOSDelegate
//================================================================//
@interface MOAIAdColonyIOSDelegate : NSObject < AdColonyDelegate, AdColonyAdDelegate > {
@private
}
@end

#endif // MOAIADCOLONYIOS_H
