// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaicore/moaicore.h>
#include <moaiext-debugger/MOAIHarness.h>
#ifdef WIN32
#include <winsock.h>
#endif

//================================================================//
// MOAIHarness
//================================================================//

//----------------------------------------------------------------//
int MOAIHarness::mSocketID = -1;
bool MOAIHarness::mEnginePaused = false;
struct sockaddr_in MOAIHarness::mSocketAddr;

//----------------------------------------------------------------//
void MOAIHarness::Callback(lua_State *L, lua_Debug *ar)
{
	lua_getinfo(L, "nSl", ar);
	int ev = ar->event;
	const char *name = ar->name;
	const char *namewhat = ar->namewhat;
	const char *what = ar->what;
	const char *source = ar->source;
	const char *short_src = ar->short_src;
	int currentline = ar->currentline;
	int nups = ar->nups;
	int linedefined = ar->linedefined;
	int lastlinedefined = ar->lastlinedefined;
	// debugging information is now filled in.
	// compare with set breakpoints etc...

	return;
}

//----------------------------------------------------------------//
void MOAIHarness::HookLua(lua_State* L, const char* target, int port)
{
	MOAIHarness::mSocketID = -1;
	MOAIHarness::mEnginePaused = false;

#ifdef WIN32
	// Initalize WinSock if required.
	WORD wVersionRequested;
    WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
    int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		printf("debug harness: Unable to initalize WinSock! [winsock error %i]", WSAGetLastError());
		return;
	}
#endif

	// Attempt to connect to the IDE.  There needs to be
	// a command-line toggle for this so that it doesn't attempt
	// to do it if there's no IDE available.
	MOAIHarness::mSocketID = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MOAIHarness::mSocketID == -1)
	{
		printf("debug harness: Unable to create socket for debugging! [winsock error %i]", WSAGetLastError());
		return;
	}

	// Resolve the port and target name into a socket address.
	memset(&MOAIHarness::mSocketAddr, 0, sizeof(MOAIHarness::mSocketAddr));
    MOAIHarness::mSocketAddr.sin_family = AF_INET;
    MOAIHarness::mSocketAddr.sin_port = htons(port);
    int res = inet_pton(AF_INET, target, &MOAIHarness::mSocketAddr.sin_addr);
	if (res < 0)
	{
		printf("debug harness: Unable to connect socket for debugging (invalid address family)! [winsock error %i]", WSAGetLastError());
		closesocket(MOAIHarness::mSocketID);
		MOAIHarness::mSocketID = -1;
		return;
	}
	else if (res == 0)
	{
		printf("debug harness: Unable to connect socket for debugging (invalid IP address)! [winsock error %i]", WSAGetLastError());
		closesocket(MOAIHarness::mSocketID);
		MOAIHarness::mSocketID = -1;
		return;
	}

	// Connect to the actual IDE.
	if (connect(MOAIHarness::mSocketID, (struct sockaddr *)&MOAIHarness::mSocketAddr, sizeof(MOAIHarness::mSocketAddr)) == -1)
	{
		printf("debug harness: Unable to connect socket to requested endpoint! [winsock error %i]", WSAGetLastError());
		closesocket(MOAIHarness::mSocketID);
		MOAIHarness::mSocketID = -1;
		return;
	}

	// Initalize the Lua hooks.
	lua_sethook(L, MOAIHarness::Callback, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE | LUA_MASKCOUNT, 1);

	// At this point the debug hooks are initalized, but we don't want to proceed until the IDE tells
	// us to continue (in case it wants to set up initial breakpoints).
	MOAIHarness::SendWait();
	MOAIHarness::Pause();

	// After we have done the wait-and-pause cycle, we are ready to give control back to the engine.
	return;
}

//----------------------------------------------------------------//
void MOAIHarness::Pause()
{
	printf("debug harness: Waiting to receive messages from debugging interface.");
	MOAIHarness::mEnginePaused = true;
	while (MOAIHarness::mEnginePaused)
	{
		// Receive and handle IDE messages until we get a continue.
		MOAIHarness::ReceiveMessage();
	}
	printf("debug harness: Continuing execution of program.");
}

//----------------------------------------------------------------//
void MOAIHarness::SendWait()
{
	// Sends a wait signal to the IDE.
	std::string wait;
	wait = "{\"ID\":\"wait\"}";
	MOAIHarness::SendMessage(wait);
}

//----------------------------------------------------------------//
void MOAIHarness::SendMessage(std::string data)
{
	// Add the terminators.
	data += "\0\0";

	// Send the data in 256-byte chunks.
	for (unsigned int i = 0; i < data.length(); i += 256)
	{
		// Create a buffer.
		char buffer[256];
		const char* raw = data.c_str();
		memset(&buffer, 0, sizeof(buffer));

		// Copy our data.
		int size = 0;
		int a = 0;
		for (a = 0; a < 256; a++)
		{
			buffer[a] = raw[i + a];
			if (raw[i + a] == '\0')
				break; // NULL terminator.
		}
		size = (a == 256) ? 256 : a + 1;
		size = (size < 2) ? 2 : size;

		// Send the burst of data.
		sendto(MOAIHarness::mSocketID, buffer, size, 0, (struct sockaddr *)&MOAIHarness::mSocketAddr, sizeof(MOAIHarness::mSocketAddr));
	}
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveContinue(json_t* node)
{
	MOAIHarness::mEnginePaused = false;
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveBreak(json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveBreakSetAlways(json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveBreakSetConditional(json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveBreakClear(json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveVariableGet(json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveVariableSet(json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveEvaluate(json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveMessage()
{
	// Receive a single message from the TCP socket and then
	// delegate the data to one of the ReceiveX functions.
	std::string json = "";
	while (true)
	{
		// Get the data.
		char buffer[256];
		memset(&buffer, 0, sizeof(buffer));
		int bytes = recv(MOAIHarness::mSocketID, &buffer[0], 256, 0);
		if (bytes == 0 || bytes == SOCKET_ERROR)
			break;

		// Add to the std::string buffer.
		for (int i = 0; i < bytes; i += 1)
			json += buffer[i];

		// Check to see if the last two bytes received were NULL terminators.
		if (json[json.length() - 1] == '\0' &&
			json[json.length() - 2] == '\0')
			break;
	}

	// We have received a message.  Parse the JSON to work out
	// exactly what type of message it is.
	json_t* node = json_loads(json.c_str(), 0, NULL);
	
	// Get a reference to the ID value.
	json_t* np_id = json_object_get(node, "ID");
	if (np_id == NULL || json_typeof(np_id) == JSON_STRING)
		return; // Unknown message type.
	
	// Check the type of message.
	if (std::string(json_string_value(np_id)) == "continue")
		MOAIHarness::ReceiveContinue(node);
	else if (std::string(json_string_value(np_id)) == "break")
		MOAIHarness::ReceiveBreak(node);
	else if (std::string(json_string_value(np_id)) == "break_set_always")
		MOAIHarness::ReceiveBreakSetAlways(node);
	else if (std::string(json_string_value(np_id)) == "break_set_conditional")
		MOAIHarness::ReceiveBreakSetConditional(node);
	else if (std::string(json_string_value(np_id)) == "break_clear")
		MOAIHarness::ReceiveBreakClear(node);
	else if (std::string(json_string_value(np_id)) == "variable_get")
		MOAIHarness::ReceiveVariableGet(node);
	else if (std::string(json_string_value(np_id)) == "variable_set")
		MOAIHarness::ReceiveVariableSet(node);
	else if (std::string(json_string_value(np_id)) == "evaluate")
		MOAIHarness::ReceiveEvaluate(node);
	json_decref(node);
}