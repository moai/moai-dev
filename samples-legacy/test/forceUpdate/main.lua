----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 640, 640 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 640 )
viewport:setScale ( 640, 640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

transform = MOAITransform.new ()
transform:setLoc ( 200, 0 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setParent ( transform )
layer:insertProp ( prop )

-- force the dep graph to update so prop:inside () will take parent transform unto account
prop:forceUpdate ()

if prop:inside ( 200, 0 ) then
	print ( 'INSIDE!' )
end

prop:moveRot ( 360, 1.5 )
