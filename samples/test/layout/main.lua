print ( "hello, moai!" )

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )
--viewport:setOffset ( -1, 1 )

scene:setViewport ( viewport )
MOAISim.pushRenderPass ( scene )

-- set us up the layout

frame = MOAIFrame.new ()
frame:setLayout ( MOAIFrame.LAYOUT_JUSTIFIED, MOAIFrame.LAYOUT_CENTER )
frame:setFit ( MOAIFrame.FIT_CONTENT, MOAIFrame.FIT_CONTENT )

child = MOAIFrame.new ()
child:setIdealSize ( 32, 48 )
child:setParent ( frame )

child = MOAIFrame.new ()
child:setIdealSize ( 48, 64 )
child:setParent ( frame )

child = MOAIFrame.new ()
child:setIdealSize ( 64, 32 )
child:setParent ( frame )

----------------------------------------------------------------
-- set us up the sprite
texture = MOAITexture.new ()
texture:load ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )
texture:setUVRect ( 0, 1, 1, 0 )

sprite = MOAISprite2D.new ()
sprite:setGfxSource ( texture )
sprite:setParent ( frame )
--sprite:setFrame ( -64, 64, 128, 128 )
--sprite:setFrameSource ( MOAISprite2D.FRAME_FROM_DEFINE )

scene:insertPrim ( sprite )

----------------------------------------------------------------
-- set us up the ninepatch
--[[
ninePatch = MOAINinePatch.new ()
ninePatch:setTexture ( "cathead.png" )

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

sprite = MOAISprite2D.new ()
sprite:setGfxSource ( ninePatch )
sprite:setParent ( frame )
--sprite:setFrame ( -64, -64, 192, 64 )
--sprite:setFrameSource ( MOAISprite2D.FRAME_FROM_DEFINE )
scene:insertPrim ( sprite )
]]--

frame:layout ()

----------------------------------------------------------------
sprite:moveRot ( -360, 3 )
sprite:moveScl ( 0, 1, 3 )

--frame:moveLoc ( 64, 64, 3 )
--frame:moveRot ( 360, 3 )

----------------------------------------------------------------
MOAISim.openWindow ( "layout", 320, 480 )