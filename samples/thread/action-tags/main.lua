----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

done = false

function spawn ( text )
	local action = MOAICoroutine.new ()
	action:run ( function ()
		while not done do
			print ( text )
			coroutine.yield ()
		end
	end )
	return action
end

function main ()
 
    local action1 = spawn ( 'action1' )
	local action2 = spawn ( 'action2' )
	local action3 = spawn ( 'action3' )
	
	MOAIActionMgr.setActionTag ( action2, 'kill' )
	
	coroutine.yield ()
	
	MOAIActionMgr.stopActionsWithTag ( 'kill' )
	
	coroutine.yield ()
	
	done = true
end

MOAICoroutine.new ():run ( main )
