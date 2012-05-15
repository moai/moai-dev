// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaicore/moaicore.h>
#include <moaiext-debugger/MOAIHarness.h>

#ifdef WIN32
	#include <winsock.h>
#endif

extern "C" {
	extern int Curl_inet_pton ( int, const char*, void* );
}

//================================================================//
// MOAIHarness
//================================================================//

//----------------------------------------------------------------//
int MOAIHarness::mSocketID = -1;
bool MOAIHarness::mEnginePaused = false;
struct sockaddr_in MOAIHarness::mSocketAddr;
std::vector<MOAIBreakpoint> MOAIHarness::mBreakpoints;
std::vector<char> MOAIHarness::mSocketInBuffer;

//----------------------------------------------------------------//
void MOAIHarness::Callback(lua_State *L, lua_Debug *ar)
{
	// Fill debugging information.
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

	// Compare against a list of breakpoints.
	for (std::vector<MOAIBreakpoint>::const_iterator i = MOAIHarness::mBreakpoints.begin(); i < MOAIHarness::mBreakpoints.end(); i++)
	{
		if (source[0] == '@' && (*i).filename == (source + 1) && (*i).line == currentline)
		{
			// Breakpoint hit.
			if (std::string(what) == "main")
				MOAIHarness::SendBreak(L, "<main>", (unsigned int)currentline, short_src);
			else if (std::string(what) == "C")
				MOAIHarness::SendBreak(L, "<native>", (unsigned int)currentline, short_src);
			else if (std::string(what) == "tail")
				MOAIHarness::SendBreak(L, "<tail call>", (unsigned int)currentline, short_src);
			else if (std::string(what) == "Lua" && name == NULL)
				MOAIHarness::SendBreak(L, "<anonymous>", (unsigned int)currentline, short_src);
			else if (std::string(what) == "Lua")
				MOAIHarness::SendBreak(L, name, (unsigned int)currentline, short_src);
			MOAIHarness::Pause(L);
			return;
		}
	}

	return;
}

//----------------------------------------------------------------//
int MOAIHarness::_sendMessage(lua_State* L)
{
	// Read the message off of the top of the stack
	json_t* message = MOAIHarness::ConvertStackIndexToJSON(L, lua_gettop(L));

	// Send the message back to the IDE.
	json_t* msg = json_object();
	json_object_set_new(msg, "ID", json_string("message"));
	json_object_set_new(msg, "Value", message);
	char* data = json_dumps(msg, 0);
	MOAIHarness::SendMessage(std::string(data));
	free(data);

	// Done!
	return 0;
}

