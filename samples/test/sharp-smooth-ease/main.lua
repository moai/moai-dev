----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 800, 800 )

viewport = MOAIViewport.new ()
viewport:setSize ( 800, 800 )
viewport:setScale ( 800, 800 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop1 = MOAIProp2D.new ()
prop1:setDeck ( gfxQuad )
prop1:setLoc ( -350, 250 )
layer:insertProp ( prop1 )

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad )
prop2:setLoc ( -350, -250 )
layer:insertProp ( prop2 )

-- the broken curve
curve1 = MOAIAnimCurve.new ()
curve1:reserveKeys ( 2 )
curve1:setKey ( 1, 0, -350, MOAIEaseType.SHARP_SMOOTH )
curve1:setKey ( 2, 1, 350 )

curve2 = MOAIAnimCurve.new ()
curve2:reserveKeys ( 2 )
curve2:setKey ( 1, 0, -350, MOAIEaseType.SOFT_SMOOTH )
curve2:setKey ( 2, 1, 350 )

prop1:setAttrLink ( MOAIProp2D.ATTR_X_LOC, curve1, MOAIAnimCurve.ATTR_VALUE )
prop2:setAttrLink ( MOAIProp2D.ATTR_X_LOC, curve2, MOAIAnimCurve.ATTR_VALUE )

curvet = MOAIAnimCurve.new ()
curvet:reserveKeys ( 2 )
curvet:setKey ( 1, 0, 0, MOAIEaseType.LINEAR, .95 )
curvet:setKey ( 2, 1, 0, MOAIEaseType.LINEAR, .95 )

timer = MOAITimer.new ()
timer:setSpan ( 0, curvet:getLength ())
timer:setMode ( MOAITimer.LOOP )
timer:setSpeed ( .2 )
timer:setCurve ( curvet )

curve1:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )
curve2:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )

timer:start ()

