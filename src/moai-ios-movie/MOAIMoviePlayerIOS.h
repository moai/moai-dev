// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOVIEPLAYERIOS_H
#define	MOAIMOVIEPLAYERIOS_H

#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>
#import <moai-core/headers.h>

@class MOAIMoviePlayerPlaybackDelegate;

//================================================================//
// MOAIMoviePlayerIOS
//================================================================//
/**	@lua	MOAIMoviePlayerIOS
	@text	Wrapper for simple video playback. Exposed to Lua via 
			MOAIMoviePlayer on all mobile platforms.
*/
class MOAIMoviePlayerIOS :
	public ZLContextClass < MOAIMoviePlayerIOS >,
	public virtual MOAILuaObject {
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
	MOAILuaStrongRef			mListeners [ TOTAL ];
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
