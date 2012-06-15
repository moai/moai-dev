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
// Internal constants
//================================================================//

static const char JSON_DATAPAIR_ANTICONFLICT_KEY[] = "__datapair_anticonflict";
static const char JSON_DATAPAIR_ANTICONFLICT_VALUE[] = "xwhTg0h14MfM56ekKEJ4cmgyeSAwBTINyR2F9Q314wb7Er6y8AmARbCWuaWgSdO";
static const char JSON_DATAPAIR_NAME_KEY[] = "__datapair_name";
static const char JSON_DATAPAIR_DATA_KEY[] = "__datapair_data";


//================================================================//
// MOAIPathDictionary
//================================================================//

//----------------------------------------------------------------//
MOAIPathDictionary::MOAIPathDictionary() :
	mCount(0)
{
	const size_t INITIAL_SIZE = 1 << 6; // Must be a power of 2
	mBuckets.resize(INITIAL_SIZE, -1);
	mEntries.resize(INITIAL_SIZE);
}

//----------------------------------------------------------------//
int MOAIPathDictionary::GetIdentifier(const char path[])
{
	size_t pathLength;
	unsigned int pathHash = GetHash(path, &pathLength);
	int id = FindIdentifierRelative(pathHash, path);
	if (id < 0)
	{
		std::string absPathStr = USFileSys::GetAbsoluteFilePath(path);
		size_t absPathLength;
		unsigned int absPathHash = GetHash(absPathStr.c_str(), &absPathLength);
		id = FindIdentifierAbsolute(absPathHash, absPathStr.c_str());
		if (id < 0)
		{
			id = MakeIdentifier(absPathStr.c_str(), absPathLength);
			SetIdentifier(absPathHash, -1, id);
		}
		int idRel = MakeIdentifier(path, pathLength);
		SetIdentifier(pathHash, idRel, id);
	}
	return id;
}

//----------------------------------------------------------------//
unsigned int MOAIPathDictionary::GetHash(const char path[], size_t* pathLength)
{
	// Using djb2 (see http://www.cse.yorku.ca/~oz/hash.html)
	unsigned long hash = 5381;
	int c;
	for (*pathLength = 0; c = path[*pathLength]; ++*pathLength)
		hash = ((hash << 5) + hash) + c; // hash * 33 + c
	return hash;
}

//----------------------------------------------------------------//
int MOAIPathDictionary::FindIdentifierRelative(unsigned int hash, const char path[])
{
	size_t bucketIndex = hash & (mBuckets.size() - 1);
	for (int cursor = mBuckets[bucketIndex]; cursor >= 0; cursor = mEntries[cursor].next)
	{
		const MOAIPathDictionary::Entry& entry = mEntries[cursor];
		if (entry.hash == hash && strcmp(path, &mStringPool[entry.idRel]) == 0)
		{
			return entry.idAbs;
		}
	}
	return -1;
}

//----------------------------------------------------------------//
int MOAIPathDictionary::FindIdentifierAbsolute(unsigned int hash, const char path[])
{
	size_t bucketIndex = hash & (mBuckets.size() - 1);
	for (int cursor = mBuckets[bucketIndex]; cursor >= 0; cursor = mEntries[cursor].next)
	{
		const MOAIPathDictionary::Entry& entry = mEntries[cursor];
		if (entry.hash == hash && strcmp(path, &mStringPool[entry.idAbs]) == 0)
		{
			return entry.idAbs;
		}
	}
	return -1;
}

//----------------------------------------------------------------//
int MOAIPathDictionary::MakeIdentifier(const char path[], size_t length)
{
	int id = mStringPool.size();
	mStringPool.insert(mStringPool.end(), path, path + length + 1);
	return id;
}

//----------------------------------------------------------------//
void MOAIPathDictionary::SetIdentifier(unsigned int hash, int idRel, int idAbs)
{
	size_t bucketIndex = hash & (mBuckets.size() - 1);
	if (mCount == mEntries.size())
	{
		Grow();
		bucketIndex = hash & (mBuckets.size() - 1);
	}
	Entry& entry = mEntries[mCount];
	entry.hash = hash;
	entry.next = mBuckets[bucketIndex];
	entry.idRel = idRel;
	entry.idAbs = idAbs;
	mBuckets[bucketIndex] = mCount;
	++mCount;
}

//----------------------------------------------------------------//
void MOAIPathDictionary::Grow()
{
	size_t newSize = mEntries.size() << 1;
	mEntries.resize(newSize);

	std::vector<int> newBuckets;
	newBuckets.resize(newSize, -1);
	for (int i = 0; i < mCount; ++i)
	{
		Entry& entry = mEntries[i];
		size_t bucketIndex = entry.hash & (newSize - 1);
		entry.next = newBuckets[bucketIndex];
		newBuckets[bucketIndex] = i;
	}
	mBuckets.swap(newBuckets);
}


