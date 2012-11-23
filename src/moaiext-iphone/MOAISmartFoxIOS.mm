//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MOAISmartFoxIOS.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//
int MOAISmartFoxIOS::_init( lua_State* L )
{
 	/**
     * Create a connection to SmartFoxServer as soon as the application has launched.
     * First, allocate a SmartFox2XClient instance
     */
	MOAISmartFoxIOS::Get ().mSmartFox = [[SmartFox2XClient alloc] initSmartFoxWithDebugMode:YES delegate:MOAISmartFoxIOS::Get ().mSmartFoxDelgate];
	
    /*
     * Load our XML config file and once done, attempt a connection
     */
	[MOAISmartFoxIOS::Get ().mSmartFox loadConfig:@"config.xml" connectOnSuccess:YES];
    
}

int MOAISmartFoxIOS::_sendJoinRoomRequest( lua_State* L )
{
    MOAILuaState state ( L );
	
	NSString* roomId = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	
	[MOAISmartFoxIOS::Get ().mSmartFox send:[JoinRoomRequest requestWithId:roomId]];
}


int MOAISmartFoxIOS::_login( lua_State* L )
{
    MOAILuaState state ( L );
	
	NSString* loginName = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	
	NSString* password = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 2, "" ) ];
	
	NSString* zone = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 3, "" ) ];
    
	[MOAISmartFoxIOS::Get ().mSmartFox send:[LoginRequest requestWithUserName:loginName password:password zoneName:zone params:nil]];

}
     
int MOAISmartFoxIOS::_sendPublicMessageRequest( lua_State* L )
{
    MOAILuaState state ( L );
    
    NSString* message = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	//NSString* room = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 2, "" ) ];
    
    [MOAISmartFoxIOS::Get ().mSmartFox send:[PublicMessageRequest requestWithMessage:message params:nil targetRoom:nil]];
 }


int MOAISmartFoxIOS::_sendRoomVariablesRequest(lua_State* L ) {

	MOAILuaState state ( L );

	NSString* message = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];

	NSArray * roomVariables = nil;
	
	[MOAISmartFoxIOS::Get ().mSmartFox send:[SetRoomVariablesRequest requestWithRoomVariables:roomVariables room:nil]];

}

int MOAISmartFoxIOS::_sendQuickJoinGameRequest(lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* varName = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	int matchOperator = state.GetValue < int >( 2, 0 );	

	MatchExpression *exp = nil;
	
	if (matchOperator == BOOL_MATCH_EQUALS) {
		
		Boolean value = state.GetValue < Boolean >( 3, false );
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[BoolMatch boolMatchEquals] value:[NSNumber numberWithBool:value]];
	}
	else if (matchOperator == BOOL_MATCH_NOT_EQUALS) {

		Boolean value = state.GetValue < Boolean >( 3, false );
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[BoolMatch boolMatchEquals] value:[NSNumber numberWithBool:value]];
		
	}
	else if (matchOperator == NUMBER_MATCH_EQUALS){
		
		int value = state.GetValue < int >( 3, 0 );
		
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[NumberMatch numberMatchEquals] value:[NSNumber numberWithInt:value]];
		
	}
	else if (matchOperator == NUMBER_MATCH_NOT_EQUALS){

			int value = state.GetValue < int >( 3, 0 );
			MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[NumberMatch numberMatchNotEquals] value:[NSNumber numberWithInt:value]];
	}
	else if (matchOperator == NUMBER_MATCH_GREATER_THAN){

		int value = state.GetValue < int >( 3, 0 );
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[NumberMatch numberMatchGreaterThan] value:[NSNumber numberWithInt:value]];
	}
	else if (matchOperator == NUMBER_MATCH_GREATER_THAN_OR_EQUAL_TO){
		int value = state.GetValue < int >( 3, 0 );
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[NumberMatch numberMatchGreaterThanOrEqualTo] value:[NSNumber numberWithInt:value]];
	}
	else if (matchOperator == NUMBER_MATCH_LESS_THAN){
		
		int value = state.GetValue < int >( 3, 0 );
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[NumberMatch numberMatchLessThan] value:[NSNumber numberWithInt:value]];
		
	}
	else if (matchOperator == NUMBER_MATCH_LESS_THAN_OR_EQUAL_TO){

		int value = state.GetValue < int >( 3, 0 );
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[NumberMatch numberMatchLessThanOrEqualTo] value:[NSNumber numberWithInt:value]];
		
	}
	else if (matchOperator == STRING_MATCH_EQUALS){

		NSString* value = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 3, "" ) ];
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[StringMatch stringMatchEquals] value:value];
		
	}
	else if (matchOperator == STRING_MATCH_NOT_EQUALS){

		NSString* value = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 3, "" ) ];
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[StringMatch stringMatchNotEquals] value:value];
		
	}
	else if (matchOperator == STRING_MATCH_CONTAINS){

		NSString* value = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 3, "" ) ];
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[StringMatch stringMatchContains] value:value];
		
	}
	else if (matchOperator == STRING_MATCH_STARTS_WITH){

		NSString* value = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 3, "" ) ];
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[StringMatch stringMatchStartsWith] value:value];
		
	}
	else if (matchOperator == STRING_MATCH_ENDS_WITH){

		NSString* value = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 3, "" ) ];
		MatchExpression *exp = [MatchExpression expressionWithVarName:varName condition:[StringMatch stringMatchEndsWith] value:value];
	}
	else {
		
		NSLog(@"invalid match type %i", matchOperator);
	}
	
	if (exp == nil)
	{
		// error here
		NSLog(@"no valid match expression %i", matchOperator);
		
	}
	
	// todo if we need more than one condition
	// exp = [exp and:@"country" condition:[StringMatch stringMatchEquals] value:@"Italy"];
	
	NSString* groupName = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 4, "" ) ];
	
	NSArray * roomGroupNames = [[NSArray alloc] init];
	[roomGroupNames addObject:groupName];
	
	[MOAISmartFoxIOS::Get ().mSmartFox send:[QuickJoinGameRequest requestWithMatchExpression:exp whereToSearch:roomGroupNames roomToLeave:nil]];	
}



