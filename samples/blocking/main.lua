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

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

function threadFunc ()
 
    local action
 
    action = prop:moveRot ( 180, 3 )
    MOAIThread.blockOnAction ( action )
 
    action = prop:moveLoc ( 64, 0, 2 ) 
    MOAIThread.blockOnAction ( action )
 
    action = prop:moveScl ( -0.5, -0.5, 1 ) 
    MOAIThread.blockOnAction ( action )

	action = prop:moveRot ( -180, 3 )
    MOAIThread.blockOnAction ( action )
	
end

thread = MOAIThread.new ()
thread:run ( threadFunc )

