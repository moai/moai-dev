----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAISim.forceGarbageCollection ()
MOAISim.setHistogramEnabled ( true )

local function makeThreadClosure ( theLayer )

	local scrollUVTransform = MOAITransform.new ()
	local UVscroll = MOAIThread.new ()
	UVscroll:run (
		function ()
			while true do
				
				MOAIThread.blockOnAction ( scrollUVTransform:moveLoc ( 0, 40, 350, MOAIEaseType.LINEAR ) )
			end
		end
	)
end

for i = 1, 3 do
	
	makeThreadClosure ( )
	
	print ( "---- Iteration " .. i .. " ----" )
	MOAISim.forceGarbageCollection ()	
	MOAISim.reportHistogram ()	
end

print ( "---- FINAL ----" )
MOAISim.forceGarbageCollection ()	
MOAISim.reportHistogram ()