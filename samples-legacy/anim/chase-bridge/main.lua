----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function addProp ( layer, x0, y0, x1, y1 )

	local gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( "moai.png" )
	gfxQuad:setRect ( x0, y0, x1, y1 )
	gfxQuad:setUVRect ( 0, 0, 1, 1 )

	local prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )

	return prop
end

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

--set up the world layer
worldLayer = MOAILayer2D.new ()
worldLayer:setViewport ( viewport )
MOAISim.pushRenderPass ( worldLayer )

-- set up the world camera
camera = MOAICamera2D.new ()
camera:moveLoc ( 0, -192, 3 )
camera:moveScl ( 1, 1, 3 )
worldLayer:setCamera ( camera )

-- add a prop to the world
prop = addProp ( worldLayer, -64, -64, 64, 64 )
prop:moveRot ( 360, 3 )

--set up the hud layer
hudLayer = MOAILayer2D.new ()
hudLayer:setViewport ( viewport )
MOAISim.pushRenderPass ( hudLayer )

-- create the bridge
bridge = MOAILayerBridge2D.new ()
bridge:init ( prop, worldLayer, hudLayer )

-- create the pin and attach it to the bridge
pin = addProp ( hudLayer, -16, -16, 16, 16 )
pin:setAttrLink ( MOAIProp2D.INHERIT_LOC, bridge, MOAIProp2D.TRANSFORM_TRAIT )
pin:moveRot ( -360, 3 )

-- chase the pin
chase = addProp ( hudLayer, -32, -32, 32, 32 )
chase:setLoc ( 0, -128 )

driver = MOAIEaseDriver.new ()
driver:reserveLinks ( 2 )
driver:setLink ( 1, chase, MOAITransform.ATTR_X_LOC, pin, MOAITransform.ATTR_WORLD_X_LOC, MOAIEaseType.EASE_OUT )
driver:setLink ( 2, chase, MOAITransform.ATTR_Y_LOC, pin, MOAITransform.ATTR_WORLD_Y_LOC, MOAIEaseType.EASE_OUT )
driver:setSpan ( 3 )
driver:start ()
