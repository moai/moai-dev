----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )
--viewport:setOffset ( -1, 1 )

scene:setViewport ( viewport )
MOAISim.pushRenderPass ( scene )

-- set us up the layout

frame = MOAILayoutFrame.new ()
frame:setLayout ( MOAILayoutFrame.LAYOUT_JUSTIFIED, MOAILayoutFrame.LAYOUT_CENTER )
frame:setFit ( MOAILayoutFrame.FIT_CONTENT, MOAILayoutFrame.FIT_CONTENT )

child = MOAILayoutFrame.new ()
child:setIdealSize ( 32, 48 )
child:setParent ( frame )

child = MOAILayoutFrame.new ()
child:setIdealSize ( 48, 64 )
child:setParent ( frame )

child = MOAILayoutFrame.new ()
child:setIdealSize ( 64, 32 )
child:setParent ( frame )

----------------------------------------------------------------
-- set us up the sprite
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "moai.png" )
texture:setRect ( -64, -64, 64, 64 )
texture:setUVRect ( 0, 1, 1, 0 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( texture )
sprite:setParent ( frame )
--sprite:setFrame ( -64, 64, 128, 128 )
--sprite:setFrameSource ( MOAIProp2D.FRAME_FROM_DEFINE )

scene:insertProp ( sprite )

----------------------------------------------------------------
-- set us up the ninepatch
--[[
ninePatch = MOAIStretchPatch.new ()
ninePatch:setTexture ( "moai.png" )

ninePatch:reserveRows ( 3 )
ninePatch:setRow ( 0, 0.25, false )
ninePatch:setRow ( 1, 0.50, true )
ninePatch:setRow ( 2, 0.25, false )

ninePatch:reserveColumns ( 3 )
ninePatch:setColumn ( 0, 0.25, false )
ninePatch:setColumn ( 1, 0.50, true )
ninePatch:setColumn ( 2, 0.25, false )

ninePatch:reserveUVRects ( 1 )
ninePatch:setUVRect ( 0, 0, 0, 1, 1 )

ninePatch:setRect ( -64, -64, 64, 64 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( ninePatch )
sprite:setParent ( frame )
--sprite:setFrame ( -64, -64, 192, 64 )
--sprite:setFrameSource ( MOAIProp2D.FRAME_FROM_DEFINE )
scene:insertProp ( sprite )
]]--

frame:layout ()

----------------------------------------------------------------
sprite:moveRot ( -360, 3 )
sprite:moveScl ( 0, 1, 3 )

--frame:moveLoc ( 64, 64, 3 )
--frame:moveRot ( 360, 3 )

----------------------------------------------------------------
MOAISim.openWindow ( "layout", 320, 480 )