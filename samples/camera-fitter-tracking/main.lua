----------------------------------------------------------------
-- Demonstrate tracking a prop with the camera fitter.
-- With MOAICameraFitter2D.FITTING_MODE_APPLY_BOUNDS mode set,
-- it is visible in the end how the fitter stops at the boundaries
-- while the prop keeps moving off the screen center.
----------------------------------------------------------------

function drawBackground ( draw )
    draw:bindVectorPresets ()
	draw:setPenColor ( 1, 1, 1 )
	draw:drawLine ( -1000, -1000, 1000, 1000 )
	draw:drawLine ( 1000, -1000, -1000, 1000 )
end

MOAIDebugLinesMgr.setStyle ( MOAIGraphicsProp.DEBUG_DRAW_WORLD_BOUNDS, 2, 0.75, 0.75, 0.75 )

MOAISim.openWindow ( "test", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

camera = MOAICamera.new ()

background = MOAITableViewLayer.new ()
background:setViewport ( viewport )
background:setCamera ( camera )
background:setRenderTable ({ drawBackground })
background:pushRenderPass ()

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
layer:pushRenderPass ()
layer:showDebugLines ( true )

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( '../resources/moai.png' )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop0 = MOAIProp.new ()
prop0:setDeck ( gfxQuad )
prop0:moveLoc ( 256, 0, 0, 3 )
prop0:setPartition ( layer )

prop1 = MOAIProp.new ()
prop1:setDeck ( gfxQuad )
prop1:moveLoc ( -256, 0, 0, 3 )
prop1:setPartition ( layer )

fitter = MOAICameraFitter2D.new ()
fitter:setViewport ( viewport )
fitter:setCamera ( camera )
fitter:setBounds ( -500, -128, 500, 1000 )
fitter:setMin ( 256 )
fitter:start ()

fitter:setFitMode ( MOAICameraFitter2D.FITTING_MODE_APPLY_BOUNDS )
fitter:startTrackingNode ( prop0 )
