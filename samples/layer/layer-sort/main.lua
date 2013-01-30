----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

function addProp ( x, y )
	local prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	prop:setLoc ( x, y )
	layer:insertProp ( prop )
end

addProp ( -32, 64 )
addProp ( -16, 32 )
addProp ( 0, 0 )
addProp ( 16, -32 )
addProp ( 32, -64 )

-- sort using prop location in camera space
layer:setSortMode ( MOAILayer2D.SORT_Y_DESCENDING )

