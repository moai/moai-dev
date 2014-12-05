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

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0.0, 0 )
curve:setKey ( 2, 1.5, 360 )
curve:setWrapMode ( MOAIAnimCurve.CLAMP )

curve2 = MOAIAnimCurve.new ()
curve2:reserveKeys ( 2 )
curve2:setKey ( 1, 0.0, -64 )
curve2:setKey ( 2, 1.5, 64 )
curve2:setWrapMode ( MOAIAnimCurve.MIRROR )

curve3 = MOAIAnimCurve.new ()
curve3:reserveKeys ( 2 )
curve3:setKey ( 1, 0.0, -64 )
curve3:setKey ( 2, 1.5, -32 )
curve3:setWrapMode ( MOAIAnimCurve.APPEND )

prop:setAttrLink ( MOAIProp2D.ATTR_Z_ROT, curve, MOAIAnimCurve.ATTR_VALUE )
prop:setAttrLink ( MOAIProp2D.ATTR_X_LOC, curve2, MOAIAnimCurve.ATTR_VALUE )
prop:setAttrLink ( MOAIProp2D.ATTR_Y_LOC, curve3, MOAIAnimCurve.ATTR_VALUE )

timer = MOAITimer.new ()
timer:setSpan ( 0, 3.0 )

timer2 = MOAITimer.new ()
timer2:setSpan ( 0, 2.25 )

curve:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )
curve2:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer2, MOAITimer.ATTR_TIME )
curve3:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )

timer:start ()
timer2:start ()
