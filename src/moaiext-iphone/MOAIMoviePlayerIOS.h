// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOVIEPLAYERIOS_H
#define	MOAIMOVIEPLAYERIOS_H

#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>
#import <moaicore/moaicore.h>

@class MOAIMoviePlayerPlaybackDelegate;

//================================================================//
// MOAIMoviePlayerIOS
//================================================================//
/**	@name	MOAIMoviePlayerIOS
	@text	Wrapper for simple video playback. Exposed to lua via 
			MOAIMoviePlayer on all mobile platforms.
*/
class MOAIMoviePlayerIOS :
	public MOAIGlobalClass < MOAIMoviePlayerIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init			( lua_State* L );
	static int	_play			( lua_State* L );
	static int	_pause			( lua_State* L );
	static int	_setListener	( lua_State* L );
	static int	_stop			( lua_State* L );

	MOAIMoviePlayerPlaybackDelegate * 	mPlaybackDelegate;
		
public:
	
	DECL_LUA_SINGLETON ( MOAIMoviePlayerIOS )

	enum {
		TOTAL,
	};
	
	UIInterfaceOrientation  	mInitialOrientation;
	UIInterfaceOrientation  	mLastOrientation;
	MOAILuaRef					mListeners [ TOTAL ];
	MPMoviePlayerController * 	mMoviePlayer;

			MOAIMoviePlayerIOS	();
			~MOAIMoviePlayerIOS	();
	void	RegisterLuaClass	( MOAILuaState& state );
	void	Reset				();
};

@interface MOAIMoviePlayerPlaybackDelegate : NSObject {
	
}

- ( void ) videoLoadedCallback:( NSNotification * )notification;
- ( void ) videoFinishedCallback:( NSNotification * )notification;
- ( void ) deviceOrientationChanged:( NSNotification * )notification;

@end

#endif  //MOAIMOVIEPLAYERIOS_H