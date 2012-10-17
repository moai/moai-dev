----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 256 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 256 )
viewport:setScale ( 512, 256 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

stretchPatch = MOAIStretchPatch2D.new ()
stretchPatch:setTexture ( "moai.png" )

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

prop:setLoc ( -128, 0 )
prop:moveScl ( 1, 1, 3 )

stretchPatch = MOAIStretchPatch2D.new ()
stretchPatch:setTexture ( "moai2.png" )

stretchPatch:reserveRows ( 3 )
stretchPatch:setRow ( 1, 0.25, false )
stretchPatch:setRow ( 2, 0.50, true )
stretchPatch:setRow ( 3, 0.25, false )

stretchPatch:reserveColumns ( 3 )
stretchPatch:setColumn ( 1, 0.25, false )
stretchPatch:setColumn ( 2, 0.50, true )
stretchPatch:setColumn ( 3, 0.25, false )

stretchPatch:reserveUVRects ( 1 )
stretchPatch:setUVRect ( 1, 0.25, .75, .75, 0.25 )

stretchPatch:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( stretchPatch )
layer:insertProp ( prop )

prop:setLoc ( 128, 0 )
prop:moveScl ( 1, 1, 3 )
