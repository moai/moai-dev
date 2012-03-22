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
	@text	Wrapper for iPhone app delegate.

	@const	TWEET_SUCCESSFUL
	@const	TWEET_CANCELLED
*/
class MOAITwitterIOS :
	public MOAIGlobalClass < MOAITwitterIOS, MOAILuaObject > {
private:

	enum {
		TWEET_SUCCESSFUL,
		TWEET_CANCELLED,
		TOTAL,
	};
	
	MOAILuaRef				mListeners [ TOTAL ];

	//----------------------------------------------------------------//
	static int		_composeTweet							( lua_State* L );
	static int		_canTweet								( lua_State* L );
	static int		_setListener							( lua_State* L );

public:
	
	
	DECL_LUA_SINGLETON ( MOAITwitterIOS )
	
	//----------------------------------------------------------------//
	void		DidSendTweet												( bool success );
				MOAITwitterIOS												();
				~MOAITwitterIOS												();
	void		RegisterLuaClass											( MOAILuaState& state );
};

#endif
