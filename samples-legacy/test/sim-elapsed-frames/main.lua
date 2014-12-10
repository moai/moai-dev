----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAISim.setStep ( 1 / 60 )
-- MOAISim.clearLoopFlags ()
-- MOAISim.setLoopFlags ( MOAISim.LOOP_FLAGS_FIXED )

thread = MOAICoroutine.new ()
thread:run ( function()
    for frame = 1, 100 do
        print ( "Real frames", MOAISim.getElapsedFrames () )
        print ( "Sim frames", MOAISim.timeToFrames ( MOAISim.getElapsedTime () ) )
        print ()
        coroutine.yield()
    end
    os.exit()
end )