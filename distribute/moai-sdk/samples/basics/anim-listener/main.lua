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

tileLib = MOAITileDeck2D:new ()
tileLib:setTexture ( "numbers.png" )
tileLib:setSize ( 8, 8 )
tileLib:setRect ( -16, -16, 16, 16 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileLib )
layer:insertProp ( prop )

curve = MOAIAnimCurve.new ()

curve:reserveKeys ( 5 )
curve:setKey ( 1, 0.00, 1, MOAIEaseType.FLAT )
curve:setKey ( 2, 0.25, 2, MOAIEaseType.FLAT )
curve:setKey ( 3, 0.50, 3, MOAIEaseType.FLAT )
curve:setKey ( 4, 0.75, 4, MOAIEaseType.FLAT )
curve:setKey ( 5, 1.00, 5, MOAIEaseType.FLAT )

anim = MOAIAnim:new ()
anim:reserveLinks ( 1 )
anim:setLink ( 1, curve, prop, MOAIProp2D.ATTR_INDEX )
--anim:setMode ( MOAITimer.LOOP )
anim:start ()

local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

function onKeyframe ( self, i, t, v )
	printf ( "keyframe %d: %f, %f\n", i, t, v )
end

function onLoop ( self, i )
	printf ( "LOOP %d\n", i )
end

function onStop ( self )
	printf ( "STOP\n" )
end

anim:setCurve ( curve )
anim:setListener ( MOAITimer.EVENT_TIMER_KEYFRAME, onKeyframe )
anim:setListener ( MOAITimer.EVENT_TIMER_LOOP, onLoop )
anim:setListener ( MOAIAction.EVENT_STOP, onStop )

