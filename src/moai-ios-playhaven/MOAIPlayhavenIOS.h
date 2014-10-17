#ifndef	MOAIPLAYHAVENIOS_H
#define	MOAIPLAYHAVENIOS_H

#ifndef DISABLE_PLAYHAVEN

#include <moai-core/headers.h>
#import "PlayHavenSDK.h"

@class MOAIPlayhavenListener;

//================================================================//
// MOAIPlayhavenIOS
//================================================================//
class MOAIPlayhavenIOS :
	public MOAIGlobalClass < MOAIPlayhavenIOS, MOAILuaObject > {
private:
		static int _init( lua_State* L );
		static int _setListener(lua_State* L);
		static int _getContent( lua_State* L );
		static int _reportIAPSolution(lua_State* L);
	
public:

	DECL_LUA_SINGLETON ( MOAIPlayhavenIOS );
	
		enum {
			UNLOCKED_REWARD,
			SHOULD_MAKE_IAP,
			TOTAL,
		};
		
		MOAILuaStrongRef mListeners[TOTAL];
		
		NSString* _token;
		NSString* _secretKey;
		
		MOAIPlayhavenListener *phListener;
		PHPurchase *phPurchase;

	MOAIPlayhavenIOS();
	~MOAIPlayhavenIOS();
	void RegisterLuaClass( MOAILuaState& state );
	void notifyUnlockedReward(cc8* id, int qty);
	void notifyShouldMakeIAP(cc8* id);
};


@interface MOAIPlayhavenListener : NSObject<PHPublisherContentRequestDelegate> {	
}
@end

#endif  //DISABLE_PLAYHAVEN

#endif  //MOAIPLAYHAVENIOS_H