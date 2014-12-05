----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 320 )
viewport:setScale ( 320, -320 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -160, -160, 160, 160 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

scissor1 = MOAIScissorRect.new ()
scissor1:setRect ( -160, -160, 160, 0 )
scissor1:moveLoc ( 0, -160, 0, 1.5 )

scissor2 = MOAIScissorRect.new ()
scissor2:setRect ( -64, -160, 64, 0 )
scissor2:setScissorRect ( scissor1 )
scissor2:moveLoc ( 0, 160, 0, 1.5 )
prop:setScissorRect ( scissor2 )

prop:moveRot ( 360, 1.5 )
