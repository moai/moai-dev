// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHARNESS_H
#define	MOAIHARNESS_H

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
	std::string filename;
	unsigned int line;

	inline MOAIBreakpoint(std::string f, unsigned int l)
	{
		filename = f;
		line = l;
	}
};

//================================================================//
// MOAIHarness
//================================================================//
/**	@name	MOAIHarness
	@text	Internal debugging and hooking class.
*/
class MOAIHarness {
private:

	// socket information
	static int		mSocketID;
	static struct sockaddr_in mSocketAddr;

	// breakpoint information
	static std::vector<MOAIBreakpoint> mBreakpoints;

	// pausing and waiting
	static bool     mEnginePaused;
	static void		Pause();

	// callbacks
	static void		Callback(lua_State *L, lua_Debug *ar);

	// message sending
	static void     SendWait();
	static void     SendBreak(std::string func, unsigned int line, std::string file);
	static void     SendMessage(std::string data);

	// message receiving
	static void		ReceiveContinue(json_t* node);
	static void		ReceivePause(json_t* node);
	static void		ReceiveStop(json_t* node);
	static void		ReceiveBreakSetAlways(json_t* node);
	static void		ReceiveBreakSetConditional(json_t* node);
	static void		ReceiveBreakClear(json_t* node);
	static void		ReceiveVariableGet(json_t* node);
	static void		ReceiveVariableSet(json_t* node);
	static void		ReceiveEvaluate(json_t* node);
	static void		ReceiveMessage();

public:

	// hook function
	static void		HookLua(lua_State* L, const char* target, int port);

};

#endif
