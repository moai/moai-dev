----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 1 )

MOAISim.openWindow ( "test", 320, 480 )

print ( 2 )

BASE_X_LOC = 0
BASE_Y_LOC = -176

ROCKET_SPEED = 0.75
ANTI_ROCKET_SPEED = 1.25
HIT_RADIUS = 32
EXPLOSION_STEPS = 64
ROCKET_SPAWN_HEIGHT = 200
ANTI_ROCKET_LAUNCH_DELAY = 60

mouseX = 0
mouseY = 0

launchCounter = ANTI_ROCKET_LAUNCH_DELAY

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )
print ( 3 )

lobsterGfx = MOAIGfxQuad2D.new ()
lobsterGfx:setTexture ( "openlobster.png" )
lobsterGfx:setRect ( -128, -128, 128, 128 )

explosionGfx = MOAIGfxQuad2D.new ()
explosionGfx:setTexture ( "fire.png" )
explosionGfx:setRect ( -32, -32, 32, 32 )

rocketGfx = MOAIGfxQuad2D.new ()
rocketGfx:setTexture ( "rocket.png" )
rocketGfx:setRect ( -32, -32, 32, 32 )

base = MOAIProp2D.new ()
base:setDeck ( lobsterGfx )
base:setLoc ( BASE_X_LOC, BASE_Y_LOC - 64 )
layer:insertProp ( base )
print ( 4 )

font =  MOAIFont.new ()
font:loadFromTTF ("arialbd.ttf", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.?! ", 12, 163 )

print ( 5 )

textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setRect ( -160, -80, 160, 80 )
textbox:setLoc ( 0, -160 )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
textbox:setYFlip ( true )
layer:insertProp ( textbox )
alive = true

local rockets = {}

function setVecLength ( dx, dy, len )

	local origLen = math.sqrt ( dx * dx + dy * dy )
	local scl = len / origLen
	return dx * scl, dy * scl, origLen / len
end

function hit ( prop1, prop2, dist )

	x1, y1 = prop1:getLoc ()
	x2, y2 = prop2:getLoc ()
	
	dx = x2 - x1
	dy = y2 - y1
	
	return dx * dx + dy * dy <= dist * dist

end

function setState ( prop, state, ... )

	if prop then
		if prop.finish then
			prop:finish ()
			prop.finish = nil
		end
		state ( prop, ... )
	end
end

function antiRocketState ( prop, x, y )

	if alive == true
		then
		prop:setDeck ( rocketGfx )
		prop:setLoc ( BASE_X_LOC, BASE_Y_LOC )
		prop:setScl ( 1, -1 )
		prop.state = antiRocketState
		layer:insertProp ( prop )
	end
	
	prop.dx, prop.dy, prop.steps = setVecLength (
		x - BASE_X_LOC,
		y - BASE_Y_LOC,
		ANTI_ROCKET_SPEED
	)
	
	function prop:finish ()
		prop.action:stop ()
		prop.action = nil
		prop.main = nil
	end
	
	function prop:main ()
		
		for i = 1, prop.steps do
			prop:addLoc ( prop.dx, prop.dy )
			coroutine.yield ()
		end
		
		setState ( prop, explosionState )
	end
	
	prop.action = MOAIThread.new ()
	prop.action:run ( prop.main, prop )
end

function explosionState ( prop )

	prop:setDeck ( explosionGfx )
	prop:setScl ( 1, 1 )
	prop.state = explosionState
	
	function prop:main ()
		
		for i = 1, EXPLOSION_STEPS do
		
			self:setScl ( i / 64 )
		
			local x
			local y
			x, y = self:getLoc ()
			
			for rocket, v in pairs ( rockets ) do
				if hit ( prop, rocket, i + HIT_RADIUS ) then
					setState ( rocket, explosionState )
				end			
			end
			
			coroutine.yield ()
		end
		
		layer:removeProp ( prop )
	end
	
	prop.action = MOAIThread.new ()
	prop.action:run ( prop.main, prop )
end

function rocketHitState ( prop )

	setState ( prop, explosionState )
	layer:removeProp ( base )
	alive = false
end

function rocketState ( prop, x, y )

	prop:setDeck ( rocketGfx )
	prop:setLoc ( x, y )
	prop.state = rocketState
	layer:insertProp ( prop )
	
	rockets [ prop ] = true
	
	prop.dx, prop.dy, prop.steps = setVecLength (
		BASE_X_LOC - x,
		BASE_Y_LOC - y,
		ROCKET_SPEED
	)
	
	function prop:finish ()
		prop.action:stop ()
		prop.action = nil
		prop.main = nil
		
		rockets [ prop ] = nil
	end
	
	function prop:main ()
		
		for i = 1, prop.steps do
			prop:addLoc ( prop.dx, prop.dy )
			coroutine.yield ()
		end
		
		setState ( prop, rocketHitState )
	end
	
	prop.action = MOAIThread.new ()
	prop.action:run ( prop.main, prop )
end

function onPointerEvent ( x, y )
	mouseX, mouseY = layer:wndToWorld ( x, y )
end

function onMouseLeftEvent ( down )

	if down and launchCounter > ANTI_ROCKET_LAUNCH_DELAY then
		setState ( MOAIProp2D.new (), antiRocketState, mouseX, mouseY )
		launchCounter = 0
	end
end

MOAIInputMgr.device.pointer:setCallback ( onPointerEvent )
MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )

function main ()

	while alive == true do
	
		local x = math.random ( -160, 160 )
		setState ( MOAIProp2D.new (), rocketState, x, ROCKET_SPAWN_HEIGHT )
		
		for i = 1, 90 do
			launchCounter = launchCounter + 1
			coroutine.yield ()
		end
	end
	
	while alive == false do
		textbox:setString ( "You are dead!" )
		textbox:spool ()
		alive = nil
	end	
	
end

spawnThread = MOAIThread.new ()
spawnThread:run ( main )




