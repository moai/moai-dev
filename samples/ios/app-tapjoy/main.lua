----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, iPhone!" )

MOAITapjoy.setListener ( MOAITapjoy.TAPJOY_VIDEO_AD_BEGIN, function () print ( 'Video Ad started...' ) if ENABLE_SOUND then MOAIUntzSystem.setVolume ( 0 ) end end )
MOAITapjoy.setListener ( MOAITapjoy.TAPJOY_VIDEO_AD_CLOSE, function () print ( 'Video Ad closed' ) if ENABLE_SOUND then MOAIUntzSystem.setVolume ( 1 ) end end )

MOAITapjoy.init ( "TAPJOY APP KEY", "TAPJOY APP SECRET" )

print ( "Tapjoy User Id: " .. MOAITapjoy.getUserId () )

MOAITapjoy.initVideoAds ( 2 )

MOAITapjoy.showOffers ()

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

prop:moveRot ( 720, 2.0 )