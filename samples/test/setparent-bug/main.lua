----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------
MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop1 = MOAIProp2D.new ()
prop1:setDeck ( gfxQuad )
prop1:setLoc ( 0, 64 )
layer:insertProp ( prop1 )

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad )
prop2:setLoc ( 0, -64 )
layer:insertProp ( prop2 )

prop2:setParent ( prop1 )

function threadFunc ()

   local action

   action = prop1:moveLoc ( -64, 128, 1 )
       --prop1:setShearByZ ( 1, -1 )
       prop1:moveRot ( 75, 1 )
       prop1:moveScl ( 0.5, 1, 1 )
   MOAIThread.blockOnAction ( action )

       --prop2:detach()
       
   action = prop1:moveLoc ( 256, -64, 1 ) 
       prop1:moveRot ( -75, 1 )
       prop1:moveScl ( -0.5, -1, 1 )
   MOAIThread.blockOnAction ( action )
       
end

thread = MOAICoroutine.new ()
--thread:run ( threadFunc )