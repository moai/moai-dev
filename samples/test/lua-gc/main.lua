----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

print ( 'original collectgarbage' )
collectgarbage ( 'collect' )

MOAISim.openWindow ( 'test', 320, 480 )

function testGC ( active, step )

	print ( '--------------------------------' )
	print ( 'TESTING GC' )
	print ( active, step )

	MOAISim.setGCActive ( active )
	MOAISim.setGCStep ( step )

	local done = false
	local node = MOAIScriptNode.new ()
	node:setFinalizer ( function () done = true end )
	node = nil
	repeat coroutine.yield () until done
end

function main ()

	print ( 'MOAISim collectgarbage' )
	collectgarbage ( 'collect' )

	MOAILuaRuntime.reportGC ( true )
	
	testGC ( true, 0 )
	testGC ( true, 200 )
	testGC ( false, 0 )
end

thread = MOAICoroutine.new ()
thread:run ( main )
