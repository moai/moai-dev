--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

--==============================================================
-- set up
--==============================================================
print ( "hello, moai!" )

SCREEN_UNITS_X = 320 
SCREEN_UNITS_Y = 480
SCREEN_WIDTH = SCREEN_UNITS_X
SCREEN_HEIGHT = SCREEN_UNITS_Y

MOAISim.openWindow ( "Rocket Lobster", SCREEN_WIDTH, SCREEN_HEIGHT )

viewport = MOAIViewport.new ()
viewport:setScale ( SCREEN_UNITS_X, SCREEN_UNITS_Y )
viewport:setSize ( SCREEN_WIDTH, SCREEN_HEIGHT )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

--==============================================================
-- base
--==============================================================
lobsterGfx = MOAIGfxQuad2D.new ()
lobsterGfx:setTexture ( "openlobster.png" )
lobsterGfx:setRect ( -128, -128, 128, 128 )

baseX = 0
baseY = -240

base = MOAIProp2D.new ()
base:setDeck ( lobsterGfx )
base:setLoc ( baseX, baseY )
layer:insertProp ( base )

--==============================================================
-- explosion rig
--==============================================================
explosionGfx = MOAIGfxQuad2D.new ()
explosionGfx:setTexture ( "fire.png" )
explosionGfx:setRect ( -32, -32, 32, 32 )

--------------------------------
function makeExplosion ()

	local explosion = MOAIProp2D.new ()
	explosion:setDeck ( explosionGfx )
	layer:insertProp ( explosion )

	----------------
	function explosion:explode ( size )

		for i = 1, size do
			self:setFrame ( -i, -i, i, i )
			coroutine.yield ()
		end

		layer:removeProp ( explosion )
	end

	----------------
	return explosion
end

--==============================================================
-- rocket 
--==============================================================
rocketGfx = MOAIGfxQuad2D.new ()
rocketGfx:setTexture ( "rocket.png" )
rocketGfx:setRect ( -32, -32, 32, 32 )

ALLY_ROCKET = 1
ENEMY_ROCKET = 2

--------------------------------
function makeRocket ( rocketType )

	local rocket = MOAIProp2D.new ()
	rocket:setDeck ( rocketGfx )
	layer:insertProp ( rocket )
	
	----------------
	function rocket:launch ( targetX, targetY, travelTime )
	
		self.thread = MOAIThread:new ()
		
		self.thread:run (
			
			function ()
				
				-- wait for the rocket to travel all the way to its target
				MOAIThread.blockOnAction ( self:seekLoc ( targetX, targetY, travelTime, MOAIEaseType.LINEAR ))
				
				-- once rocket is at end of its path, hide it
				layer:removeProp ( self )
				
				-- spawn an explosion where the rocket is
				local explosion = makeExplosion ()
				explosion:setLoc ( self:getLoc () )

				if rocketType == ALLY_ROCKET then
					
					-- ally rocket hit end of its path, so it explodes
					explosion:explode ( 64 )
				else
					
					-- if enemy rocket gets to end of its path, the base is hit so do big explosion
					if rocketType == ENEMY_ROCKET then
						explosion:explode ( 128 )
					end
					
					-- hide the base
					layer:removeProp ( base )
				end
			end	
		)
	end
	
	----------------
	return rocket
end

--------------------------------
function launchRocket ( rocketType, startX, startY, targetX, targetY, travelTime )

	local rocketAngle = math.atan2 ( targetY - startY, targetX - startX ) * ( 180 / math.pi )

	local rocket = makeRocket ( rocketType )
	rocket:setRot ( rocketAngle + 90 )
	rocket:setLoc ( startX, startY )
	rocket:launch ( targetX, targetY, travelTime )
end

--------------------------------
function launchEnemyRocket ( startX, startY )

	local minTravelTime = 2.0
	local maxTravelTime = 3.0
	launchRocket ( ENEMY_ROCKET, startX, startY, baseX, baseY, math.random ( minTravelTime, maxTravelTime ))
end

--------------------------------
function launchAllyRocket ( targetX, targetY )

	local scalar = ( targetY + 240 ) / 480
	local travelTime = 2.0 * scalar
	launchRocket ( ALLY_ROCKET, baseX, baseY, targetX, targetY, travelTime )
end

--==============================================================
-- game loop
--==============================================================
mainThread = MOAIThread.new ()

mainThread:run ( 

	function ()

		local frames = 0

		while true do
		
			coroutine.yield ()
			frames = frames + 1
			
			if frames >= 90 then
				frames = 0
				launchEnemyRocket ( math.random ( -160, 160 ), 300 )
			end

			if MOAIInputMgr.device.mouseLeft:down () then
				launchAllyRocket ( layer:wndToWorld ( MOAIInputMgr.device.pointer:getLoc () ))
			end
		end
	end 
)