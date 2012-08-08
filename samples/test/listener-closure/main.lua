----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAISim.forceGarbageCollection ()
MOAISim.setHistogramEnabled ( true )

local function callWithDelay ( func, delay )

	local callWithDelayTimer = MOAITimer.new ()
	callWithDelayTimer:setSpan ( delay )
	
	callWithDelayTimer:setListener ( MOAITimer.EVENT_TIMER_LOOP, 

		function ()

			callWithDelayTimer:stop ()
			callWithDelayTimer = nil
			
			func ()
		end
	)
	
	callWithDelayTimer:start ()
end

local function makeTableForEnclosing ()

	local anims = {}
	local sprites = {}
	for i = 1, 5 do
		
		sprites [ i ] = MOAIProp2D.new ()
		anims [ i ] = MOAIAnim.new ()
		
		callWithDelay ( function () anims [ i ]:start () end, math.random () )
	end
end

for i = 1, 3 do
	
	makeTableForEnclosing ()
	
	print ( "---- Iteration " .. i .. " ----" )
	MOAISim.forceGarbageCollection ()	
	MOAISim.reportHistogram ()	
end

print ( "---- FINAL ----" )
MOAISim.forceGarbageCollection ()	
MOAISim.reportHistogram ()