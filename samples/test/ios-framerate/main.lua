----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewWidth, viewHeight = MOAIGfxDevice.getViewSize ()

viewport = MOAIViewport.new ()
viewport:setSize ( viewWidth, viewHeight )
viewport:setScale ( viewWidth, viewHeight )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:setLoc ( 100, 100 )
prop:setRot ( 135 )
prop:setScl ( 2, 2 )

function onResize ( width, height )
	viewport:setSize ( width, height )
	viewport:setScale ( width, height )
end

MOAIGfxDevice.setListener ( MOAIGfxDevice.EVENT_RESIZE, onResize )

timer = MOAITimer.new ()
timer:setMode ( MOAITimer.LOOP )
timer:setSpan ( .5 )
timer:setListener ( MOAITimer.EVENT_TIMER_LOOP, 
	function ()
		print ( MOAISim.getPerformance ())
	end )
timer:start ()
