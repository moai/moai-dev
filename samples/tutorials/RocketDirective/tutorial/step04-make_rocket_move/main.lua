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
-- rocket 
--==============================================================
rocketGfx = MOAIGfxQuad2D.new ()
rocketGfx:setTexture ( "rocket.png" )
rocketGfx:setRect ( -32, -32, 32, 32 )

--------------------------------
function makeRocket ()

	local rocket = MOAIProp2D.new ()
	rocket:setDeck ( rocketGfx )
	layer:insertProp ( rocket )

	----------------
	function rocket:launch ()
	
		self.thread = MOAIThread:new ()
		
		self.thread:run (
			
			function ()
				MOAIThread.blockOnAction ( self:seekLoc ( baseX, baseY, 3.0, MOAIEaseType.LINEAR ))
			end	
		)
	end
	
	----------------
	return rocket
end

rocket = makeRocket ()
rocket:launch ()