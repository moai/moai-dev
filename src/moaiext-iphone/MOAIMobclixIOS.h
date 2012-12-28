// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOBCLIXIOS_H
#define	MOAIMOBCLIXIOS_H

#ifndef DISABLE_MOBCLIX

#import <Foundation/Foundation.h> 
#import <moaicore/moaicore.h>

#import <UIKit/UIKit.h>
#import "MobclixAds.h"

//================================================================//
// MOAIMobclixIOS
//================================================================//
/**	@name	MOAIMobclixIOS
	@text	Wrapper for Crittercism integration on iOS devices.
			Crittercism provides real-time, actionable crash reports 
			for mobile apps. Exposed to lua via MOAICrittercism on
			all mobile platforms.
*/

@class MoaiMobclixDelegate;

class MOAIMobclixIOS :
	public MOAIGlobalClass < MOAIMobclixIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init 				( lua_State* L );
	static int	_showAds            ( lua_State* L );
	static int	_hideAds            ( lua_State* L );
	static int	_dealloc			( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIMobclixIOS );
	
			MoaiMobclixDelegate*	mMobclixDelgate;
			MobclixAdView*			mAdView;
		
			MOAIMobclixIOS		();
			~MOAIMobclixIOS		();
	void	RegisterLuaClass		( MOAILuaState& state );
};

//================================================================//
// MoaiMobclixDelegate
//================================================================//
@interface MoaiMobclixDelegate : NSObject < MobclixAdViewDelegate > {
@private
}

@property(nonatomic,retain) MobclixAdView* mAdView;

@end


#endif  //DISABLE_MOBCLIX

#endif  //MOAIMOBCLIXIOS_H
