//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAICHARTBOOSTIOS_H
#define	MOAICHARTBOOSTIOS_H

#ifndef DISABLE_CHARTBOOST

#include <moaicore/moaicore.h>
#import <ChartBoost.h>

@class MoaiChartBoostDelegate;

//================================================================//
// MOAIChartBoostIOS
//================================================================//
class MOAIChartBoostIOS :
	public MOAIGlobalClass < MOAIChartBoostIOS, MOAILuaObject > {
private:

	MoaiChartBoostDelegate*	mDelegate;

	//----------------------------------------------------------------//
	static int	_hasCachedInterstitial	( lua_State* L );
	static int	_init					( lua_State* L );
	static int	_loadInterstitial		( lua_State* L );
	static int	_setListener			( lua_State* L );
	static int	_showInterstitial		( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIChartBoostIOS );

	enum {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
		TOTAL
	};
	
	MOAILuaRef		mListeners [ TOTAL ];
	
					MOAIChartBoostIOS				();
					~MOAIChartBoostIOS				();
	void 			NotifyInterstitialDismissed		();
	void 			NotifyInterstitialLoadFailed	();
	void			RegisterLuaClass				( MOAILuaState& state );
};

//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@interface MoaiChartBoostDelegate : NSObject < ChartBoostDelegate > {
@private
}
@end

#endif  //DISABLE_CHARTBOOST

#endif  //MOAICHARTBOOSTIOS_H
