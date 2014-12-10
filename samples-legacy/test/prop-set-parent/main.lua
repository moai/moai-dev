----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )

color = MOAIColor.new ()
color:setColor ( 1, 0, 0, 1 )
color:seekColor ( 0, 1, 1, 1, 1.5 )

root = MOAIProp2D.new ()
root:moveRot ( 360, 1.5 )
root:moveLoc ( 0, 128, 1.5 )
root:moveScl ( 0, -2, 1.5 )
root:setColor ( 1, 0, 0, 1 )
root:seekColor ( 0, 1, 1, 1, 1.5 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setFrame ( -64, -64, 64, 64 )
prop:setParent ( root )
layer:insertProp ( prop )

