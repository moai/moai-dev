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
-- gfxQuad:setTexture ( "cathead.jpg" ) -- crashes

 
local texture = MOAITexture.new () 
texture:load ( "moai.png" )
-- texture:setFilter ( MOAITexture.GL_LINEAR ) 
-- local w, h = texture:getSize () 
-- local gfxQuad = MOAIGfxQuad2D.new ()
-- gfxQuad:setTexture ( texture )
-- gfxQuad:setRect ( -(w/2), -(h/2), (w/2), (h/2) )

-- prop = MOAIProp2D.new ()
-- prop:setDeck ( gfxQuad )
-- layer:insertProp ( prop )

-- action = prop:moveRot ( 360, 1.5 )


-- local threadFunc = function ()
	
	-- repeat coroutine.yield () until not action:isBusy ()
	
	-- layer:removeProp ( prop )

	-- gfxQuad2 = MOAIGfxQuad2D.new ()
	
	-- texture:load ( "prof-pic.jpg" )
	-- texture:setFilter ( MOAITexture.GL_LINEAR ) 
	-- local w, h = texture:getSize () 
	-- local gfxQuad2 = MOAIGfxQuad2D.new ()
	-- gfxQuad2:setTexture ( texture )
	-- gfxQuad2:setRect ( -(w/2), -(h/2), (w/2), (h/2) )

	-- prop2 = MOAIProp2D.new ()
	-- prop2:setDeck ( gfxQuad2 )
	-- layer:insertProp ( prop2 )

	-- prop2:moveRot ( 360, 1.5 )
-- end

-- thread = MOAIThread.new ()
-- thread:run ( threadFunc )