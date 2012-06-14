// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHARNESS_H
#define	MOAIHARNESS_H

#include <moaicore/moaicore.h>

#include <sstream>
#include <string>
#include <vector>
#include <jansson.h>
#include <lua.h>

//================================================================//
// MOAIBreakpoint
//================================================================//
/**	@name	MOAIBreakpoint
	@text	Internal breakpoint definition class.
*/
struct MOAIBreakpoint {
	int identifier;
	unsigned int line;

	inline MOAIBreakpoint(int i, unsigned int l)
	{
		identifier = i;
		line = l;
	}
};

class MOAIPathDictionary {
public:
	MOAIPathDictionary();
	int GetIdentifier(const char path[]);
private:
	static unsigned int GetHash(const char path[], size_t* pathLength);
	int FindIdentifierRelative(unsigned int hash, const char path[]);
	int FindIdentifierAbsolute(unsigned int hash, const char path[]);
	int MakeIdentifier(const char path[], size_t length);
	void SetIdentifier(unsigned int hash, int idRel, int idAbs);
	void Grow();

	struct Entry {
		unsigned int hash;
		int next;
		int idRel;
		int idAbs;
	};
	int mCount;
	std::vector<int> mBuckets;
	std::vector<Entry> mEntries;
	std::vector<char> mStringPool;
};

//================================================================//
// MOAIHarness
//================================================================//
/**	@name	MOAIHarness
	@text	Internal debugging and hooking class.
*/
class MOAIHarness :
	public MOAIGlobalClass< MOAIHarness, MOAILuaObject > {
private:

	enum {
		RUN,
		STEP_INTO,
		STEP_OVER,
		STEP_OUT
	};

	// socket information
	static int		mSocketID;
	static struct sockaddr_in mSocketAddr;
	static std::vector<char> mSocketInBuffer;

	// breakpoint information
	static MOAIPathDictionary mPathDictionary;
	static std::vector<MOAIBreakpoint> mBreakpoints;

	// execution mode
	static int mStepMode;
	static lua_State *mStepState;
	static int mStepDepth;

	// pausing and waiting
	static bool     mEnginePaused;
	static void		Pause(lua_State *L);

	// callbacks
	static void		Callback(lua_State *L, lua_Debug *ar);
	static int		VariableGetCallback(lua_State* L);

	// message sending
	static void     SendWait();
	static void		SendResume();
	static void     SendBreak(lua_State* L, std::string func, unsigned int line, std::string file);
	static void     SendError(std::string message, json_t* stack, int level);
	static void     SendResult(json_t* result);
	static void     SendVariableGetResult(json_t* keys, json_t* result);
	static void     SendMessage(std::string data);

	// message receiving
	static void		ReceiveContinue(lua_State *L, json_t* node);
	static void		ReceivePause(lua_State *L, json_t* node);
	static void		ReceiveStop(lua_State *L, json_t* node);
	static void		ReceiveBreakSetAlways(lua_State *L, json_t* node);
	static void		ReceiveBreakSetConditional(lua_State *L, json_t* node);
	static void		ReceiveBreakClear(lua_State *L, json_t* node);
	static void		ReceiveStep(lua_State *L, int executionMode);
	static void		ReceiveVariableGet(lua_State *L, json_t* node);
	static void		ReceiveVariableSet(lua_State *L, json_t* node);
	static void		ReceiveEvaluate(lua_State *L, json_t* node);
	static void		ReceiveMessage(lua_State *L);

	// value encoding
	static json_t*  json_datapair(const char* name, json_t* data);
	static json_t*	ConvertCallStackToJSON(lua_State* L, int level);
	static json_t*  ConvertStackToJSON(lua_State * L);
	static json_t*  ConvertStackPartialToJSON(lua_State * L, int start, int end);
	static json_t*  ConvertStackIndexToJSON(lua_State * L, int idx, bool shallow = false, std::vector<const void*> * carried_references = NULL);

	// Lua functions
	static int		_sendMessage(lua_State* L);

public:

	DECL_LUA_SINGLETON ( MOAIHarness )

					MOAIHarness			();
					~MOAIHarness		();
	void			RegisterLuaClass	(MOAILuaState& state);
	void			RegisterLuaFuncs	(MOAILuaState& state);
	void			HookLua				(lua_State* L, const char* target, int port);
	void			Update				(lua_State* L);
	void            HandleError         (const char* message, lua_State* L, int level);
};

#endif
