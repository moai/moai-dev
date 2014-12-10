----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:setRot ( 45 )
prop:setLoc ( 50, 100 )
prop:setScl ( 2, 2 )

print ( prop:getWorldDir ())
print ( prop:getWorldLoc ())
print ( prop:getWorldRot ())
print ( prop:getWorldScl ())

function onEnd ()
	print ( prop:getWorldDir ())
	print ( prop:getWorldLoc ())
	print ( prop:getWorldRot ())
	print ( prop:getWorldScl ())
end

timer = MOAITimer.new ()
timer:setSpan ( 1 )
timer:setListener ( MOAITimer.EVENT_TIMER_END_SPAN, onEnd )
timer:start ()