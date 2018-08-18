----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

if MOAIEnvironment.osBrand ~= MOAIEnvironment.OS_BRAND_IOS then
	print ( 'iOS only sample - please run on an iOS device or simulator' )
	os.exit ()
end

viewWidth, viewHeight = MOAIGfxMgr.getViewSize ()

viewport = MOAIViewport.new ()
viewport:setSize ( viewWidth, viewHeight )
viewport:setScale ( viewWidth, viewHeight )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -640, -640, 640, 640 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )

prop:moveRot ( 360, 1.5 )

function onResize ( width, height )
	viewport:setSize ( width, height )
	viewport:setScale ( width, height )
	print ( 'RESIZED', width, height )
end

MOAIGfxMgr.setListener ( MOAIGfxMgr.EVENT_RESIZE, onResize )

onTouch = function ( eventType )

	if eventType == MOAITouchSensor.TOUCH_DOWN then
		
		webView = MOAIWebViewIOS.new ()
		webView:show ( true )
		webView:loadRequest ( 'https://news.google.com' )
		webView = nil
	end
end

MOAIInputMgr.device.touch:setCallback ( onTouch )

