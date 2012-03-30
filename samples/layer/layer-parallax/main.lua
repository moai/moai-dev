----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 480, 320 )

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 320 )
viewport:setScale ( 480, 320 )

camera = MOAICamera2D.new ()
camera:moveLoc ( 240, 0, 4 )

layer1 = MOAILayer2D.new ()
layer1:setViewport ( viewport )
layer1:setCamera ( camera )
layer1:setParallax ( 0.5, 1 )
MOAISim.pushRenderPass ( layer1 )

layer2 = MOAILayer2D.new ()
layer2:setViewport ( viewport )
layer2:setCamera ( camera )
MOAISim.pushRenderPass ( layer2 )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setScl ( 0.75, 0.75 )
layer1:insertProp ( prop )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer2:insertProp ( prop )
