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

	local done = false
	local node = MOAIScriptNode.new ()
	node:setCallback ( onUpdate )
	node:scheduleUpdate ()
	node:setFinalizer ( function () print ( 'FINALIZING' ) done = true end )
	node = nil
	
	repeat coroutine.yield () until done
	
	print ( 'done' )
end

thread = MOAICoroutine.new ()
thread:run ( main )
