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
gfxQuad:setRect ( -64, -64, 64, 64 )

matrix = MOAIMatrix.new ()
matrix:setMatrix (
	0.5, 0, 0, 64,
	0, 2, 0, 0,
	0, 0, 1, 0
)

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setLoc ( 0, 0 )
prop:setParent ( matrix )
layer:insertProp ( prop )
