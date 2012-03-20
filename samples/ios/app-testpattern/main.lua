----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, iPhone!" )

----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 640, 960 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "testimage.png" )
gfxQuad:setRect ( -512, -512, 512, 512 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "testpattern.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )
