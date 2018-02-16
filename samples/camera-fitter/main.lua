----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

camera = MOAICamera2D.new ()

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
layer:pushRenderPass ()

fitter = MOAICameraFitter2D.new ()
fitter:setViewport ( viewport )
fitter:setCamera ( camera )
fitter:setBounds ( -1000, -64, 1000, 10000 )
fitter:setMin ( 256 )
fitter:start ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:moveLoc ( 256, 0, 3 )
prop:setPartition ( layer )

anchor = MOAICameraAnchor2D.new ()
anchor:setParent ( prop )
anchor:setRect ( -64, -64, 64, 64 )
fitter:insertAnchor ( anchor )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:moveLoc ( -256, 0, 3 )
prop:setPartition ( layer )

anchor = MOAICameraAnchor2D.new ()
anchor:setParent ( prop )
anchor:setRect ( -64, -64, 64, 64 )
fitter:insertAnchor ( anchor )
