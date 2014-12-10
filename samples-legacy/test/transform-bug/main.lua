----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 1280, 960 )
viewport:setScale ( 1280, 960 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "test", 1280, 960 )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop1 = MOAIProp2D.new ()
prop1:setDeck ( gfxQuad )
prop1:setColor ( 0, 0, .5, .5 )
prop1:setLoc ( 0, 0 )

transform = MOAITransform.new ()
transform:setParent ( prop1 )

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad )
prop2:setColor ( .5, .5, 0, .5 )
prop2:setLoc ( 0, 0 )
prop2:setParent ( transform )

layer:insertProp ( prop1 )
layer:insertProp ( prop2 )

prop1:moveLoc ( 100, 100, 1 )