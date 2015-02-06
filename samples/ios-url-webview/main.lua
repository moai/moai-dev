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
--layer:insertProp ( prop )

local action = prop:moveRot ( 540, 1.0 )

-- 320, 568

onTouch = function ( eventType )

	if eventType == MOAITouchSensor.TOUCH_DOWN then
		
		webView = MOAIWebViewIOS.new ()
		webView:show ( true )
		webView:loadRequest ( 'https://news.google.com' )
		webView = nil
	end
end

MOAIInputMgr.device.touch:setCallback ( onTouch )
