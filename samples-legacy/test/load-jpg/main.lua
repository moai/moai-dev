----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.jpg" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

action = prop:moveRot ( 360, 1.5 )


local threadFunc = function ()
	
	repeat coroutine.yield () until not action:isBusy ()
	
	layer:removeProp ( prop )

	gfxQuad2 = MOAIGfxQuad2D.new ()
	gfxQuad2:setTexture ( "prof-pic.jpg" )
	gfxQuad2:setRect ( -25, -25, 25, 25 )
	gfxQuad2:setUVRect ( 0, 0, 1, 1 )

	prop2 = MOAIProp2D.new ()
	prop2:setDeck ( gfxQuad2 )
	layer:insertProp ( prop2 )

	prop2:moveRot ( 360, 1.5 )
end

thread = MOAIThread.new ()
thread:run ( threadFunc )