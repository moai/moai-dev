----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 256 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 256 )
viewport:setScale ( 512, -256 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

function addProp ( x, y, filter )

	local texture = MOAITexture.new ()
	texture:setFilter ( filter )
	texture:load ( "moai.png" )	

	local gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( texture )
	gfxQuad:setRect ( -128, -128, 128, 128 )
	gfxQuad:setUVRect ( 0, 0, 1, 1 )

	local prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	prop:setLoc ( x, y )
	layer:insertProp ( prop )
end

addProp ( -128, 0, MOAITexture.GL_NEAREST )
addProp ( 128, 0, MOAITexture.GL_LINEAR )