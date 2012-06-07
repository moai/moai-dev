----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 4, 1, 0, 0 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 1, 1, 1 )

MOAISim.openWindow ( "test", 1024, 640 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 640 )
viewport:setScale ( 1024, 640 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

camera = MOAICamera.new ()
camera:setOrtho ( true )
camera:setNearPlane ( 10000 )
camera:setFarPlane ( -10000 )
camera:setRot ( 54.736, 0, 135 )
layer:setCamera ( camera )

texture = MOAITexture.new ()
texture:load ( 'moai.png' )

function makeBoundsProp ( layer, size )

	size = size * 0.5

	local boundsDeck = MOAIBoundsDeck.new ()
	boundsDeck:reserveBounds ( 1 )
	boundsDeck:reserveIndices ( 1 )
	boundsDeck:setBounds ( 1, -size, -size, -size, size, size, size )

	local gfxQuad
	gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( texture )
	gfxQuad:setBoundsDeck ( boundsDeck )
	gfxQuad:setRect ( -size, -size, size, size )
	gfxQuad:setUVRect ( 0, 0, 1, 1 )
	
	local prop = MOAIProp.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )
	
	return prop
end

prop = makeBoundsProp ( layer, 96 )
prop:setLoc ( 0, 0 )
prop:moveRot ( 0, 0, 360, 3 )
