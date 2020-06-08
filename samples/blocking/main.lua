----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( "../resources/moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )

function threadFunc ()
 
    local action
 
    action = prop:moveRot ( 0, 0, 180, 3 )
    MOAICoroutine.blockOnAction ( action )
 
    action = prop:moveLoc ( 64, 0, 0, 2 ) 
    MOAICoroutine.blockOnAction ( action )
 
    action = prop:moveScl ( -0.5, -0.5, 0, 1 ) 
    MOAICoroutine.blockOnAction ( action )

	action = prop:moveRot ( 0, 0, -180, 3 )
    MOAICoroutine.blockOnAction ( action )
	
end

thread = MOAICoroutine.new ()
thread:run ( threadFunc )
