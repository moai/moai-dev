----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

main = function ()

	print ( 'step 1' )

	MOAICoroutine.new ():run ( true, function () print ( 'thread1' ) end )
	MOAICoroutine.new ():run ( false, function () print ( 'thread2' ) end )

	MOAICoroutine.new ():run ( true, function ()

		print ( 'thread3' )

		MOAICoroutine.new ():run ( true, function () print ( 'thread4' ) end )
		MOAICoroutine.new ():run ( false, function () print ( 'thread5' ) end )

	end )

	coroutine.yield ()
	print ( 'step 2' )

	coroutine.yield ()
	print ( 'step 3' )

end

MOAICoroutine.new ():run ( main )
