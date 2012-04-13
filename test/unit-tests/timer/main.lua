----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

workingDir = MOAIFileSystem.getWorkingDirectory ()
MOAIFileSystem.setWorkingDirectory ( ".." )
require "testhelpers"
MOAIFileSystem.setWorkingDirectory ( workingDir )
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

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad )
prop2:setLoc ( -350, 0 )
layer:insertProp ( prop2 )

prop3 = MOAIProp2D.new ()
prop3:setDeck ( gfxQuad )
prop3:setLoc ( -350, 250 )
layer:insertProp ( prop3 )

function callback ( timer, keyframe, executed, keytime, value )
	-- if keyframe == 1 then
		-- timer:setTime ( .5 )
	-- end
	if keyframe == 2 then
		timer:stop ()
		timer:start ()
	end
	print ( timer:getTime ())
	print ( "key frame: " .. keyframe, "executed: " .. executed, "key time: " .. keytime, "value: " .. value )
end

function callback2 ( timer, executed )
	print ( "callback" )
	print ( executed )
end

function timerTests ()
	timer = MOAITimer.new ()
	timer:setMode ( MOAITimer.LOOP )
	
	curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 2 )
	curve:setKey ( 1, 0, 0, MOAIEaseType.LINEAR, .95 )
	curve:setKey ( 2, 1, 30, MOAIEaseType.LINEAR, .95 )
	
	timer:setCurve ( curve )
	
	timer:setListener ( MOAITimer.EVENT_TIMER_KEYFRAME, callback )
	timer:setSpeed ( 2 )
	timer:setSpan ( 5 )
	timer:start ()
	
end

test = 3
wrapmode = MOAIAnimCurve.APPEND

function animCurveTests ()
	
	if test == 1 then
		curve = MOAIAnimCurve.new ()
		curve:reserveKeys ( 2 )
		curve:setKey ( 1, 0, -350, MOAIEaseType.SOFT_EASE_IN )
		curve:setKey ( 2, 1, -100 )
		curve:setWrapMode ( wrapmode )
		
		curve2 = MOAIAnimCurve.new ()
		curve2:reserveKeys ( 2 )
		curve2:setKey ( 1, 0, -350, MOAIEaseType.EASE_IN )
		curve2:setKey ( 2, 1, -100 )
		curve2:setWrapMode ( wrapmode )
		
		curve3 = MOAIAnimCurve.new ()
		curve3:reserveKeys ( 2 )
		curve3:setKey ( 1, 0, -350, MOAIEaseType.SHARP_EASE_IN )
		curve3:setKey ( 2, 1, -100 )
		curve3:setWrapMode ( wrapmode )
		
	elseif test == 2 then
		curve = MOAIAnimCurve.new ()
		curve:reserveKeys ( 2 )
		curve:setKey ( 1, 0, -350, MOAIEaseType.SOFT_EASE_OUT )
		curve:setKey ( 2, 1, -100 )
		curve:setWrapMode ( wrapmode )
		
		curve2 = MOAIAnimCurve.new ()
		curve2:reserveKeys ( 2 )
		curve2:setKey ( 1, 0, -350, MOAIEaseType.EASE_OUT )
		curve2:setKey ( 2, 1, -100 )
		curve2:setWrapMode ( wrapmode )
		
		curve3 = MOAIAnimCurve.new ()
		curve3:reserveKeys ( 2 )
		curve3:setKey ( 1, 0, -350, MOAIEaseType.SHARP_EASE_OUT )
		curve3:setKey ( 2, 1, -100 )
		curve3:setWrapMode ( wrapmode )
		
	elseif test == 3 then
		curve = MOAIAnimCurve.new ()
		curve:reserveKeys ( 2 )
		curve:setKey ( 1, 0, -350, MOAIEaseType.SOFT_SMOOTH )
		curve:setKey ( 2, 1, -100 )
		curve:setWrapMode ( wrapmode )
		
		curve2 = MOAIAnimCurve.new ()
		curve2:reserveKeys ( 2 )
		curve2:setKey ( 1, 0, -350, MOAIEaseType.SMOOTH )
		curve2:setKey ( 2, 1, -100 )
		curve2:setWrapMode ( wrapmode )
		
		curve3 = MOAIAnimCurve.new ()
		curve3:reserveKeys ( 2 )
		curve3:setKey ( 1, 0, -350, MOAIEaseType.SHARP_SMOOTH )
		curve3:setKey ( 2, 1, -100 )
		curve3:setWrapMode ( wrapmode )
		
	elseif test == 4 then
		curve = MOAIAnimCurve.new ()
		curve:reserveKeys ( 2 )
		curve:setKey ( 1, 0, -350, MOAIEaseType.FLAT )
		curve:setKey ( 2, 1, -100 )
		curve:setWrapMode ( wrapmode )
		
		curve2 = MOAIAnimCurve.new ()
		curve2:reserveKeys ( 2 )
		curve2:setKey ( 1, 0, -350, MOAIEaseType.LINEAR )
		curve2:setKey ( 2, 1, -100 )
		curve2:setWrapMode ( wrapmode )
		
		curve3 = MOAIAnimCurve.new ()
		curve3:reserveKeys ( 2 )
		curve3:setKey ( 1, 0, -350, MOAIEaseType.FLAT )
		curve3:setKey ( 2, 1, -100 )
		curve3:setWrapMode ( wrapmode )
	end

	prop:setAttrLink ( MOAIProp2D.ATTR_X_LOC, curve, MOAIAnimCurve.ATTR_VALUE )
	prop2:setAttrLink ( MOAIProp2D.ATTR_X_LOC, curve2, MOAIAnimCurve.ATTR_VALUE )
	prop3:setAttrLink ( MOAIProp2D.ATTR_X_LOC, curve3, MOAIAnimCurve.ATTR_VALUE )
	-- prop:setAttrLink ( MOAIProp2D.ATTR_Y_LOC, curve2, MOAIAnimCurve.ATTR_VALUE )
	
	curve1 = MOAIAnimCurve.new ()
	curve1:reserveKeys ( 2 )
	curve1:setKey ( 1, 0, 0, MOAIEaseType.LINEAR, .95 )
	curve1:setKey ( 2, 3, 0, MOAIEaseType.LINEAR, .95 )

	timer = MOAITimer.new ()
	timer:setSpan ( 0, curve1:getLength ())
	timer:setMode ( MOAITimer.LOOP )
	timer:setSpeed ( .2 )
	timer:setCurve ( curve1 )
	timer:setListener ( MOAITimer.EVENT_TIMER_KEYFRAME, callback )

	curve:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )
	curve2:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )
	curve3:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )

	timer:start ()
	
end

animCurveTests ()

