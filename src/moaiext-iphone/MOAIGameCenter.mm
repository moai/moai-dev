// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#import <moaiext-iphone/MOAIGameCenter.h>
#import <moaiext-iphone/NSDate+MOAILib.h>


//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	authenticatePlayer
	@text	Makes sure a Game Center is supported and an account is 
			logged in. If none are logged in, will prompt the user 
			to log in. This must be	called before any other 
			MOAIGameCenter functions.
			
	@in		nil
	@out	nil
*/
int MOAIGameCenter::_authenticatePlayer ( lua_State* L ) {
	USLuaState state ( L );
	
	// Check for presence of GKLocalPlayer class.
    BOOL localPlayerClassAvailable = ( NSClassFromString ( @"GKLocalPlayer" )) != nil;
	
	// The device must be running iOS 4.1 or later.
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[ UIDevice currentDevice ] systemVersion ];
    BOOL osVersionSupported = ([ currSysVer compare:reqSysVer options:NSNumericSearch ] != NSOrderedAscending );
    if ( localPlayerClassAvailable && osVersionSupported ) {
		
		// If Game Center is available, attempt to authenticate the local player
		GKLocalPlayer *localPlayer = [ GKLocalPlayer localPlayer ];
		[ localPlayer authenticateWithCompletionHandler: ^( NSError *error ) {
			if ( [ error code ] == GKErrorNotSupported ) {
				MOAIGameCenter::Get ().mIsGameCenterSupported = FALSE;
			}
			else if ([ GKLocalPlayer localPlayer ].isAuthenticated) {
				MOAIGameCenter::Get ().mIsGameCenterSupported = TRUE;							
			}
		 }];
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getScores
	@text	Returns the top ten scores of everyone for all time. Optionally 
			specify a leaderboard, player scope, time scope and range. If
			no leaderboard was specified, will return scores from default
			leaderboard.
	
	@opt	string		category
	@opt	number		playerScope
	@opt	number		timeScope
	@opt	number		startRange
	@opt	number		endRange
	@out	table		scores
*/
int MOAIGameCenter::_getScores ( lua_State* L ) {
	if ( !MOAIGameCenter::Get ().mIsGameCenterSupported ) return 0;
	USLuaState state ( L );
		
	cc8* category = state.GetValue < cc8* >( 1, NULL );
	int playerScope = state.GetValue < int >( 2, PLAYERSCOPE_GLOBAL );
	int timeScope = state.GetValue < int >( 3, TIMESCOPE_ALLTIME );
	int startRange = state.GetValue < int >( 4, 1 );
	int endRange = state.GetValue < int >( 5, 10 );

	GKLeaderboard *leaderboardRequest = [[ GKLeaderboard alloc] init ];
    if ( leaderboardRequest != nil ) {
	
		if ( category ) leaderboardRequest.category = [ NSString stringWithUTF8String:category];
        leaderboardRequest.playerScope = playerScope;
        leaderboardRequest.timeScope = timeScope;
        leaderboardRequest.range = NSMakeRange ( startRange, endRange );
        [ leaderboardRequest loadScoresWithCompletionHandler: ^( NSArray *scores, NSError *error ) {
		
            if ( error != nil ) {
				printf ( "Error in getting leaderboard scores: %d\n", [ error code ]);
            }
            if ( scores != nil ) {
				MOAIGameCenter::Get ().CallScoresCallback ( scores );
            }
		}];
    }
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	isSupported
	@text	Returns whether or not Game Center is supported on the 
			current device.
			
	@in		nil
	@out	bool isSupported
*/
int MOAIGameCenter::_isSupported ( lua_State* L ) {
	USLuaState state ( L );

	lua_pushboolean ( state, MOAIGameCenter::Get ().mIsGameCenterSupported );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	reportScore
	@text	Reports a score to a desired leaderboard on Game Center.
			
	@in		number	score
	@in		string	category
	@out	nil
*/
int MOAIGameCenter::_reportScore ( lua_State* L ) {
	if ( !MOAIGameCenter::Get ().mIsGameCenterSupported ) return 0;
	USLuaState state ( L );

	s64 score =  ( s64 )lua_tonumber ( state, 1 );
	cc8* category = lua_tostring ( state, 2 );
		
	MOAIGameCenter::Get ().ReportScore ( score, category );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGetScoresCallabck
	@text	Sets the function to be called upon a successfule 'getScores' call.
			
	@in		function	getScoresCallback
	@out	nil
*/
int MOAIGameCenter::_setGetScoresCallback ( lua_State* L ) {
	USLuaState state ( L );

	MOAIGameCenter::Get ().mGetScoresCallback.SetRef ( state, 1, false );
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showDefaultLeaderboard
	@text	Displays the default leaderboard view controller
			
	@in		nil
	@out	nil
*/
int MOAIGameCenter::_showDefaultLeaderboard ( lua_State* L ) {
	USLuaState state ( L );

	printf("1\n");
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];	
	GKLeaderboardViewController *leaderboardController = [[ GKLeaderboardViewController alloc ] init ];
    if ( leaderboardController != nil )
    {	
		printf("2\n");
        leaderboardController.leaderboardDelegate = MOAIGameCenter::Get ().mLeaderboardDelegate;
		if ( rootVC != nil ) {
			printf("3\n");
			[ rootVC presentModalViewController: leaderboardController animated: YES ];
		}
    }	
		
	return 0;
}

//================================================================//
// MOAIGameCenter
//================================================================//

//----------------------------------------------------------------//
void MOAIGameCenter::CallScoresCallback ( NSArray* scores ) {

	if ( mGetScoresCallback && [ scores count ] > 0 ) {
	
		USLuaStateHandle state = mGetScoresCallback.GetSelf ();
		lua_newtable ( state );
		
		for ( id score in scores ) {
		
			lua_pushstring( state, "formattedValue");
			lua_pushstring ( state, [[ score formattedValue ] UTF8String ]);
			lua_settable ( state, -3 );			
			
			lua_pushstring( state, "playerId");
			lua_pushstring ( state, [[ score playerID ] UTF8String ]);
			lua_settable ( state, -3 );
			
			lua_pushstring( state, "category");
			lua_pushstring ( state, [[ score category ] UTF8String ]);
			lua_settable ( state, -3 );
						
			lua_pushstring( state, "rank");
			lua_pushnumber ( state, [ score rank ] );
			lua_settable ( state, -3 );
			
			lua_pushstring( state, "date");
			[[ score date ] toLua:state ];
			lua_settable ( state, -3 );
		}
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
MOAIGameCenter::MOAIGameCenter () :
	mIsGameCenterSupported ( false ) {

	RTTI_SINGLE ( USLuaObject )		
	
	mLeaderboardDelegate = [ MoaiLeaderboardDelegate alloc ];
}

//----------------------------------------------------------------//
MOAIGameCenter::~MOAIGameCenter () {

}

//----------------------------------------------------------------//
void MOAIGameCenter::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "TIMESCOPE_TODAY",		( u32 )TIMESCOPE_TODAY );
	state.SetField ( -1, "TIMESCOPE_WEEK",		( u32 )TIMESCOPE_WEEK );
	state.SetField ( -1, "TIMESCOPE_ALLTIME",	( u32 )TIMESCOPE_ALLTIME );
	
	state.SetField ( -1, "PLAYERSCOPE_GLOBAL",	( u32 )PLAYERSCOPE_GLOBAL );
	state.SetField ( -1, "PLAYERSCOPE_FRIENDS",	( u32 )PLAYERSCOPE_FRIENDS );
	
	luaL_Reg regTable[] = {
		{ "authenticatePlayer",		_authenticatePlayer },
		{ "getScores",				_getScores },
		{ "isSupported",			_isSupported },
		{ "reportScore",			_reportScore },
		{ "setGetScoresCallback",	_setGetScoresCallback },
		{ "showDefaultLeaderboard",	_showDefaultLeaderboard },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGameCenter::ReportScore ( s64 score, cc8* category ) {

	GKScore *scoreReporter = [[[ GKScore alloc ] initWithCategory:[ NSString stringWithUTF8String:category ]] autorelease ];
    scoreReporter.value = score;
    [ scoreReporter reportScoreWithCompletionHandler: ^( NSError *error ) {
		if (error != nil)
		{
			printf ( "Error in score reporting: %d", [ error code ]);
			// TODO: Save off score for later if network error
		}
    }];
}

//================================================================//
// MoaiLeaderboardDelegate
//================================================================//
@implementation MoaiLeaderboardDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol GKLeaderboardViewControllerDelegate
	//================================================================//
	
	-( void ) leaderboardViewControllerDidFinish:( GKLeaderboardViewController* ) viewController {
	
		UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
		UIViewController* rootVC = [ window rootViewController ];
		[ rootVC dismissModalViewControllerAnimated:YES ];
	}


@end