//----------------------------------------------------------------//
MOAIHarness::MOAIHarness ()
{
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIHarness::~MOAIHarness ()
{
}

//----------------------------------------------------------------//
void MOAIHarness::RegisterLuaClass(MOAILuaState& state)
{
	luaL_Reg regTable[] = {
		{ "sendMessage", _sendMessage },
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

//----------------------------------------------------------------//
void MOAIHarness::RegisterLuaFuncs(MOAILuaState& state)
{
	UNUSED(state);
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
		printf("debug harness: Unable to initalize WinSock! [winsock error %i]\n", WSAGetLastError());
		return;
	}
#endif

	// Attempt to connect to the IDE.  There needs to be
	// a command-line toggle for this so that it doesn't attempt
	// to do it if there's no IDE available.
	MOAIHarness::mSocketID = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MOAIHarness::mSocketID == -1)
	{
		printf("debug harness: Unable to create socket for debugging! [winsock error %i]\n", WSAGetLastError());
		return;
	}

	// Resolve the port and target name into a socket address.
	memset(&MOAIHarness::mSocketAddr, 0, sizeof(MOAIHarness::mSocketAddr));
    MOAIHarness::mSocketAddr.sin_family = AF_INET;
    MOAIHarness::mSocketAddr.sin_port = htons(port);
    int res = Curl_inet_pton(AF_INET, target, &MOAIHarness::mSocketAddr.sin_addr);
	if (res < 0)
	{
		printf("debug harness: Unable to connect socket for debugging (invalid address family)! [winsock error %i]\n", WSAGetLastError());
		closesocket(MOAIHarness::mSocketID);
		MOAIHarness::mSocketID = -1;
		return;
	}
	else if (res == 0)
	{
		printf("debug harness: Unable to connect socket for debugging (invalid IP address)! [winsock error %i]\n", WSAGetLastError());
		closesocket(MOAIHarness::mSocketID);
		MOAIHarness::mSocketID = -1;
		return;
	}

	// Connect to the actual IDE.
	if (connect(MOAIHarness::mSocketID, (struct sockaddr *)&MOAIHarness::mSocketAddr, sizeof(MOAIHarness::mSocketAddr)) == -1)
	{
		printf("debug harness: Unable to connect socket to requested endpoint! [winsock error %i]\n", WSAGetLastError());
		closesocket(MOAIHarness::mSocketID);
		MOAIHarness::mSocketID = -1;
		return;
	}

	// Initalize the Lua hooks.
	lua_sethook(L, MOAIHarness::Callback, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);

	// At this point the debug hooks are initalized, but we don't want to proceed until the IDE tells
	// us to continue (in case it wants to set up initial breakpoints).
	MOAIHarness::SendWait();
	MOAIHarness::Pause(L);

	// After we have done the wait-and-pause cycle, we are ready to give control back to the engine.
	return;
}

//----------------------------------------------------------------//
void MOAIHarness::Update(lua_State* L)
{
	if (MOAIHarness::mSocketID == -1)
        return;

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(MOAIHarness::mSocketID, &readfds);

	// Check to see if any data is available for read on our socket
	int result = select(1, &readfds, NULL, NULL, &tv);

	// If we found data, process the next full message
	if (result > 0)
	{
		MOAIHarness::ReceiveMessage(L);
	}
}

//----------------------------------------------------------------//
void MOAIHarness::HandleError(const char* message, lua_State* L, int level)
{
    if (MOAIHarness::mSocketID == -1)
    {
        USLog::Print ( "%s\n", message );
        MOAILuaStateHandle state ( L );
        state.PrintStackTrace ( USLog::CONSOLE, level );
    }
    else
    {
        // Package the call stack into a json object
    	json_t* stack = ConvertCallStackToJSON(L, level);
        MOAIHarness::SendError(message, stack);
        MOAIHarness::Pause(L);
    }
}

//----------------------------------------------------------------//
void MOAIHarness::Pause(lua_State * L)
{
	printf("debug harness: Waiting to receive messages from debugging interface.\n");
	MOAIHarness::mEnginePaused = true;
	while (MOAIHarness::mEnginePaused)
	{
		// Receive and handle IDE messages until we get a continue.
		MOAIHarness::ReceiveMessage(L);
	}
	printf("debug harness: Continuing execution of program.\n");
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
void MOAIHarness::SendBreak(lua_State* L, std::string func, unsigned int line, std::string file)
{
	// Sends a "breakpoint hit" signal to the IDE.
	json_t* msg = json_object();
	json_object_set_new(msg, "ID", json_string("break"));
	json_object_set_new(msg, "FunctionName", json_string(func.c_str()));
	json_object_set_new(msg, "LineNumber", json_integer(line));
	json_object_set_new(msg, "FileName", json_string(file.c_str()));
    json_object_set_new(msg, "Stack", ConvertCallStackToJSON(L, 0));
	char* data = json_dumps(msg, 0);
	MOAIHarness::SendMessage(std::string(data));
	free(data);
}

//----------------------------------------------------------------//
void MOAIHarness::SendError(std::string message, json_t* stack)
{
    // Sends an "error occurred" signal to the IDE.
	json_t* msg = json_object();
	json_object_set_new(msg, "ID", json_string("error"));
	json_object_set_new(msg, "Message", json_string(message.c_str()));
    json_object_set_new(msg, "Stack", stack);
	char* data = json_dumps(msg, 0);
	MOAIHarness::SendMessage(std::string(data));
	free(data);
}

//----------------------------------------------------------------//
void MOAIHarness::SendResult(json_t* result)
{
	// Sends a "result of evaluation" signal to the IDE.
	json_t* msg = json_object();
	json_object_set_new(msg, "ID", json_string("result"));
	json_object_set_new(msg, "Value", result);
	char* data = json_dumps(msg, 0);
	MOAIHarness::SendMessage(std::string(data));
	free(data);
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
void MOAIHarness::ReceiveContinue(lua_State *L, json_t* node)
{
	MOAIHarness::mEnginePaused = false;
}

//----------------------------------------------------------------//
void MOAIHarness::ReceivePause(lua_State *L, json_t* node)
{
	MOAIHarness::Pause(L);
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveStop(lua_State *L, json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveBreakSetAlways(lua_State *L, json_t* node)
{
	// Get the breakpoint data.
	json_t* np_file = json_object_get(node, "FileName");
	if (np_file == NULL || json_typeof(np_file) != JSON_STRING)
		return;
	json_t* np_line = json_object_get(node, "LineNumber");
	if (np_line == NULL || json_typeof(np_line) != JSON_INTEGER)
		return;

	// Store breakpoint data.
	std::string file = std::string(json_string_value(np_file));
	int line = ( int )json_integer_value(np_line);

	// Add the breakpoint.
	mBreakpoints.insert(mBreakpoints.begin(), MOAIBreakpoint(file, line));
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveBreakSetConditional(lua_State *L, json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveBreakClear(lua_State *L, json_t* node)
{
	// Get the breakpoint data.
	json_t* np_file = json_object_get(node, "FileName");
	if (np_file == NULL || json_typeof(np_file) != JSON_STRING)
		return;
	json_t* np_line = json_object_get(node, "LineNumber");
	if (np_line == NULL || json_typeof(np_line) != JSON_INTEGER)
		return;

	// Store breakpoint data.
	std::string file = std::string(json_string_value(np_file));
	int line = ( int )json_integer_value(np_line);

	// Find and remvoe the breakpoint
	for (std::vector<MOAIBreakpoint>::const_iterator i = MOAIHarness::mBreakpoints.begin(); i < MOAIHarness::mBreakpoints.end(); i++)
	{
		if ((*i).filename == file && (*i).line == line)
		{
			MOAIHarness::mBreakpoints.erase(i);
			break;
		}
	}

}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveVariableGet(lua_State *L, json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveVariableSet(lua_State *L, json_t* node)
{
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveEvaluate(lua_State *L, json_t* node)
{
	// Get the evaluation.
	json_t* np_eval = json_object_get(node, "Evaluation");
	if (np_eval == NULL || json_typeof(np_eval) != JSON_STRING)
		return;

	// Get the current stack index so we know how many
	// positions to clear afterward.
	int top = lua_gettop(L);

	// Perform the evaluation with the Lua state.
	luaL_dostring(L, json_string_value(np_eval));

	// Now unload all of the things we just put on the stack
	// until the stack is the same size.
	std::string indent = "    ";
	json_t* result = MOAIHarness::ConvertStackPartialToJSON(L, top + 1, lua_gettop(L));
	lua_pop(L, lua_gettop(L) - top);

	// Send the message back to the IDE.
	MOAIHarness::SendResult(result);
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveMessage(lua_State *L)
{
	// Receive a single message from the TCP socket and then
	// delegate the data to one of the ReceiveX function.s
	int terminator = -1;
	while (true) {

		// Look for a double null terminator
		for (size_t i = 1; i < mSocketInBuffer.size(); ++i) {
			if (mSocketInBuffer[i - 1] == '\0' && mSocketInBuffer[i] == '\0') {
				terminator = i;
				break;
			}
		}
		if (terminator >= 0)
			break;

		// Get the data.
		char buffer[256];
		memset(&buffer, 0, sizeof(buffer));
		int bytes = recv(MOAIHarness::mSocketID, &buffer[0], 256, 0);
		if (bytes == 0 || bytes == SOCKET_ERROR)
			return;

		// Append the data to our input buffer
		mSocketInBuffer.reserve(bytes);
		for (int i = 0; i < bytes; i += 1)
			mSocketInBuffer.push_back(buffer[i]);
		
	}

	// Copy the message and erase it from the input buffer
	std::string json(&mSocketInBuffer[0], terminator);
	mSocketInBuffer.erase(mSocketInBuffer.begin(), mSocketInBuffer.begin() + terminator + 1);

	// We have received a message.  Parse the JSON to work out
	// exactly what type of message it is.
	json_t* node = json_loads(json.c_str(), 0, NULL);
	
	// Get a reference to the ID value.
	json_t* np_id = json_object_get(node, "ID");
	if (np_id == NULL || json_typeof(np_id) != JSON_STRING)
		return; // Unknown message type.
	
	printf("debug harness: Received message with ID %s.\n", json_string_value(np_id));

	// Check the type of message.
	if (std::string(json_string_value(np_id)) == "continue")
		MOAIHarness::ReceiveContinue(L, node);
	else if (std::string(json_string_value(np_id)) == "pause")
		MOAIHarness::ReceivePause(L, node);
	else if (std::string(json_string_value(np_id)) == "stop")
		MOAIHarness::ReceiveStop(L, node);
	else if (std::string(json_string_value(np_id)) == "break_set_always")
		MOAIHarness::ReceiveBreakSetAlways(L, node);
	else if (std::string(json_string_value(np_id)) == "break_set_conditional")
		MOAIHarness::ReceiveBreakSetConditional(L, node);
	else if (std::string(json_string_value(np_id)) == "break_clear")
		MOAIHarness::ReceiveBreakClear(L, node);
	else if (std::string(json_string_value(np_id)) == "variable_get")
		MOAIHarness::ReceiveVariableGet(L, node);
	else if (std::string(json_string_value(np_id)) == "variable_set")
		MOAIHarness::ReceiveVariableSet(L, node);
	else if (std::string(json_string_value(np_id)) == "evaluate")
		MOAIHarness::ReceiveEvaluate(L, node);
	json_decref(node);
}

//----------------------------------------------------------------//
json_t*	MOAIHarness::ConvertCallStackToJSON(lua_State* L, int level)
{
	lua_Debug ar;
	json_t* stack = json_array();
	while ( lua_getstack ( L, level++, &ar ))
	{
		lua_getinfo ( L, "Snlu", &ar );
		json_t* traceLine = json_object();
		json_object_set_new(traceLine, "Source", json_string(ar.short_src));
		json_object_set_new(traceLine, "LineDefined", json_integer(ar.linedefined));
		json_object_set_new(traceLine, "LastLineDefined", json_integer(ar.lastlinedefined));
		json_object_set_new(traceLine, "What", json_string(ar.what));
		json_object_set_new(traceLine, "Name", json_string(ar.name));
		json_object_set_new(traceLine, "CurrentLine", json_integer(ar.currentline));
		json_object_set_new(traceLine, "NameWhat", json_string(ar.namewhat));
		json_object_set_new(traceLine, "NUps", json_integer(ar.nups));
		json_array_append_new(stack, traceLine);
	}
	return stack;
}

//----------------------------------------------------------------//
json_t* MOAIHarness::ConvertStackToJSON(lua_State * L)
{
	// Just convert the whole stack.
	return MOAIHarness::ConvertStackPartialToJSON(L, 1, lua_gettop(L));
}

//----------------------------------------------------------------//
json_t* MOAIHarness::ConvertStackPartialToJSON(lua_State * L, int start, int end)
{
	// Check to see whether start and end are the same; if they are
	// we don't need to create a JSON object to hold all the values
	// since there will only be one returned.
	if (start == end)
		return MOAIHarness::ConvertStackIndexToJSON(L, start);

	// Otherwise, create a new JSON array to hold all of the stack values.
	json_t* holder = json_array();

	// Loop through all of the stack positions, adding them to the JSON
	// object.
	for (int i = start; i <= end; i += 1)
		json_array_append_new(holder, MOAIHarness::ConvertStackIndexToJSON(L, i));
	
	// Now return the array.
	return holder;
}

//----------------------------------------------------------------//
json_t* MOAIHarness::json_datapair(const char* name, json_t* data)
{
	// Note: This function steals a JSON reference from data (i.e.
	//       it does not increase data's reference count).

	// Returns a "datapair".  We need some way of returning
	// functions, userdata and threads as "special" objects in
	// JSON, but if we were to do a direct translation of
	// Lua tables -> JSON object, we'd have no way of indicating
	// this.  Therefore, tables, functions, userdata and threads
	// are all wrapped in "datapairs" which is a simple JSON
	// object containing name and another JSON data.
	json_t* holder = json_object();
	json_object_set_new(holder, "__datapair_anticonflict",                                // Prevents the developer accidently creating
		json_string("xwhTg0h14MfM56ekKEJ4cmgyeSAwBTINyR2F9Q314wb7Er6y8AmARbCWuaWgSdO"));  // a construct like an internal datapair.
	json_object_set_new(holder, "__datapair_name", json_string(name));
	json_object_set_new(holder, "__datapair_data", data);
	return holder;
}

//----------------------------------------------------------------//
json_t* MOAIHarness::ConvertStackIndexToJSON(lua_State * L, int idx, std::vector<const void*> * carried_references)
{
	// Check to see if idx is negative.
	if (idx < 0)
		idx = lua_gettop(L) + (idx + 1);

	// What we do is going to be based on what is located at
	// the specified index.
	switch (lua_type(L, idx))
	{
	case LUA_TNIL:
		return json_null();
	case LUA_TNUMBER:
		return json_real(lua_tonumber(L, idx));
	case LUA_TSTRING:
		return json_string(lua_tostring(L, idx));
	case LUA_TBOOLEAN:
		return (lua_toboolean(L, idx) == 0) ? json_false() : json_true();
	case LUA_TFUNCTION:
		return json_datapair("function", json_integer((int)lua_topointer(L, idx)));
	case LUA_TUSERDATA:
		return json_datapair("userdata", json_integer((int)lua_topointer(L, idx)));
	case LUA_TTHREAD:
		return json_datapair("thread", json_integer((int)lua_topointer(L, idx)));
	case LUA_TLIGHTUSERDATA:
		return json_datapair("lightuserdata", json_integer((int)lua_topointer(L, idx)));
	case LUA_TTABLE:
		// Unlike other data values, table must be recursively evaluated.  In addition
		// we must check for circular references since it's possible they may occur.
		char s[LUAI_MAXNUMBER2STR];
		std::vector<const void*> * references = (carried_references == NULL) ? new std::vector<const void*>() : carried_references;
		json_t* holder = json_object();
		lua_pushnil(L);
		while (lua_next(L, idx) != 0)
		{
			// Key is at index -2
			// Value is at index -1
			json_t* key = NULL;
			json_t* value = NULL;

			// Safely convert the key into a string if needed (we
			// can't use lua_tostring with lua_next).
			if (lua_isnumber(L, -2))
			{
				lua_Number n = lua_tonumber(L, -2);
				lua_number2str(s, n);
				key = json_string((const char*)&s);
			}
			else
				key = json_string(lua_tostring(L, -2));

			// Recursively convert the value ONLY if it doesn't
			// appear in our references vector.
			bool evaluate = true;
			if (lua_type(L, -1) == LUA_TTABLE)
			{
				const void* ptr = lua_topointer(L, -1);
				for (std::vector<const void*>::iterator it = references->begin(); it != references->end(); ++it)
				{
					if (*it == ptr)
					{
						evaluate = false;
						break;
					}
				}
			}

			// Now evaluate the value if we should.
			if (evaluate)
			{
				if (lua_type(L, -1) == LUA_TTABLE)
					references->insert(references->end(), lua_topointer(L, -1));
				value = MOAIHarness::ConvertStackIndexToJSON(L, -1, references);
			}
			else
				value = json_datapair("recursive", json_integer((int)lua_topointer(L, -1)));

			// Assign the key value pair to the holder object.
			json_object_set(holder, json_string_value(key), value);
			json_decref(key);
			json_decref(value);

			// Pop the value from the stack (lua_next will consume
			// the key on the next iterator).
			lua_pop(L, 1);
		}
		if (carried_references == NULL)
			delete references;
		return holder;
	}
	return json_datapair("unknown", json_integer((int)lua_topointer(L, idx)));
}

//----------------------------------------------------------------//
json_t* MOAIHarness::ConvertStackIndexToJSON(lua_State * L, int idx)
{
	return MOAIHarness::ConvertStackIndexToJSON(L, idx, NULL);
}