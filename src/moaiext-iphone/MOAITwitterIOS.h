// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITWITTERIOS_H
#define	MOAITWITTERIOS_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>

//================================================================//
// MOAITwitterIOS
//================================================================//
/**	@name	MOAITwitterIOS
	@text	Wrapper for Twitter interaction on iOS devices. Only 
			available on iOS 5.0 and above.

	@const	TWEET_SUCCESSFUL		Event indicating a successful Tweet.
	@const	TWEET_CANCELLED			Event indicating an unsuccessful Tweet.
*/
class MOAITwitterIOS :
	public MOAIGlobalClass < MOAITwitterIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_composeTweet	( lua_State* L );
	static int	_canTweet		( lua_State* L );
	static int	_setListener	( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAITwitterIOS )
	
	enum {
		TWEET_SUCCESSFUL,
		TWEET_CANCELLED,
		TOTAL,
	};
	
	MOAILuaRef		mListeners [ TOTAL ];

			MOAITwitterIOS		();
			~MOAITwitterIOS		();
	void	DidSendTweet		( bool success );
	void	RegisterLuaClass	( MOAILuaState& state );
};

#endif
