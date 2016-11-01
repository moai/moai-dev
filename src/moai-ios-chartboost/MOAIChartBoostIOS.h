//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAICHARTBOOSTIOS_H
#define	MOAICHARTBOOSTIOS_H

#include <moai-core/headers.h>
#import <Chartboost.h>

@class MoaiChartBoostDelegate;

//================================================================//
// MOAIChartBoostIOS
//================================================================//
class MOAIChartBoostIOS :
	public ZLContextClass < MOAIChartBoostIOS, MOAIGlobalEventSource > {
private:

	MoaiChartBoostDelegate*	mDelegate;

	//----------------------------------------------------------------//
	static int		_cacheInterstitial				( lua_State* L );
	static int		_hasCachedInterstitial			( lua_State* L );
	static int		_init							( lua_State* L );
	static int		_showInterstitial				( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIChartBoostIOS );

	enum {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
		TOTAL
	};
	
	//----------------------------------------------------------------//
					MOAIChartBoostIOS				();
					~MOAIChartBoostIOS				();
	void 			NotifyInterstitialDismissed		();
	void 			NotifyInterstitialLoadFailed	();
	void			RegisterLuaClass				( MOAILuaState& state );
};

//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@interface MoaiChartBoostDelegate : NSObject < ChartboostDelegate > {
@private
}
@end

#endif  //MOAICHARTBOOSTIOS_H
