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
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setLoc ( -350, -250 )
layer:insertProp ( prop )

function keyframeCallback ( timer, keyframe, executed, keytime, value )
	if keyframe == 2 then
		timer:stop ()
		timer:start ()
	end
	print ( timer:getTime ())
	print ( "key frame: " .. keyframe, "executed: " .. executed, "key time: " .. keytime, "value: " .. value )
end

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 3 )
curve:setKey ( 1, 0, -350, MOAIEaseType.SOFT_EASE_IN )
curve:setKey ( 2, 1, 0, MOAIEaseType.SOFT_EASE_IN )
curve:setKey ( 3, 2, 350 )

prop:setAttrLink ( MOAIProp2D.ATTR_X_LOC, curve, MOAIAnimCurve.ATTR_VALUE )

curve1 = MOAIAnimCurve.new ()
curve1:reserveKeys ( 3 )
curve1:setKey ( 1, 0, 0, MOAIEaseType.LINEAR, .95 )
curve1:setKey ( 2, 1, 0, MOAIEaseType.LINEAR, .95 )
curve1:setKey ( 3, 2, 0, MOAIEaseType.LINEAR, .95 )

timer = MOAITimer.new ()
timer:setSpan ( 0, curve:getLength ())
timer:setMode ( MOAITimer.LOOP )
timer:setSpeed ( .5 )
timer:setCurve ( curve1 )
timer:setListener ( MOAITimer.EVENT_TIMER_KEYFRAME, keyframeCallback )

curve:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )

timer:start ()
