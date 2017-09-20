// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDELTADNAANDROID_H
#define	MOAIDELTADNAANDROID_H

#include <moai-core/headers.h>
#include <moai-android/JniUtils.h>

//================================================================//
// MOAIDeltaDNAAndroid
//================================================================//
/**	@lua	MOAIDeltaDNAAndroid
	@text	Wrapper for Facebook integration on Android devices.
			Facebook provides social integration for sharing on
			www.facebook.com. Exposed to Lua via MOAIFacebook on
			all mobile platforms.

	@const	DIALOG_DID_COMPLETE			Event code for a successfully completed Facebook dialog.
	@const	DIALOG_DID_NOT_COMPLETE		Event code for a failed (or canceled) Facebook dialog.
	@const	REQUEST_RESPONSE			Event code for graph request responses.
	@const	REQUEST_RESPONSE_FAILED		Event code for failed graph request responses.
	@const	SESSION_DID_LOGIN			Event code for a successfully completed Facebook login.
	@const	SESSION_DID_NOT_LOGIN		Event code for a failed (or canceled) Facebook login.
*/
class MOAIDeltaDNAAndroid :
	public MOAIGlobalClass < MOAIDeltaDNAAndroid, MOAIGlobalEventSource >,
	public JniUtils {
private:

	jmethodID	mJava_Initialize;

	//----------------------------------------------------------------//
	static int	_initialize				( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIDeltaDNAAndroid );

			MOAIDeltaDNAAndroid		();
			~MOAIDeltaDNAAndroid	();
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //MOAIDELTADNAANDROID_H
