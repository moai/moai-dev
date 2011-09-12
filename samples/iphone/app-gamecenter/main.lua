----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

function scoresCallback ( scoreArray )
	print "in score callback"
	for i, v in ipairs ( scoreArray ) do print ( i, v ) end
end

MOAIGameCenter.authenticatePlayer ()
MOAIGameCenter.setGetScoresCallback ( scoresCallback )

--function threadFunc ()
--	score = 10483
--	while true do
--		coroutine.yield ()
		--MOAIGameCenter.getScores ( nil, MOAIGameCenter.PLAYERSCOPE_GLOBAL, MOAIGameCenter.TIMESCOPE_ALLTIME, 1, 10 )
		--MOAIGameCenter.reportScore ( score + math.random ( 23897 ), "sampleboard" )
--		MOAIGameCenter.getScores ( )
--	end
--end

--function threadFunc ()
--	nextframe = false
--	while true do
--		if nextframe == true then			
--			MOAIGameCenter.reportScore ( 54321, "sampleboard" )
--			print "done reporting"
--			break
--		elseif MOAIGameCenter.isSupported () then
--			nextframe = true
--		end
--		coroutine.yield ()
--	end
--end

function threadFunc ()
	score = 10483
	while true do
		coroutine.yield ()
		if MOAIGameCenter.isSupported () then
			--MOAIGameCenter.reportScore ( score + math.random ( 23897 ), "sampleboard" )
			MOAIGameCenter.showDefaultLeaderboard ()
			break
		end
	end
end

thread = MOAIThread.new ()
thread:run ( threadFunc )