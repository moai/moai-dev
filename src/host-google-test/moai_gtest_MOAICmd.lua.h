// Copyright (c) 2017-2018 Cryptogogue, Inc. All Rights Reserved.
// http://cryptogogue.com

#define LUA_STR(...) #__VA_ARGS__

static const char* MOAICmd_lua = LUA_STR (

	handler = MOAITestCmdHandler.new ()

	print ( 'IMMEDIATE from Lua' )
	stream = MOAIImmediateCmdStream.new ()
	stream
		:_ ( MOAISharedTestCmdHandler )
		:foo ()
		:bar ()
		:baz ()
		:_ ( handler )
		:foo ()
		:bar ()
		:baz ()

	stream = MOAIRetainedCmdStream.new ()
	stream
		:_ ( MOAISharedTestCmdHandler )
		:foo ()
		:bar ()
		:baz ()
		:_ ( handler )
		:foo ()
		:bar ()
		:baz ()

	print ( 'RETAINED from Lua' )
	stream:executeBytecode ()
	print ()
);