int MOAISmartFoxIOS::_sendUserVariablesRequest(lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* message = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	//NSString* room = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 2, "" ) ];
	
	NSArray * userVariables = nil;
	
	[MOAISmartFoxIOS::Get ().mSmartFox send:[SetUserVariablesRequest requestWithUserVariables:userVariables room:nil]];
	
}

int MOAISmartFoxIOS::_sendObjectMessageRequest(lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSString* message = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 1, "" ) ];
	//NSString* room = [[ NSString alloc ] initWithUTF8String:state.GetValue < cc8* >( 2, "" ) ];
	
	NSArray * obj = nil;
	
	[MOAISmartFoxIOS::Get ().mSmartFox send:[ObjectMessageRequest requestWithObject:obj room:nil recipients:nil]];
	
}

//================================================================//
// MOAISmartFoxIOS
//================================================================//

//----------------------------------------------------------------//
MOAISmartFoxIOS::MOAISmartFoxIOS () {
    
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )	

	mSmartFoxDelgate = [[ MoaiSmartFoxDelegate alloc ] init];
	
}

//----------------------------------------------------------------//
MOAISmartFoxIOS::~MOAISmartFoxIOS () {
    
    [ mSmartFoxDelgate release];    
}

//----------------------------------------------------------------//
void MOAISmartFoxIOS::RegisterLuaClass ( MOAILuaState& state ) {
    
	state.SetField ( -1, "ON_CONNECTION",       ( u32 )ON_CONNECTION );
	state.SetField ( -1, "ON_CONNECTION_LOST",	( u32 )ON_CONNECTION_LOST );
	state.SetField ( -1, "ON_LOGIN",            ( u32 )ON_LOGIN );
	state.SetField ( -1, "ON_ROOM_JOIN", 		( u32 )ON_ROOM_JOIN );
	state.SetField ( -1, "ON_ROOM_JOIN_ERROR", 	( u32 )ON_ROOM_JOIN_ERROR );
	state.SetField ( -1, "ON_USER_ENTER_ROOM",	( u32 )ON_USER_ENTER_ROOM );
	state.SetField ( -1, "ON_USER_EXIT_ROOM",	( u32 )ON_USER_EXIT_ROOM );
	state.SetField ( -1, "ON_USER_COUNT_CHANGE", ( u32 )ON_USER_COUNT_CHANGE );
	state.SetField ( -1, "ON_ROOM_ADD",         ( u32 )ON_ROOM_ADD );
	state.SetField ( -1, "ON_ROOM_REMOVE",		( u32 )ON_ROOM_REMOVE );
	state.SetField ( -1, "ON_PUBLIC_MESSAGE",	( u32 )ON_PUBLIC_MESSAGE );
	state.SetField ( -1, "ON_PRIVATE_MESSAGE",	( u32 )ON_PRIVATE_MESSAGE );
	state.SetField ( -1, "ON_ROOM_CREATION_ERROR",	( u32 )ON_ROOM_CREATION_ERROR );
	state.SetField ( -1, "ON_ROOM_VARIABLES_UDATE",	( u32 )ON_ROOM_VARIABLES_UDATE );
	state.SetField ( -1, "ON_OBJECT_MESSAGE",	( u32 )ON_OBJECT_MESSAGE );
	
	state.SetField ( -1, "BOOL_MATCH_EQUALS",		( u32 )BOOL_MATCH_EQUALS );
	state.SetField ( -1, "BOOL_MATCH_NOT_EQUALS",	( u32 )BOOL_MATCH_NOT_EQUALS );
	state.SetField ( -1, "NUMBER_MATCH_EQUALS",	( u32 )NUMBER_MATCH_EQUALS );
	state.SetField ( -1, "NUMBER_MATCH_NOT_EQUALS",		( u32 )NUMBER_MATCH_NOT_EQUALS );
	state.SetField ( -1, "NUMBER_MATCH_GREATER_THAN",	( u32 )NUMBER_MATCH_GREATER_THAN );
	state.SetField ( -1, "NUMBER_MATCH_GREATER_THAN_OR_EQUAL_TO",	( u32 )NUMBER_MATCH_GREATER_THAN_OR_EQUAL_TO );
	state.SetField ( -1, "NUMBER_MATCH_LESS_THAN",		( u32 )NUMBER_MATCH_LESS_THAN );
	state.SetField ( -1, "NUMBER_MATCH_LESS_THAN_OR_EQUAL_TO",	( u32 )NUMBER_MATCH_LESS_THAN_OR_EQUAL_TO );
	state.SetField ( -1, "STRING_MATCH_EQUALS",	( u32 )STRING_MATCH_EQUALS );
	state.SetField ( -1, "STRING_MATCH_NOT_EQUALS",		( u32 )STRING_MATCH_NOT_EQUALS );
	state.SetField ( -1, "STRING_MATCH_CONTAINS",	( u32 )STRING_MATCH_CONTAINS );
	state.SetField ( -1, "STRING_MATCH_STARTS_WITH",	( u32 )STRING_MATCH_STARTS_WITH );
	state.SetField ( -1, "STRING_MATCH_ENDS_WITH",	( u32 )STRING_MATCH_ENDS_WITH );
	
	
	luaL_Reg regTable[] = {
		{ "init",					_init },
		{ "login",					_login },
		{ "sendPublicMessageRequest",		_sendPublicMessageRequest},
		{ "sendJoinRoomRequest",	_sendJoinRoomRequest},		
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAISmartFoxIOS > },
		{ "sendRoomVariablesRequest",      _sendRoomVariablesRequest},
		{ "sendUserVariablesRequest",      _sendUserVariablesRequest},
		{ "sendObjectMessageRequest", _sendObjectMessageRequest},
		{ "sendQuickJoinGameRequest", _sendQuickJoinGameRequest},		
		{ NULL, NULL }	
	};
    
	luaL_register ( state, 0, regTable );
}


