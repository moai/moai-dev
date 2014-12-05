--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

--==============================================================
-- set up
--==============================================================
SCREEN_UNITS_X = 320 
SCREEN_UNITS_Y = 480
SCREEN_WIDTH = SCREEN_UNITS_X
SCREEN_HEIGHT = SCREEN_UNITS_Y

MOAISim.openWindow ( "Rocket Directive", SCREEN_WIDTH, SCREEN_HEIGHT )

viewport = MOAIViewport.new ()
viewport:setScale ( SCREEN_UNITS_X, SCREEN_UNITS_Y )
viewport:setSize ( SCREEN_WIDTH, SCREEN_HEIGHT )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

--==============================================================
-- utility functions
--==============================================================

--------------------------------
function angle ( x1, y1, x2, y2 )

	return math.atan2 ( y2 - y1, x2 - x1 ) * ( 180 / math.pi )
end

--------------------------------
function distance ( x1, y1, x2, y2 )

	return math.sqrt ((( x2 - x1 ) ^ 2 ) + (( y2 - y1 ) ^ 2 ))
end

--------------------------------
function randomFloat ( minVal, maxVal )

	return minVal + (( maxVal - minVal ) * math.random () )
end

--------------------------------
function runInThread ( func, ... )

	local thread = MOAIThread:new ()
	thread:run ( func, ... )
	return thread
end

--==============================================================
-- decoration methods
-- used to share common functions between objects
--==============================================================

--------------------------------
function decorateAsVisibleObject ( object )

	----------------
	function object:setVisible ( visible )
	
		if visible then
			layer:insertProp ( self )
		else
			layer:removeProp ( self )
		end
	end
	
	----------------
	object:setVisible ( true )
end

--------------------------------
function decorateAsExplodableObject ( object )

	decorateAsVisibleObject ( object )

	----------------
	function object:explode ( radius )

		local explosion = makeExplosion ()
		explosion:setLoc ( self:getLoc () )
		explosion:explode ( radius )
	
		self:setVisible ( false )
		
		if self.cleanUp then
			self:cleanUp ()
		end

		self = nil
	end
end

--==============================================================
-- base rig
--==============================================================
lobsterGfx = MOAIGfxQuad2D.new ()
lobsterGfx:setTexture ( "openlobster.png" )
lobsterGfx:setRect ( -128, -128, 128, 128 )

--------------------------------
function makeBase ()

	local base = MOAIProp2D.new ()
	base:setDeck ( lobsterGfx )
	decorateAsExplodableObject ( base )
	return base
end

--==============================================================
-- game setup
--==============================================================
gameOver = false
baseX, baseY = 0, -240

base = makeBase ()
base:setLoc ( baseX, baseY )

enemy = { count = 0, rockets = {} }
ally = { count = 0, rockets = {} }

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

	----------------
	function explosion:cleanUp ()

		self:setVisible ( false )
	
		if self.thread then
			self.thread:stop ()
			self.thread = nil
		end
	end
	
	----------------
	function explosion:explode ( radius )

		radius = radius or 64
		
		self.thread = runInThread ( 
		
			function ()
				
				for i = 1, radius do

					coroutine.yield ()
					self:setRadius ( i )
				
					for rocket in pairs ( enemy.rockets ) do
						if self:hit ( rocket ) then
							rocket:explode ()
						end
					end
				end

				self:cleanUp ()
			end 
		)
	end
	
	----------------
	function explosion:hit ( prop )

		local x1, y1 = self:getLoc ()
		local x2, y2 = prop:getLoc ()
	
		return distance ( x1, y1, x2, y2 ) <= ( self.radius * 1.5 )
	end
	
	----------------
	function explosion:setRadius ( radius )
		
		self:setScl ( radius / 32 )
		self.radius = radius
	end
	
	----------------
	decorateAsVisibleObject ( explosion )
	return explosion
end

--==============================================================
-- rocket rig
--==============================================================
rocketGfx = MOAIGfxQuad2D.new ()
rocketGfx:setTexture ( "rocket.png" )
rocketGfx:setRect ( -32, -32, 32, 32 )

