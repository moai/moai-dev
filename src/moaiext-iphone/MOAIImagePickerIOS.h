//
//  ImagePicker.h
//  SocialArcade
//
//  Created by Megan Peterson on 6/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef AKU_IMAGEPICKER_H
#define AKU_IMAGEPICKER_H

#include "MOAIImagePickerIOS.h"

#import <Foundation/Foundation.h>

#include <moaicore/moaicore.h>

@class MOAIImagePickerIOSDelegate;
@class MOAIImagePickerIOSTakeoverDelegate;

//================================================================//
// MOAIAdColonyIOS
//================================================================//
/**	@name	MOAIAdColonyIOS
 @text	Wrapper for AdColony integration on iOS devices. 
 Exposed to lua via MOAIAdColony on all mobile platforms.
 
 @const	VIDEO_BEGAN_IN_ZONE		Event code for video playback started notifications.
 @const	VIDEO_ENDED_IN_ZONE		Event code for video playback ended notifications.
 @const	VIDEO_FAILED_IN_ZONE	Event code for video playback failed notifications.
 @const	VIDEO_PAUSED_IN_ZONE	Event code for video playback paused notifications.
 @const	VIDEO_RESUMED_IN_ZONE	Event code for video playback resumed notifications.
 */
class MOAIImagePickerIOS :
public MOAIGlobalClass < MOAIImagePickerIOS, MOAILuaObject >,
public MOAIGlobalEventSource {
private:
    
	MOAIImagePickerIOSDelegate*			mImagePickerDelegate;
	MOAIImagePickerIOSTakeoverDelegate*	mTakeoverDelegate;
	
	//----------------------------------------------------------------//
	//static int	_getDeviceID		( lua_State* L );
	static int	_init				( lua_State* L );
	static cc8*	_luaParseTable 		( lua_State* L, int idx );
	//static int	_playVideo			( lua_State* L );
	//static int	_videoReadyForZone	( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIImagePickerIOS );
	

    
	//NSString * 		mAppId;
	//NSDictionary * 	mZones;
	
    MOAIImagePickerIOS				();
    ~MOAIImagePickerIOS             ();
	void	NotifyTakeoverEventOccurred	( int event, cc8* zone );
	void	RegisterLuaClass			( MOAILuaState& state );
};

//================================================================//
// MOAIAdColonyIOSDelegate
//================================================================//
//@interface MOAIImagePickerIOSDelegate : NSObject <UIImagePickerControllerDelegate> {
//@private
//}
//@end

//================================================================//
// MOAIAdColonyIOSTakeoverDelegate
//================================================================//
//@interface MOAIImagePickerIOSTakeoverDelegate : NSObject {
//@private
//}
//@end
#endif
