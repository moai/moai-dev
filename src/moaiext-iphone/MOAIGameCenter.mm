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
	MOAILuaState state ( L );
	
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
	
	MOAIGameCenter::Get ().GetAchievements ();
	
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
	MOAILuaState state ( L );
		
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
	MOAILuaState state ( L );

	lua_pushboolean ( state, MOAIGameCenter::Get ().mIsGameCenterSupported );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	reportAchievementProgress
	@text	Reports an achievement progress tod Game Center.
			
	@in		string	identifier
	@in		number	percentComplete
	@out	nil
*/
int MOAIGameCenter::_reportAchievementProgress ( lua_State* L ) {
	if ( !MOAIGameCenter::Get ().mIsGameCenterSupported ) return 0;
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	float percent =  ( float )lua_tonumber ( state, 2 );
		
	MOAIGameCenter::Get ().ReportAchievementProgress ( identifier, percent );
	
	return 0;
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
	MOAILuaState state ( L );

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
	MOAILuaState state ( L );

	MOAIGameCenter::Get ().mGetScoresCallback.SetStrongRef ( state, 1 );
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showDefaultAchievments
	@text	Displays the default achievements view controller
			
	@in		nil
	@out	nil
*/
int MOAIGameCenter::_showDefaultAchievements ( lua_State* L ) {
	MOAILuaState state ( L );

	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];	
	
	GKAchievementViewController *achievements = [[ GKAchievementViewController alloc ] init ];
    if (achievements != NULL)
    {
        achievements.achievementDelegate = MOAIGameCenter::Get ().mAchievementDelegate;
		if  ( rootVC != nil ) {
			[ rootVC presentModalViewController: achievements animated: YES ];
		}
    }
	[ achievements release ];
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showDefaultLeaderboard
	@text	Displays the default leaderboard view controller
			
	@in		nil
	@out	nil
*/
int MOAIGameCenter::_showDefaultLeaderboard ( lua_State* L ) {
	MOAILuaState state ( L );

	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];	
	GKLeaderboardViewController *leaderboardController = [[ GKLeaderboardViewController alloc ] init ];
    if ( leaderboardController != nil )
    {	
        leaderboardController.leaderboardDelegate = MOAIGameCenter::Get ().mLeaderboardDelegate;
		if ( rootVC != nil ) {
			[ rootVC presentModalViewController: leaderboardController animated: YES ];
		}
    }
	[ leaderboardController release ];
		
	return 0;
}

//================================================================//
// MOAIGameCenter
//================================================================//