--------------------------------
function makeRocket ()

	local rocket = MOAIProp2D.new ()
	rocket:setDeck ( rocketGfx )
	
	----------------
	function rocket:cleanUp ()
	
		if self.thread then
			self.thread:stop ()
			self.thread = nil
		end
		
		self.group.rockets [ self ] = nil
	end
	
	----------------
	function rocket:launch ( targetX, targetY, delay )
	
		self.group.rockets [ self ] = self
		self.group.count = self.group.count + 1
		
		self.thread = runInThread (	
		
			function ()

				MOAIThread.blockOnAction ( self:seekLoc ( targetX, targetY, delay, MOAIEaseType.LINEAR ))
				self.group.count = self.group.count - 1
				
				if self.group == ally then
					self:explode ()
				else
					self:explode ( 128 )
					gameOver = true
				end
			end	
		)
	end
	
	----------------
	function rocket:setGroup ( group )
		self.group = group
	end
	
	----------------
	decorateAsExplodableObject ( rocket )
	return rocket
end

--------------------------------
function launchRocket ( group, x, y, targetX, targetY, delay )

	local rocket = makeRocket ()
	rocket:setGroup ( group )
	rocket:setRot ( angle ( x, y, targetX, targetY ) + 90 )
	rocket:setLoc ( x, y )
	rocket:launch ( targetX, targetY, delay )
end

--------------------------------
function launchEnemyRocket ( x, y )

	local minSpeed = 6.0
	local maxSpeed = 8.0
	
	launchRocket ( enemy, x, y, baseX, baseY, randomFloat ( minSpeed, maxSpeed ))
end

--------------------------------
function launchAllyRocket ( x, y )

	local maxSpeed = 3.0
	local delayScalar = (( y + ( SCREEN_UNITS_Y / 2 )) / SCREEN_UNITS_Y )
	
	launchRocket ( ally, baseX, baseY, x, y, maxSpeed * delayScalar )
end

--==============================================================
-- game over textbox rig
--==============================================================
function makeGameOverTextbox ()

	local rig = {}

	local font = MOAIFont.new ()
	font:loadFromTTF ( "arialbd.ttf", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.?! ", 12, 163 )

	local textbox = MOAITextBox.new ()
	textbox:setFont ( font )
	textbox:setRect ( -160, -80, 160, 80 )
	textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	textbox:setYFlip ( true )
	textbox:setString ( "You are dead!" )
	
	decorateAsVisibleObject ( textbox )
	textbox:setVisible ( false )
	
	----------------
	function rig:show ()
	
		textbox:setVisible ( true )
		textbox:spool ()
	end
	
	----------------
	return rig
end

gameOverTextbox = makeGameOverTextbox ()

--==============================================================
-- main game thread
--==============================================================
runInThread ( 

	function ()

		--------------------------------
		-- config
		--------------------------------
		local enemySpawnXMin = -( SCREEN_UNITS_X / 2 )
		local enemySpawnXMax = ( SCREEN_UNITS_X / 2 )
		local enemySpawnY = ( SCREEN_UNITS_Y / 2 ) * 1.25
		local enemySpawnDelay = 90
		local allyLimit = 1

		--------------------------------
		-- game loop
		--------------------------------
		local frames = 0

		while not gameOver do
		
			coroutine.yield ()
			frames = frames + 1
			
			if frames >= enemySpawnDelay then
				frames = 0
				launchEnemyRocket ( math.random ( enemySpawnXMin, enemySpawnXMax ), enemySpawnY )
			end

			if ( ally.count < allyLimit ) and MOAIInputMgr.device.mouseLeft:down () then
				launchAllyRocket ( layer:wndToWorld ( MOAIInputMgr.device.pointer:getLoc () ))
			end
		end

		--------------------------------
		-- game over
		--------------------------------
		base:setVisible ( false )
		gameOverTextbox:show ()
	end 
)