----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

uvTransform = MOAITransform.new ()
uvTransform:moveLoc ( 1, 0, 3 )
uvTransform:moveRot ( 360, 3 )
uvTransform:moveScl ( 1, 1, 3 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setUVTransform ( uvTransform )
layer:insertProp ( prop )

prop:moveRot ( -360, 2 )
prop:moveScl ( 1.5, 1.5, 3 )

MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "test", 320, 480 )