//-------------------------------------------------------------
void MOAISmartFoxIOS::Connection(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	if ( this->PushListener ( ON_CONNECTION, state )) {

        if ([[evt.params objectForKey:@"success"] boolValue])
        {
            NSLog(@"Connection was established");
        }
        else
        {
            NSLog(@"Connection failed");
        }
        
        state.Push ( [[evt.params objectForKey:@"success"] boolValue] );

		state.DebugCall ( 1, 0 );
	}
	
}

void MOAISmartFoxIOS::ConnectionLost(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_CONNECTION_LOST, state )) {
		
        NSLog(@"Connection was lost, Reason: %@", [evt.params objectForKey:@"reason"]);
    
        NSString* reason = [evt.params objectForKey:@"reason"];

        state.Push ( [ reason UTF8String ] );
		state.DebugCall ( 1, 0 );
	}
	
}

void MOAISmartFoxIOS::Login(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_LOGIN, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::LoginError(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_LOGIN_ERROR, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::RoomJoin(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_JOIN, state )) {
	
		lua_newtable ( state );
		
		SFSRoom *room = [evt.params objectForKey:@"room"];
		NSLog(@"The Room %@ was successfully joined!", room.name);
		
		NSArray* players = [room userList];
		
		if ( [room isGame] )
		{
			NSLog(@"is game");
		}
		else{
			NSLog(@"is not game");
		}
		
		state.SetField ( -1, "name", [ room.name UTF8String ]);
		state.SetField ( -1, "groupId", [ room.groupId	UTF8String ]);
				
		NSLog(@"about to count players %i", [room userCount]);
		
		lua_pushstring ( state, "userList" );
		lua_newtable ( state );
		
		// interate through the player list
		int count = 1;
		for ( SFSUser* player in players ) {
				
			lua_pushnumber ( state, count++ );
			lua_newtable ( state );
		
			//state.SetField ( -1, "id", [ player  ]);
			//state.SetField ( -1, "playerId", [ player.playerId UTF8String ]);

			state.SetField ( -1, "name", [ player.name UTF8String ]);
			state.SetField ( -1, "id",  player.id  );
			state.SetField ( -1, "isItMe", player.isItMe);
			
			//state.SetField ( -1, "localizedDescription", [ product.localizedDescription UTF8String ]);
			//state.SetField ( -1, "price", [ product.price floatValue ]);
			//state.SetField ( -1, "localizedPrice", [ formattedString UTF8String]);
			//state.SetField ( -1, "priceLocale", [ product.priceLocale.localeIdentifier UTF8String ]);
			//state.SetField ( -1, "productIdentifier", [ product.productIdentifier UTF8String ]);
			
			printf("set table");
			
			lua_settable ( state, -3 );
			
		}
		
		lua_settable ( state, -3 );
		
		NSArray* variables = [room getVariables];
		
		int count2 = 1;
		for ( SFSRoomVariable* variable in variables ) {
			
            if (variable.type == "String"){
            
				state.SetField ( -1, [variable.name UTF8String ], [variable getStringValue]);
            }

			if (variable.type == "Int"){
				
				state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
            }
            
			if (variable.type == "Bool"){
				
				state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
            }
			
			if (variable.type == "Double"){
				
				state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
            }
            
			lua_settable ( state, -3 );
		}
		
		NSLog(@"calling back with count %i", count);
		
		state.DebugCall ( 1, 0 );
	}
    
}

