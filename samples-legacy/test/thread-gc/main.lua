----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

function spin ()
	while true do
		coroutine.yield ()
	end
end

function main1 ( self )

	print ( 'thread 1' )
	coroutine.yield ()
	
	self:stop ()
	
	spin ()
end

thread1 = MOAIThread.new ()
thread1:run ( main1, thread1 )
thread1 = nil

function main2 ()

	print ( 'thread 2' )
	coroutine.yield ()
	
	spin ()
end

thread2 = MOAIThread.new ()
thread2:run ( main2 )

function cleanup ()

	coroutine.yield ()
	coroutine.yield ()
	coroutine.yield ()

	thread2:stop ()
	thread2 = nil

	print ( MOAISim.getLuaObjectCount ())
	MOAISim.forceGarbageCollection ()
	print ( MOAISim.getLuaObjectCount ())
end

cleaner = MOAIThread.new ()
cleaner:run ( cleanup )
