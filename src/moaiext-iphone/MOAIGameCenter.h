// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGAMECENTER_H
#define	MOAIGAMECENTER_H

#import <GameKit/GameKit.h>
#import <moaicore/moaicore.h>

@class MoaiLeaderboardDelegate;

//================================================================//
// MOAIGameCenter
//================================================================//
/**	@name	MOAIGameCenter
	@text	Wrapper for iOS Game Center functionality
	
	@const	TIMESCOPE_TODAY				Get leaderboard scores for today.
	@const	TIMESCOPE_WEEK				Get leaderboard scores for the week.
	@const	TIMESCOPE_ALLTIME			Get leaderboard scores for all time.
	
	@const	PLAYERSCOPE_GLOBAL			Get leaderboard scores for everyone.
	@const	PLAYERSCOPE_FRIENDS			Get leaderboard scores only for active player's friends.
*/

class MOAIGameCenter :
	public USGlobalClass < MOAIGameCenter, USLuaObject > {
private:

	enum {
		TIMESCOPE_TODAY = 0,
		TIMESCOPE_WEEK,
		TIMESCOPE_ALLTIME
	};

	enum {
		PLAYERSCOPE_GLOBAL = 0,
		PLAYERSCOPE_FRIENDS
	};
	
	USLuaRef					mGetScoresCallback;
	BOOL						mIsGameCenterSupported;
	MoaiLeaderboardDelegate*	mLeaderboardDelegate;
	
	//----------------------------------------------------------------//
	static int		_authenticatePlayer			( lua_State* L );
	static int		_getScores					( lua_State* L );
	static int		_isSupported				( lua_State* L );
	static int		_reportScore				( lua_State* L );
	static int		_setGetScoresCallback		( lua_State* L );
	static int		_showDefaultLeaderboard		( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIGameCenter );
	
	//----------------------------------------------------------------//
	void			CallScoresCallback			( NSArray* scores );	
					MOAIGameCenter				();
					~MOAIGameCenter				();
	void			RegisterLuaClass			( USLuaState& state );
	void			ReportScore					( s64 score, cc8* category );
	
};


//================================================================//
// MoaiLeaderboardDelegate
//================================================================//
@interface MoaiLeaderboardDelegate : NSObject < GKLeaderboardViewControllerDelegate > {
@private
}
@end


#endif
