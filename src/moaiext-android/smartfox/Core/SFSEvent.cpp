// ===================================================================
//
// Description		
//		Contains the implementation of SFSEvent
//
// Revision history
//		Date			Description
//		30-Nov-2012		First version
//
// ===================================================================
#include "SFSEvent.h"

using namespace Sfs2X::Core;

#if defined(_MSC_VER)
#pragma warning(disable:4786)			// STL library: disable warning 4786; this warning is generated due to a Microsoft bug
#endif
#include <map>							// STL library: map object
#include <string>						// STL library: string object
using namespace std;					// STL library: declare the STL namespace

namespace Sfs2X {
namespace Core {

const string* SFSEvent::HANDSHAKE (new string("handshake"));
const string* SFSEvent::UDP_INIT (new string("udpInit"));
const string* SFSEvent::CONNECTION (new string("connection"));
const string* SFSEvent::PING_PONG (new string("pingPong"));
const string* SFSEvent::SFS_SOCKET_ERROR (new string("socketError"));
const string* SFSEvent::CONNECTION_LOST (new string("connectionLost"));
const string* SFSEvent::CONNECTION_RETRY (new string("connectionRetry"));
const string* SFSEvent::CONNECTION_RESUME (new string("connectionResume"));
const string* SFSEvent::CONNECTION_ATTEMPT_HTTP (new string("connectionAttemptHttp"));
const string* SFSEvent::CONFIG_LOAD_SUCCESS (new string("configLoadSuccess"));
const string* SFSEvent::CONFIG_LOAD_FAILURE (new string("configLoadFailure"));
const string* SFSEvent::LOGIN (new string("login"));
const string* SFSEvent::LOGIN_ERROR (new string("loginError"));
const string* SFSEvent::LOGOUT (new string("logout"));
const string* SFSEvent::ROOM_ADD (new string("roomAdd"));
const string* SFSEvent::ROOM_REMOVE (new string("roomRemove"));
const string* SFSEvent::ROOM_CREATION_ERROR (new string("roomCreationError"));
const string* SFSEvent::ROOM_JOIN (new string("roomJoin"));
const string* SFSEvent::ROOM_JOIN_ERROR (new string("roomJoinError"));
const string* SFSEvent::USER_ENTER_ROOM (new string("userEnterRoom"));
const string* SFSEvent::USER_EXIT_ROOM (new string("userExitRoom"));
const string* SFSEvent::USER_COUNT_CHANGE (new string("userCountChange"));
const string* SFSEvent::PUBLIC_MESSAGE (new string("publicMessage"));
const string* SFSEvent::PRIVATE_MESSAGE (new string("privateMessage"));
const string* SFSEvent::MODERATOR_MESSAGE (new string("moderatorMessage"));
const string* SFSEvent::ADMIN_MESSAGE (new string("adminMessage"));
const string* SFSEvent::OBJECT_MESSAGE (new string("objectMessage"));
const string* SFSEvent::EXTENSION_RESPONSE (new string("extensionResponse"));
const string* SFSEvent::ROOM_VARIABLES_UPDATE (new string("roomVariablesUpdate"));
const string* SFSEvent::USER_VARIABLES_UPDATE (new string("userVariablesUpdate"));
const string* SFSEvent::ROOM_GROUP_SUBSCRIBE (new string("roomGroupSubscribe"));
const string* SFSEvent::ROOM_GROUP_UNSUBSCRIBE (new string("roomGroupUnsubscribe"));
const string* SFSEvent::ROOM_GROUP_SUBSCRIBE_ERROR (new string("roomGroupSubscribeError"));
const string* SFSEvent::ROOM_GROUP_UNSUBSCRIBE_ERROR (new string("roomGroupUnsubscribeError"));
const string* SFSEvent::SPECTATOR_TO_PLAYER (new string("spectatorToPlayer"));
const string* SFSEvent::PLAYER_TO_SPECTATOR (new string("playerToSpectator"));
const string* SFSEvent::SPECTATOR_TO_PLAYER_ERROR (new string("spectatorToPlayerError"));
const string* SFSEvent::PLAYER_TO_SPECTATOR_ERROR (new string("playerToSpectatorError"));
const string* SFSEvent::ROOM_NAME_CHANGE (new string("roomNameChange"));
const string* SFSEvent::ROOM_NAME_CHANGE_ERROR (new string("roomNameChangeError"));
const string* SFSEvent::ROOM_PASSWORD_STATE_CHANGE (new string("roomPasswordStateChange"));
const string* SFSEvent::ROOM_PASSWORD_STATE_CHANGE_ERROR (new string("roomPasswordStateChangeError"));
const string* SFSEvent::ROOM_CAPACITY_CHANGE (new string("roomCapacityChange"));
const string* SFSEvent::ROOM_CAPACITY_CHANGE_ERROR (new string("roomCapacityChangeError"));
const string* SFSEvent::ROOM_FIND_RESULT (new string("roomFindResult"));
const string* SFSEvent::USER_FIND_RESULT (new string("userFindResult"));
const string* SFSEvent::INVITATION (new string("invitation"));
const string* SFSEvent::INVITATION_REPLY (new string("invitationReply"));
const string* SFSEvent::INVITATION_REPLY_ERROR (new string("invitationReplyError"));
const string* SFSEvent::DEBUG_MESSAGE (new string("debugMessage"));

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
SFSEvent::SFSEvent(string type)
	: BaseEvent (type, NULL)
{
}

// -------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------
SFSEvent::SFSEvent(string type, map<string, void*>* data)
	: BaseEvent (type, data)
{
}

}	// namespace Core
}	// namespace Sfs2X
