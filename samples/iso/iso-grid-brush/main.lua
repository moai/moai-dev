----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--[[
	This demo shows a combination of 2d billboards and 3d geometry using ISO_SORT.
	The walls are drawn using props in billboard mode attached to grid brushes. Because
	the grid brush is a 2d primitive we use MOAIBoundsDeck to override the grid
	brush bounding box with a 3d bounding box suitable for ISO_SORT. The billboard
	bounding box is drawn in white using MOAIDebugLines.
	
	Move the avatar box around with the AWSD keys. It should sort correctly against
	the billboard props. Use EQ to raise/lower the box and X to reset its position.
	SPACE will cycle through all props and G will reset all props. P will print the
	location of each prop.
]]--

dofile ( '../lib/camera.lua' )
dofile ( '../lib/hero.lua' )
dofile ( '../lib/mesh.lua' )

MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 1, 1, 1 )

MOAISim.openWindow ( "test", 724, 512 )

layer = MOAILayer2D.new ()
layer:setSortMode ( MOAILayer.SORT_ISO )
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 724, 512 )
viewport:setScale ( 724, 512 )
layer:setViewport ( viewport )

-- this is technically a 'dimetric' projection, not a true isometric projection
camera = makeDimetricCamera ()
layer:setCamera ( camera )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "iso-grid.png" )
tileDeck:setSize ( 8, 8 )
tileDeck:setRect ( -0.5, 0.5, 0.5, -0.5 )

grid = MOAIGrid.new ()
grid:setSize ( 8, 9, 32, 32 )

grid:setRow ( 1, 	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00 )
grid:setRow ( 2, 	0x01, 0x02, 0x1a, 0x1a, 0x1a, 0x1a, 0x03, 0x04 )
grid:setRow ( 3, 	0x09, 0x0a, 0x1a, 0x1a, 0x1a, 0x1a, 0x0b, 0x0c )
grid:setRow ( 4, 	0x19, 0x19, 0x09, 0x0a, 0x0b, 0x0c, 0x1b, 0x1b )
grid:setRow ( 5, 	0x19, 0x19, 0x19, 0x19, 0x1b, 0x1b, 0x1b, 0x1b )
grid:setRow ( 6, 	0x19, 0x19, 0x19, 0x19, 0x1b, 0x1b, 0x1b, 0x1b )
grid:setRow ( 7, 	0x19, 0x19, 0x19, 0x19, 0x1b, 0x1b, 0x1b, 0x1b )
grid:setRow ( 8, 	0x11, 0x12, 0x19, 0x19, 0x1b, 0x1b, 0x13, 0x14 )
grid:setRow ( 9, 	0x00, 0x00, 0x11, 0x12, 0x13, 0x14, 0x00, 0x00 )

-- set up the grid deck
gridDeck = MOAIGridDeck2D.new ()
gridDeck:reserveBrushes ( 1 )
gridDeck:setBrush ( 1, 1, 1, 8, 9, -128, -224 )
gridDeck:setGrid ( grid )
gridDeck:setDeck ( tileDeck )

-- set the bounds override
boundsDeck = MOAIBoundsDeck.new ()
boundsDeck:reserveBounds ( 1 )
boundsDeck:reserveIndices ( 1 )
boundsDeck:setBounds ( 1, -90.5, -90.5, 0, 90.5, 90.5, 181 )
gridDeck:setBoundsDeck ( boundsDeck )

-- helper for making brush props
function makeBrushProp ( x, y )	
	local prop = MOAIProp.new ()
	prop:setDeck ( gridDeck )
	prop:setScl ( 1, -1 )
	prop:setLoc ( x, y )
	prop:setBillboard ( true ) -- billboard so it draws without distortion
	return prop
end

props = {}

props [ 1 ] = makeMeshProp ( makeBox ( 45.25, 45.25, 90.5, 'moai.png' ))
props [ 1 ]:setLoc ( 135.75, 135.75 )

props [ 2 ] = makeBrushProp ( 0, 0 )
props [ 3 ] = makeBrushProp ( 271.5, 0 )
props [ 4 ] = makeBrushProp ( 0, 271.5 )

-- TODO: mystery bug if using 'prop' instead of 'p'
for i, p in ipairs ( props ) do
	layer:insertProp ( p )
end

runHero ( props )
runCamera ( camera, props [ 1 ])