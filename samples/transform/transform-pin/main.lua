----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

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
gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
worldLayer:insertProp ( prop )

prop:moveRot ( 360, 3 )

--set up the hud layer
hudLayer = MOAILayer2D.new ()
hudLayer:setViewport ( viewport )
MOAISim.pushRenderPass ( hudLayer )

-- create the bridge
pinTransform = MOAIPinTransform.new ()
pinTransform:init ( prop, worldLayer, hudLayer )

-- create the pin hraphic and attach it to the pin transform
gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -32, -32, 32, 32 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

pin = MOAIProp2D.new ()
pin:setDeck ( gfxQuad )
pin:setAttrLink ( MOAIProp2D.INHERIT_LOC, pinTransform, MOAIProp2D.TRANSFORM_TRAIT )
hudLayer:insertProp ( pin )