//----------------------------------------------------------------//
void MOAIGameCenter::CallScoresCallback ( NSArray* scores ) {

	if ( mGetScoresCallback && [ scores count ] > 0 ) {
	
		MOAILuaStateHandle state = mGetScoresCallback.GetSelf ();
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
void MOAIGameCenter::CreateAchievementDictionary ( NSArray* achievements ) {
	
	for ( GKAchievement* achievement in achievements ) {
				
		[ mAchievementsDictionary setObject: achievement forKey: achievement.identifier ];
	}
}

//----------------------------------------------------------------//
GKAchievement* MOAIGameCenter::GetAchievementFromDictionary ( cc8* identifier ) {
	
	GKAchievement* achievement = [ mAchievementsDictionary objectForKey:[ NSString stringWithUTF8String:identifier ]];
	
    if ( achievement == nil ) {
	
        achievement = [[[ GKAchievement alloc ] initWithIdentifier:[ NSString stringWithUTF8String:identifier ]] autorelease ];
        [ mAchievementsDictionary setObject:achievement forKey:achievement.identifier  ];
    }
	
    return [[ achievement retain ] autorelease ];
}

//----------------------------------------------------------------//
void MOAIGameCenter::GetAchievements () {
	
	[ GKAchievement loadAchievementsWithCompletionHandler:^( NSArray* achievements, NSError* error ) {
	
		if ( error == nil ) {
					
			CreateAchievementDictionary ( achievements );
		}
	}];
}

//----------------------------------------------------------------//
MOAIGameCenter::MOAIGameCenter () :
	mIsGameCenterSupported ( false ) {

	RTTI_SINGLE ( MOAILuaObject )		
	
	mLeaderboardDelegate = [ MoaiLeaderboardDelegate alloc ];
	mAchievementDelegate = [ MoaiAchievementDelegate alloc ];
	mAchievementsDictionary = [[ NSMutableDictionary alloc ] init ];
}

//----------------------------------------------------------------//
MOAIGameCenter::~MOAIGameCenter () {
	
	[ mAchievementDelegate release ];
	[ mLeaderboardDelegate release ];
}

//----------------------------------------------------------------//
void MOAIGameCenter::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "TIMESCOPE_TODAY",		( u32 )TIMESCOPE_TODAY );
	state.SetField ( -1, "TIMESCOPE_WEEK",		( u32 )TIMESCOPE_WEEK );
	state.SetField ( -1, "TIMESCOPE_ALLTIME",	( u32 )TIMESCOPE_ALLTIME );
	
	state.SetField ( -1, "PLAYERSCOPE_GLOBAL",	( u32 )PLAYERSCOPE_GLOBAL );
	state.SetField ( -1, "PLAYERSCOPE_FRIENDS",	( u32 )PLAYERSCOPE_FRIENDS );
	
	luaL_Reg regTable[] = {
		{ "authenticatePlayer",			_authenticatePlayer },
		{ "getScores",					_getScores },
		{ "isSupported",				_isSupported },
		{ "reportAchievementProgress",	_reportAchievementProgress },
		{ "reportScore",				_reportScore },
		{ "setGetScoresCallback",		_setGetScoresCallback },
		{ "showDefaultAchievements",	_showDefaultAchievements },
		{ "showDefaultLeaderboard",		_showDefaultLeaderboard },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGameCenter::ReportAchievementProgress ( cc8* identifier, float percent ) {

	GKAchievement *achievement = GetAchievementFromDictionary ( identifier );
	
    if ( achievement ) {
	
		if ( achievement.percentComplete >= 100 ) return;
				
		achievement.percentComplete = percent;
		
        [ achievement reportAchievementWithCompletionHandler: ^(NSError *error) {
			if ( error != nil )
			{
				printf ( "Error in achievement reporting: %d", [ error code ]);
				// TODO: Save off achievement for later if network error
			}
		}];
				
		if ( percent >= 100 ) {
			
			 UIAlertView *alert = [[ UIAlertView alloc ] 
				initWithTitle: @"Achievement Earned!"
                      message: achievement.identifier
                     delegate: nil
			cancelButtonTitle: @"OK"
            otherButtonTitles: nil ];
			
            [alert show];
            [alert release];
		}
    }
}

//----------------------------------------------------------------//
void MOAIGameCenter::ReportScore ( s64 score, cc8* category ) {

	GKScore *scoreReporter = [[[ GKScore alloc ] initWithCategory:[ NSString stringWithUTF8String:category ]] autorelease ];
	
	if ( scoreReporter ) {
	
		scoreReporter.value = score;
		
		[ scoreReporter reportScoreWithCompletionHandler: ^( NSError *error ) {
			if ( error != nil )
			{
				printf ( "Error in score reporting: %d", [ error code ]);
				// TODO: Save off score for later if network error
			}
		}];
	}
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
		UNUSED ( viewController );
	
		UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
		UIViewController* rootVC = [ window rootViewController ];
		if ( rootVC ) {
			[ rootVC dismissModalViewControllerAnimated:YES ];
		}
	}


@end

//================================================================//
// MoaiAchievmentsdDelegate
//================================================================//
@implementation MoaiAchievementDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol GKAchievementViewControllerDelegate
	//================================================================//
	
	-( void ) achievementViewControllerDidFinish:( GKAchievementViewController* ) viewController {
		UNUSED ( viewController );
	
		UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
		UIViewController* rootVC = [ window rootViewController ];
		if ( rootVC ) {
			[ rootVC dismissModalViewControllerAnimated:YES ];
		}
	}


@end