//================================================================//
// MOAIHarness
//================================================================//

//----------------------------------------------------------------//
int MOAIHarness::mSocketID = -1;
bool MOAIHarness::mEnginePaused = false;
struct sockaddr_in MOAIHarness::mSocketAddr;
MOAIPathDictionary MOAIHarness::mPathDictionary;
std::vector<MOAIBreakpoint> MOAIHarness::mBreakpoints;
std::vector<char> MOAIHarness::mSocketInBuffer;
int MOAIHarness::mStepMode = 0;
lua_State *MOAIHarness::mStepState = NULL;
int MOAIHarness::mStepDepth = 0;

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

	bool needBreak = false;

	// Handle step operations
	if (MOAIHarness::mStepMode != MOAIHarness::RUN && MOAIHarness::mStepState == L)
	{
		// Update our stack depth
		switch (ar->event)
		{
		case LUA_HOOKCALL:
			++MOAIHarness::mStepDepth;
			return;
		case LUA_HOOKRET:
		case LUA_HOOKTAILRET:
			--MOAIHarness::mStepDepth;
			return;
		}

		// Determine whether this qualifies as a step based on our step mode
		switch (MOAIHarness::mStepMode)
		{
		case MOAIHarness::STEP_OVER:
			if (MOAIHarness::mStepDepth <= 0)
				needBreak = true;
			break;
		case MOAIHarness::STEP_OUT:
			if (MOAIHarness::mStepDepth < 0)
				needBreak = true;
			break;
		case MOAIHarness::STEP_INTO:
			needBreak = true;
			break;
		}
	}

	// Compare against any currently-set breakpoints
	if (!needBreak && ar->event == LUA_HOOKLINE && source[0] == '@' && !MOAIHarness::mBreakpoints.empty())
	{
		int identifier = MOAIHarness::mPathDictionary.GetIdentifier(source + 1);
		for (std::vector<MOAIBreakpoint>::const_iterator i = MOAIHarness::mBreakpoints.begin(); i < MOAIHarness::mBreakpoints.end(); i++)
		{
			if ((*i).identifier == identifier && (*i).line == currentline)
			{
				needBreak = true;
				break;
			}
		}
	}

	// Trigger a break if necessary
	if (needBreak) {
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
	}
}

//----------------------------------------------------------------//
int MOAIHarness::VariableGetCallback(lua_State* L)
{
	lua_gettable(L, -2);
	return 1;
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
    	json_t* stack = ConvertCallStackToJSON(L, 0);
        MOAIHarness::SendError(message, stack, level);
        MOAIHarness::Pause(L);
    }
}

