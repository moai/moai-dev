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

texture = MOAITexture.new ()
texture:load ( "moai.png" )
texture:setWrap ( true )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setRect ( -64, -64, 64, 64 )

uvTransform = MOAITransform2D.new ()
uvTransform:moveLoc ( 1, 0, 3 )
uvTransform:moveRot ( 360, 3 )
uvTransform:moveScl ( 1, 1, 3 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setUVTransform ( uvTransform )
layer:insertProp ( prop )

prop:moveRot ( -360, 2 )
prop:moveScl ( 1.5, 1.5, 3 )

