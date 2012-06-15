// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGAMECENTERIOS_H
#define	MOAIGAMECENTERIOS_H

#import <GameKit/GameKit.h>
#import <moaicore/moaicore.h>

@class MOAIGameCenterIOSLeaderboardDelegate;
@class MOAIGameCenterIOSAchievementDelegate;

//================================================================//
// MOAIGameCenterIOS
//================================================================//
/**	@name	MOAIGameCenterIOS
	@text	Wrapper for iOS GameCenter functionality.
	
	@const	TIMESCOPE_TODAY			Get leaderboard scores for today.
	@const	TIMESCOPE_WEEK			Get leaderboard scores for the week.
	@const	TIMESCOPE_ALLTIME		Get leaderboard scores for all time.
	
	@const	PLAYERSCOPE_GLOBAL		Get leaderboard scores for everyone.
	@const	PLAYERSCOPE_FRIENDS		Get leaderboard scores only for active player's friends.
*/

class MOAIGameCenterIOS :
	public MOAIGlobalClass < MOAIGameCenterIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_authenticatePlayer			( lua_State* L );
	static int	_getPlayerAlias				( lua_State* L );
	static int	_getScores					( lua_State* L );
	static int	_isSupported				( lua_State* L );
	static int	_reportAchievementProgress	( lua_State* L );
	static int	_reportScore				( lua_State* L );
	static int	_setGetScoresCallback		( lua_State* L );
	static int	_showDefaultAchievements	( lua_State* L );
	static int	_showDefaultLeaderboard		( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIGameCenterIOS );

	enum {
		TIMESCOPE_TODAY = 0,
		TIMESCOPE_WEEK,
		TIMESCOPE_ALLTIME
	};

	enum {
		PLAYERSCOPE_GLOBAL = 0,
		PLAYERSCOPE_FRIENDS
	};
	
	MOAILuaRef								mGetScoresCallback;
	BOOL									mIsGameCenterSupported;
	MOAIGameCenterIOSLeaderboardDelegate*	mLeaderboardDelegate;
	MOAIGameCenterIOSAchievementDelegate*	mAchievementDelegate;
	NSMutableDictionary*        			mAchievementsDictionary;
	GKLocalPlayer*							mLocalPlayer;
	
	void			CallScoresCallback				( NSArray* scores );
	void			CreateAchievementDictionary		( NSArray* achievements );
	void			GetAchievements					();
	GKAchievement*	GetAchievementFromDictionary	( cc8* identifier );
					MOAIGameCenterIOS				();
					~MOAIGameCenterIOS				();
	void			RegisterLuaClass				( MOAILuaState& state );
	void			ReportAchievementProgress		( cc8* identifier, float percent );
	void			ReportScore						( s64 score, cc8* category );
	
};

//================================================================//
// MOAIGameCenterIOSLeaderboardDelegate
//================================================================//
@interface MOAIGameCenterIOSLeaderboardDelegate : NSObject < GKLeaderboardViewControllerDelegate > {
@private
}
@end

//================================================================//
// MOAIGameCenterIOSAchievementDelegate
//================================================================//
@interface MOAIGameCenterIOSAchievementDelegate : NSObject < GKAchievementViewControllerDelegate > {
@private
}
@end

#endif