//----------------------------------------------------------------//
void MOAIHarness::Pause(lua_State * L)
{
	printf("debug harness: Waiting to receive messages from debugging interface.\n");
	MOAIHarness::mEnginePaused = true;
	MOAIHarness::mStepMode = MOAIHarness::RUN;
	MOAIHarness::mStepState = NULL;
	MOAIHarness::mStepDepth = 0;
	while (MOAIHarness::mEnginePaused)
	{
		// Receive and handle IDE messages until we get a continue.
		MOAIHarness::ReceiveMessage(L);
	}
	printf("debug harness: Continuing execution of program.\n");
	MOAIHarness::SendResume();
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
void MOAIHarness::SendResume()
{
	// Sends a resume signal to the IDE.
	std::string resume;
	resume = "{\"ID\":\"resume\"}";
	MOAIHarness::SendMessage(resume);
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
void MOAIHarness::SendError(std::string message, json_t* stack, int level)
{
    // Sends an "error occurred" signal to the IDE.
	json_t* msg = json_object();
	json_object_set_new(msg, "ID", json_string("error"));
	json_object_set_new(msg, "Message", json_string(message.c_str()));
	json_object_set_new(msg, "Level", json_integer(level));
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
void MOAIHarness::SendVariableGetResult(json_t* keys, json_t* result)
{
	// Sends a "result of evaluation" signal to the IDE.
	json_t* msg = json_object();
	json_object_set_new(msg, "ID", json_string("variable_get_result"));
	json_object_set_new(msg, "Keys", keys);
	json_object_set_new(msg, "Value", result);
	char* data = json_dumps(msg, 0);
	MOAIHarness::SendMessage(std::string(data));
	free(data);
}

//----------------------------------------------------------------//
void MOAIHarness::SendMessage(std::string data)
{
	// Add the terminators.
	const char* raw = data.c_str();

	// Send the data in 256-byte chunks.
	for (unsigned int offset = 0; offset <= data.length(); offset += 256)
	{
		// Figure out how much to copy
		int size = data.length() - offset;
		if (size > 256)
			size = 256;

		// Copy this portion of the string into the buffer
		char buffer[256];
		memcpy(buffer, raw + offset, size);
		if (size < 256)
		{
			buffer[size] = '\0';
			++size;
		}

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
	const char* file = json_string_value(np_file);
	int line = ( int )json_integer_value(np_line);

	// Add the breakpoint.
	int identifier = MOAIHarness::mPathDictionary.GetIdentifier(file);
	mBreakpoints.insert(mBreakpoints.begin(), MOAIBreakpoint(identifier, line));
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
	const char* file = json_string_value(np_file);
	int line = ( int )json_integer_value(np_line);

	// Find and remove the breakpoint
	int identifier = MOAIHarness::mPathDictionary.GetIdentifier(file);
	for (std::vector<MOAIBreakpoint>::const_iterator i = MOAIHarness::mBreakpoints.begin(); i < MOAIHarness::mBreakpoints.end(); i++)
	{
		if ((*i).identifier == identifier && (*i).line == line)
		{
			MOAIHarness::mBreakpoints.erase(i);
			break;
		}
	}

}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveStep(lua_State *L, int executionMode)
{
	// Don't set a step mode if we're not currently suspended
	if (!MOAIHarness::mEnginePaused)
		return;

	// Don't override an pending step mode
	if (MOAIHarness::mStepMode != MOAIHarness::RUN)
		return;

	// Store our step mode and resume execution
	MOAIHarness::mStepMode = executionMode;
	MOAIHarness::mStepState = L;
	MOAIHarness::mStepDepth = 0;
	MOAIHarness::mEnginePaused = false;
}

//----------------------------------------------------------------//
void MOAIHarness::ReceiveVariableGet(lua_State *L, json_t* node)
{
	// Get the keys to traverse to the target variable
	json_t* np_keys = json_object_get(node, "Keys");
	if (np_keys == NULL || json_typeof(np_keys) != JSON_ARRAY)
		return;
	
	// Check to see if we were actually provided keys
	size_t keyCount = json_array_size(np_keys);
	if (keyCount == 0)
		return;

	// Get the root key name (which has to be a string)
	json_t* np_root_key = json_array_get(np_keys, 0);
	if (json_typeof(np_root_key) != JSON_STRING)
		return;
	std::string root(json_string_value(np_root_key));

	// Get the stack level to look for loval variables at
	json_t* np_stack_level = json_object_get(node, "Level");
	if (np_stack_level == NULL || json_typeof(np_stack_level) != JSON_INTEGER)
		return;
	json_int_t level = json_integer_value(np_stack_level);

	// Check for the root name in the local namespace
	bool found = false;
	lua_Debug ar;
	if (lua_getstack(L, (int)level, &ar))
	{
		// Look for a local variable match
		const char* localName;
		for (int local = 1; (localName = lua_getlocal(L, &ar, local)) != 0; ++local)
		{
			if (root == localName)
			{
				found = true;
				break;
			}
			lua_pop(L, 1);
		}

		if (!found)
		{
			// Push the function onto the stack
			lua_getinfo(L, "f", &ar);

			// Look for an upvalue match
			const char* upvalueName;
			for (int upvalue = 1; (upvalueName = lua_getupvalue(L, -1, upvalue)) != 0; ++upvalue)
			{
				if (root == upvalueName)
				{
					found = true;
					break;
				}
				lua_pop(L, 1);
			}

			// Pop the function off the stack
			lua_remove(L, found ? -2 : -1);
		}
	}

	// Check for the root name in the global namespace
	if (!found)
	{
		lua_getglobal(L, root.c_str());
	}

	// Traverse through our child keys
	for (size_t childIndex = 1; !lua_isnil(L, -1) && childIndex < keyCount; ++childIndex)
	{
		json_t* np_key = json_array_get(np_keys, childIndex);
		bool valid = true;

		// First check for complex type keys
		if (json_typeof(np_key) == JSON_OBJECT)
		{
			// Check for the correct datapair encoding for keys
			json_t* anticonflict = json_object_get(np_key, JSON_DATAPAIR_ANTICONFLICT_KEY);
			json_t* np_type = json_object_get(np_key, JSON_DATAPAIR_NAME_KEY);
			json_t* np_data = json_object_get(np_key, JSON_DATAPAIR_DATA_KEY);
			bool isDatapair =
				anticonflict && json_typeof(anticonflict) == JSON_STRING && strcmp(json_string_value(anticonflict), JSON_DATAPAIR_ANTICONFLICT_VALUE) == 0 &&
				np_type && json_typeof(np_type) == JSON_STRING &&
				np_data && json_typeof(np_data) == JSON_INTEGER;				
			if (isDatapair && lua_istable(L, -1))
			{
				// Iterate through the keys of the table, looking for complex
				// types with the same type and address
				const char* keyType = json_string_value(np_type);
				json_int_t keyPtr = json_integer_value(np_data);
				bool keyFound = false;
				lua_pushnil(L);
				while (lua_next(L, -2) != 0)
				{
					// If this key is a complex type, compare its
					// type and address
					if (const void* ptr = lua_topointer(L, -2))
					{
						int type = lua_type(L, -2);
						if ((int)ptr == (int)keyPtr && strcmp(keyType, lua_typename(L, type)) == 0)
						{
							// Pop the table and key off the stack, leaving the value
							lua_remove(L, -2);
							lua_remove(L, -2);
							keyFound = true;
							break;
						}
					}
					lua_pop(L, 1);
				}
				if (keyFound)
				{
					continue;
				}
			}
			valid = false;
		}

		// Push the key onto the stack
		switch (json_typeof(np_key))
		{
		case JSON_STRING:
			lua_pushstring(L, json_string_value(np_key));
			break;
		case JSON_INTEGER:
			lua_pushinteger(L, (lua_Integer)json_integer_value(np_key));
			break;
		case JSON_REAL:
			lua_pushnumber(L, (lua_Number)json_real_value(np_key));
			break;
		case JSON_TRUE:
		case JSON_FALSE:
			lua_pushboolean(L, json_typeof(np_key) == JSON_TRUE);
			break;
		default:
			valid = false;
			break;
		}

		// If we don't have a valid key, just pop the table off the
		// stack and return a nil value
		if (!valid)
		{
			lua_pop(L, 1);
			lua_pushnil(L);
		}
		
		// Otherwise, call a function to get the table value
		else
		{
			lua_pushcfunction(L, MOAIHarness::VariableGetCallback);
			lua_insert(L, -3);

			// If an error occurred getting the value, just push nil
			if (lua_pcall(L, 2, 1, 0))
			{
				lua_pushnil(L);
			}
		}
	}

	// Convert the result to a json object
	json_t* result = MOAIHarness::ConvertStackIndexToJSON(L, -1, true);
	lua_pop(L, 1);

	// Return the result to the caller
	SendVariableGetResult(np_keys, result);
	json_decref(result);

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

	// Load the string from the message
	MOAILuaStateHandle state ( L );
	int status = luaL_loadstring ( state, json_string_value(np_eval) );
	if ( state.PrintErrors ( USLog::CONSOLE, status ))
		return;

	// Call the string
	state.DebugCall ( 0, 0 );

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
	else if (std::string(json_string_value(np_id)) == "step_into")
		MOAIHarness::ReceiveStep(L, MOAIHarness::STEP_INTO);
	else if (std::string(json_string_value(np_id)) == "step_over")
		MOAIHarness::ReceiveStep(L, MOAIHarness::STEP_OVER);
	else if (std::string(json_string_value(np_id)) == "step_out")
		MOAIHarness::ReceiveStep(L, MOAIHarness::STEP_OUT);
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
		json_object_set_new(traceLine, "Source", json_string(ar.source));
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
	json_object_set_new(holder, JSON_DATAPAIR_ANTICONFLICT_KEY, // Prevents the developer accidently creating
		json_string(JSON_DATAPAIR_ANTICONFLICT_VALUE));			// a construct like an internal datapair.
	json_object_set_new(holder, JSON_DATAPAIR_NAME_KEY, json_string(name));
	json_object_set_new(holder, JSON_DATAPAIR_DATA_KEY, data);
	return holder;
}

//----------------------------------------------------------------//
json_t* MOAIHarness::ConvertStackIndexToJSON(lua_State * L, int idx, bool shallow, std::vector<const void*> * carried_references)
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
		// Todo: fix pointer encoding for datapairs so that this will work
		// correctly on 64 bit systems
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
		json_t* holder = shallow ? json_array() : json_object();
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
				if (shallow)
				{
					key = json_real(n);
				}
				else
				{
					lua_number2str(s, n);
					key = json_string((const char*)&s);
				}
			}
			else if (lua_isboolean(L, -2))
			{
				key = json_string(lua_toboolean(L, -2) ? "true" : "false");
			}
			else if (!lua_isstring(L, -2))
			{
				int type = lua_type(L, -2);
				key = json_datapair(lua_typename(L, type), json_integer((int)lua_topointer(L, -2)));
			}
			else
				key = json_string(lua_tostring(L, -2));

			// If only a shallow result is requested, just add the key to an array
			if (shallow)
			{
				json_array_append_new(holder, key);
				lua_pop(L, 1);
				continue;
			}

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
				value = MOAIHarness::ConvertStackIndexToJSON(L, -1, shallow, references);
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
