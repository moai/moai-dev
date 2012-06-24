//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAIBURSTLYIOS_H
#define	MOAIBURSTLYIOS_H


#include <moaicore/moaicore.h>

/*
#import <Burstly.h>
#import "OAIAdManager.h"


@class MoaiChartBoostDelegate;

//================================================================//
// MOAIChartBoostIOS
//================================================================//
class MOAIBurstlyIOS :
	public MOAIGlobalClass < MOAIBurstlyIOS, MOAILuaObject > {
private:

	MoaiBurstlyDelegate*	mDelegate;

	//----------------------------------------------------------------//
	static int	_init 				( lua_State* L );
	static int	_loadInterstitial 	( lua_State* L );
	static int	_setListener		( lua_State* L );
	static int	_showInterstitial 	( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIBurstlyIOS );

	enum {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
		TOTAL
	};
	
	MOAILuaRef		mListeners [ TOTAL ];
	
					MOAIBurstlyIOS				();
					~MOAIBurstlyIOS				();
	void 			NotifyInterstitialDismissed		();
	void 			NotifyInterstitialLoadFailed	();
	void			RegisterLuaClass				( MOAILuaState& state );
};

//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@interface MoaiBurstlyDelegate : NSObject < OAIAdManagerDelegate > {
@private
}
@end

*/

#endif  //MOAIBURSTLYIOS_H
