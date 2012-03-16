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
gfxQuad:setTexture ( "cathead.png" )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setFrame ( -64, -64, 64, 64 )
layer:insertProp ( prop )

root = MOAIProp2D.new ()
root:setDeck ( gfxQuad )
root:setFrame ( -64, -64, 64, 64 )
layer:insertProp ( root )

prop:setAttrLink ( MOAITransform.ATTR_X_SCL, root, MOAITransform.ATTR_WORLD_X_SCL )
