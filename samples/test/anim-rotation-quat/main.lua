----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

dofile ( "cube.lua" )

MOAISim.openWindow ( "test", 320, 480 )
MOAIGfxDevice.setClearDepth ( true )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:setSortMode ( MOAILayer.SORT_NONE ) -- don't need layer sort
MOAISim.pushRenderPass ( layer )

prop = makeBox ( 32, 16, 64 )
layer:insertProp ( prop )

curve = MOAIAnimCurveQuat.new ()
curve:reserveKeys ( 3 )
curve:setKey ( 1, 0, 0, 0, 0 )
curve:setKey ( 2, 1.0, 0, 90, 0 )
curve:setKey ( 3, 2.0, 90, 90, 0 )
curve:setKey ( 4, 3.0, 0, 0, 0 )
curve:setKey ( 5, 4.0, 45, 45, 45 )
curve:setKey ( 6, 5.0, 32, 45, 121 )
curve:setKey ( 7, 6.0, 0, 0, 0 )
curve:setWrapMode ( MOAIAnimCurve.APPEND )

timer = MOAITimer.new ()
timer:setSpan ( 0, curve:getLength ())
curve:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )
timer:start ()
prop:setAttrLink ( MOAIProp2D.ATTR_ROTATE_QUAT, curve, MOAIAnimCurve.ATTR_VALUE )

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )
