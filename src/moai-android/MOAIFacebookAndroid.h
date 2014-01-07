//----------------------------------------------------------------//
// Using Facebook Android SDK 3.6 (January 2014)
// Copyright (c) 2013-present:
//		Author(s):
//				* An Nguyen (MeYuMe http://www.meyume.com)
//				* Ricard Sole (gh: rcsole)
//----------------------------------------------------------------//

#ifndef	MOAIFACEBOOKANDROID_H
#define	MOAIFACEBOOKANDROID_H

#ifndef DISABLE_FACEBOOK

#include <moai-core/headers.h>

//================================================================//
// MOAIFacebookAndroid
//================================================================//
/**	@name	MOAIFacebookAndroid
	@text	Wrapper for Facebook integration on Android devices.
			Facebook provides social integration for sharing on
			www.facebook.com. Exposed to lua via MOAIFacebook on
			all mobile platforms.

			DID_LOGIN,
			DID_LOGOUT,
			RETRIEVED_USER_DETAILS,
			REQUEST_DIALOG_CLOSED,
			PUBLISH_DIALOG_CLOSED,
			RETRIEVED_SCORES,
			RETRIEVED_APP_REQUESTS,
			RETRIEVED_FRIENDS,
			RETRIEVED_PERMISSIONS,
			REQUESTED_PERMISSIONS,
			RETRIEVED_PROFILE_PICTURE,
			RETRIEVED_GRAPH_REQUEST_DATA,
*/
class MOAIFacebookAndroid :
		public MOAIGlobalClass < MOAIFacebookAndroid, MOAILuaObject > {

private:
	static int _init ( lua_State* L );
	static int _login (  lua_State* L  );
	static int _logout (  lua_State* L  );
	static int _getPermissions (  lua_State* L  );
	static int _requestPermissions (  lua_State* L  );
	static int _publish (  lua_State* L  );
	static int _openRequestDialog (  lua_State* L  );
	static int _sendRequest (  lua_State* L  );
	static int _getAppRequests (  lua_State* L  );
	static int _deleteRequest (  lua_State* L  );
	static int _getUserDetails (  lua_State* L  );
	static int _loadProfilePicture (  lua_State* L  );
	static int _getFriends (  lua_State* L  );
	static int _sendScore (  lua_State* L  );
	static int _getScores (  lua_State* L  );
	static int _graphRequest (  lua_State* L  );
	static int _setListener (  lua_State* L  );

public:
	DECL_LUA_SINGLETON ( MOAIFacebookAndroid );

	enum {
		DID_LOGIN,
		DID_LOGOUT,
		RETRIEVED_USER_DETAILS,
		REQUEST_DIALOG_CLOSED,
		PUBLISH_DIALOG_CLOSED,
		RETRIEVED_SCORES,
		RETRIEVED_APP_REQUESTS,
		RETRIEVED_FRIENDS,
		RETRIEVED_PERMISSIONS,
		REQUESTED_PERMISSIONS,
		RETRIEVED_PROFILE_PICTURE,
		RETRIEVED_GRAPH_REQUEST_DATA,
		TOTAL,
	};

	MOAILuaRef mListeners [ TOTAL ];

		 MOAIFacebookAndroid ();
		 ~MOAIFacebookAndroid ();
	void RegisterLuaClass ( MOAILuaState& state );

	void notifyFBDidLogin ();
	void notifyFBDidLogout ();
	void notifyFBDidRetrieveUserDetails ( cc8* jsonResult );
	void notifyFBDidRetrieveFriends ( cc8* jsonResult );
	void notifyFBDidClosePublishDialog ( bool success );
	void notifyFBDidCloseRequestDialog ( bool success );
	void notifyFBDidRetrieveScores ( cc8* jsonResult );
	void notifyFBDidRetrieveAppRequests ( cc8* jsonResult );
	void notifyFBDidRetrievePermissions ( cc8* jsonResult );
	void notifyFBDidRequestPermissions ();
	void notifyFBDidLoadProfilePicture ( cc8* userId, cc8* picturePath );
	void notifyFBDidReceiveGraphRequestData ( cc8* jsonResult );

};

#endif  //DISABLE_FACEBOOK

#endif  //MOAIFACEBOOK_H
