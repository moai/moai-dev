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
dofile ( '../lib/walls.lua' )

--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 1, 1, 1 )

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

props = {}

props [ 1 ] = makeMeshProp ( makeBox ( 45.25, 45.25, 90.5, 'moai.png' ))
props [ 1 ]:setLoc ( tileToWorld ( 1, 1, 0 ))

table.insert ( props, makeWallProp ( 1, 0, -4 ))
table.insert ( props, makeWallProp ( 2, 0, -2 ))
table.insert ( props, makeWallProp ( 3, 0, 0 ))
table.insert ( props, makeWallProp ( 4, -2, 0 ))
table.insert ( props, makeWallProp ( 4, -4, 0 ))
table.insert ( props, makeWallProp ( 2, -6, 2 ))
table.insert ( props, makeWallProp ( 2, -6, 4 ))
table.insert ( props, makeWallProp ( 7, -6, 6 ))

table.insert ( props, makeWallProp ( 8, 4, -2 ))
table.insert ( props, makeWallProp ( 8, 8, -2 ))
table.insert ( props, makeWallProp ( 8, 12, -2 ))

for i, prop in ipairs ( props ) do
	layer:insertProp ( prop )
end

runHero ( props )
runCamera ( camera, props [ 1 ])