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

stretchPatch = MOAIStretchPatch2D.new ()
stretchPatch:setTexture ( "cathead.png" )

stretchPatch:reserveRows ( 3 )
stretchPatch:setRow ( 1, 0.25, false )
stretchPatch:setRow ( 2, 0.50, true )
stretchPatch:setRow ( 3, 0.25, false )

stretchPatch:reserveColumns ( 3 )
stretchPatch:setColumn ( 1, 0.25, false )
stretchPatch:setColumn ( 2, 0.50, true )
stretchPatch:setColumn ( 3, 0.25, false )

stretchPatch:reserveUVRects ( 1 )
stretchPatch:setUVRect ( 1, 0, 1, 1, 0 )

stretchPatch:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( stretchPatch )
layer:insertProp ( prop )

prop:moveScl ( 1, 1, 3 )

