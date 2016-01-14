----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

path = MOAIPath.new ();

path:reserve ( 7 )

path:setPoint ( 1, -100, 0 )
path:setPoint ( 2, 250, 300 )
path:setPoint ( 3, -250, 300 )
path:setPoint ( 4, 100, 0 )

path:setPoint ( 5, 350, -300 )
path:setPoint ( 6, -350, -300 )
path:setPoint ( 7, -100, 0 )

path:bless ()

stepper = MOAIPathStepper.new ()
stepper:start ( path, path:getLength () / 128 )

local points = {}

while stepper:more () do
	local x, y = stepper:next ()
	table.insert ( points, { x = x, y = y })
end

function onDraw ()

	MOAIGfxDevice.setPenColor ( 1, 0, 1, 1 )

	local v1 = points [ 1 ]
	for i = 2, #points do
		local v2 = points [ i ]
		MOAIDraw.drawLine ( v1.x, v1.y, v2.x, v2.y )
		v1 = v2
	end

	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )

	for i, v in ipairs ( points ) do
		MOAIDraw.drawCircle ( v.x, v.y, 2, 8 )
	end

	--[[
	local r = 64

	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	MOAIDraw.fillCircle ( 0, 0, r, 32 )

	MOAIGfxDevice.setPenColor ( 1, 1, 1, 1 )
	MOAIDraw.drawCircleSpokes ( 0, 0, r, 8 )
	MOAIDraw.drawCircle ( 0, 0, r, 32 )
	]]--
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -64, -64, 64, 64 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )

