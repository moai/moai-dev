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
gfxQuad:setRect ( -128, -128, 128, 128 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

curve = MOAIAnimCurveVec.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0, 0, 0, 0 )
curve:setKey ( 2, 1, 100, 100, 0 )

prop:setAttrLink ( MOAIProp2D.ATTR_TRANSLATE, curve, MOAIAnimCurve.ATTR_VALUE )

timer = MOAITimer.new ()
timer:setSpan ( 0, curve:getLength ())

curve:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )

timer:start ()