void MOAISmartFoxIOS::RoomJoinError(SFSEvent *evt)
{

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	// get all the users from the
	if ( this->PushListener ( ON_ROOM_JOIN_ERROR, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::UserEnterRoom(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	if ( this->PushListener ( ON_USER_ENTER_ROOM, state )) {
	
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::UserExitRoom(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_USER_EXIT_ROOM, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::UserCountChange(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_USER_COUNT_CHANGE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::RoomAdd(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_ADD, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::RoomRemove(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_REMOVE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}
  
void MOAISmartFoxIOS::PublicMessage(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	SFSUser *sender = [evt.params objectForKey:@"sender"];
    NSLog(@"%@: %@", sender.name, [evt.params objectForKey:@"message"]);
	
	NSString* name = sender.name;
	NSString* message = [evt.params objectForKey:@"message"];
	
	if ( this->PushListener ( ON_PUBLIC_MESSAGE, state )) {
		
		state.Push ( [ name UTF8String ] );
		state.Push ( [ message UTF8String] );
		state.DebugCall ( 2, 0 );
	}
}

void MOAISmartFoxIOS::PrivateMessage(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	SFSUser *sender = [evt.params objectForKey:@"sender"];
    NSLog(@"%@: %@", sender.name, [evt.params objectForKey:@"message"]);
	
	NSString* name = sender.name;
	NSString* message = [evt.params objectForKey:@"message"];
	
	
	if ( this->PushListener ( ON_PRIVATE_MESSAGE, state )) {
		
		state.Push ( [ name UTF8String ] );
		state.Push ( [ message UTF8String] );
		state.DebugCall ( 2, 0 );
	}
}

void MOAISmartFoxIOS::RoomCreationError(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_CREATION_ERROR, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::RoomVariablesUpdate(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_VARIABLES_UDATE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxIOS::ObjectMessage(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_OBJECT_MESSAGE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}


//================================================================//
#pragma mark - Delagate Event handlers
//================================================================//
@implementation MoaiSmartFoxDelegate


- (void)onConnection:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().Connection ( evt );
}

- (void)onConnectionLost:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().ConnectionLost ( evt );
}


- (void)onLogin:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().Login ( evt );
}

- (void)onLoginError:(SFSEvent *)evt
{
	MOAISmartFoxIOS::Get ().LoginError ( evt );
}

- (void)onRoomJoin:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().RoomJoin ( evt );
}

- (void)onRoomJoinError:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().RoomJoinError ( evt );
}

- (void)onUserEnterRoom:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().UserEnterRoom ( evt );
}

- (void)onUserExitRoom:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().UserExitRoom ( evt );
}
- (void)onUserCountChange:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().UserCountChange ( evt );
}

- (void)onRoomAdd:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().RoomAdd ( evt );
}

- (void)onRoomRemove:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().RoomRemove ( evt );
}

- (void)onPublicMessage:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().PublicMessage ( evt );
}

- (void)onPrivateMessage:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().PrivateMessage ( evt );
}

- (void)onRoomCreationError:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().RoomCreationError ( evt );
}

- (void)onRoomVariablesUpdate:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().RoomVariablesUpdate ( evt );
}

- (void)onObjectMessage:(SFSEvent *)evt
{
   MOAISmartFoxIOS::Get ().ObjectMessage ( evt );
}

@end

