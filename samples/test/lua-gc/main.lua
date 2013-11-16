----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAISim.openWindow ( 'test', 320, 480 )

function onUpdate ( self )
	print ( 'update' )
end

function main ()

	print ( 'main' )

	local node = MOAIScriptNode.new ()
	node:setCallback ( onUpdate )
	node:scheduleUpdate ()
	node = nil
	
	collectgarbage ( 'collect' )
	coroutine.yield ()
	
	print ( 'done' )
end

thread = MOAICoroutine.new ()
thread:run ( main )
