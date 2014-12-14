#ifndef DISABLE_PLAYHAVEN

#import "MOAIPlayhavenIOS.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize Playhaven.
	
	@in		string	appId			Available in Playhaven dashboard settings.
	@in 	string	appSignature	Available in Playhaven dashboard settings.
	@out 	nil
*/
int MOAIPlayhavenIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* token = lua_tostring ( state, 1 );
	cc8* secretKey = lua_tostring ( state, 2 );
	
	MOAIPlayhavenIOS::Get()._token = [NSString stringWithUTF8String:token];
	MOAIPlayhavenIOS::Get()._secretKey = [NSString stringWithUTF8String:secretKey];
	
	PHPublisherOpenRequest *request = [PHPublisherOpenRequest requestForApp:MOAIPlayhavenIOS::Get()._token secret:MOAIPlayhavenIOS::Get()._secretKey];
	[request send];
	
	return 0;
}

int MOAIPlayhavenIOS::_setListener(lua_State* L) {
	MOAILuaState state(L);
	
	u32 idx = state.GetValue<u32>(1, TOTAL);
	
	if (idx < TOTAL) {
		MOAIPlayhavenIOS::Get().mListeners[idx].SetRef(state, 2);
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getContent
	@text	Shows Playhaven content.
 
	@in		string	placement	Placement position.
	@out 	nil
*/
int MOAIPlayhavenIOS::_getContent(lua_State* L) {
	MOAILuaState state ( L );
	
	cc8* placement = lua_tostring(state, 1);
	
	PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:MOAIPlayhavenIOS::Get()._token secret:MOAIPlayhavenIOS::Get()._secretKey placement:[NSString stringWithUTF8String:placement] delegate:MOAIPlayhavenIOS::Get().phListener];
	[request send];
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reportIAPSolution
	@text	Reports IAP resolution status.

	@in		number	status	Status code.
	@out 	nil
*/
int MOAIPlayhavenIOS::_reportIAPSolution(lua_State* L) {
	MOAILuaState state ( L );
	
	int status = lua_tointeger(state, 1);
	
	if (MOAIPlayhavenIOS::Get().phPurchase) {
		PHPurchaseResolutionType resolution = PHPurchaseResolutionBuy;
		if (status > 0) {
			resolution = PHPurchaseResolutionCancel;
		} else if (status < 0) {
			resolution = PHPurchaseResolutionError;
		}
		[MOAIPlayhavenIOS::Get().phPurchase reportResolution:resolution];
		
		[MOAIPlayhavenIOS::Get().phPurchase release];
		MOAIPlayhavenIOS::Get().phPurchase = nil;
	}
	
	return 0;
}

//================================================================//
// MOAIPlayhavenIOS
//================================================================//

//----------------------------------------------------------------//
MOAIPlayhavenIOS::MOAIPlayhavenIOS () {
	RTTI_SINGLE ( MOAILuaObject )
	
	phListener = [[MOAIPlayhavenListener alloc] init];
}

//----------------------------------------------------------------//
MOAIPlayhavenIOS::~MOAIPlayhavenIOS () {
	
}

//----------------------------------------------------------------//
void MOAIPlayhavenIOS::RegisterLuaClass ( MOAILuaState& state ) {
	state.SetField(-1,"UNLOCKED_REWARD", (u32)UNLOCKED_REWARD);
	state.SetField(-1,"SHOULD_MAKE_IAP", (u32)SHOULD_MAKE_IAP);
	
	luaL_Reg regTable [] = {
		{"init", _init},
		{"setListener", _setListener},
		{"getContent", _getContent},
		{"reportIAPSolution", _reportIAPSolution},
		{NULL, NULL}
	};

	luaL_register ( state, 0, regTable );
}

void MOAIPlayhavenIOS::notifyUnlockedReward(cc8* id, int qty) {
	MOAILuaRef& callback = this->mListeners[UNLOCKED_REWARD];
	
	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();
		lua_pushstring(state, id);
		lua_pushinteger(state, qty);
		state.DebugCall(2, 0);
	}
}

void MOAIPlayhavenIOS::notifyShouldMakeIAP(cc8* id) {
	MOAILuaRef& callback = this->mListeners[SHOULD_MAKE_IAP];
	
	if (callback) {
		MOAIScopedLuaState state = callback.GetSelf();
		lua_pushstring(state, id);
		state.DebugCall(1, 0);
	}
}


@implementation MOAIPlayhavenListener

/*
-(void)requestWillGetContent:(PHPublisherContentRequest *)request {
	
}

-(void)requestDidGetContent:(PHPublisherContentRequest *)request {
	
}

-(void)request:(PHPublisherContentRequest *)request contentWillDisplay:(PHContent *)content {
	
}

-(void)request:(PHPublisherContentRequest *)request contentDidDisplay:(PHContent *)content {
	
}

-(void)request:(PHPublisherContentRequest *)request contentDidDismissWithType:(PHPublisherContentDismissType *)type {
	
}

-(void)request:(PHPublisherContentRequest *)request didFailWithError:(NSError *)error {
	
}
*/
-(void)request:(PHPublisherContentRequest *)request unlockedReward:(PHReward *)reward {
	UNUSED ( request );
	MOAIPlayhavenIOS::Get().notifyUnlockedReward([reward.name UTF8String], reward.quantity);
}

-(void)request:(PHPublisherContentRequest *)request makePurchase:(PHPurchase *)purchase {
	UNUSED ( request );
	MOAIPlayhavenIOS::Get().phPurchase = purchase;
	MOAIPlayhavenIOS::Get().notifyShouldMakeIAP([purchase.productIdentifier UTF8String]);
}

@end

#endif