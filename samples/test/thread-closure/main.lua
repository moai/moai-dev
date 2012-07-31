----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAISim.forceGarbageCollection ()
MOAISim.setHistogramEnabled ( true )

local function makeThreadClosure ( theLayer )

	local thread = MOAIThread.new ()
	thread:run ( function () print ( 'test' ) end )
	return thread
end

for i = 1, 3 do
	
	local thread = makeThreadClosure ( )
	thread:stop ()
	thread = nil
	
	print ( "---- Iteration " .. i .. " ----" )
	MOAISim.forceGarbageCollection ()	
	MOAISim.reportHistogram ()	
end

print ( "---- FINAL ----" )
MOAISim.forceGarbageCollection ()	
MOAISim.reportHistogram ()