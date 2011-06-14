----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIApp.setAppIconBadgeNumber ( 0 )
MOAIApp.registerForRemoteNotifications ( MOAIApp.REMOTE_NOTIFICATION_BADGE + MOAIApp.REMOTE_NOTIFICATION_ALERT )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

------------CALLBACKS-------------
initLoad = false;

function startLoad ()

	print ( "lua - startLoad" ) 
end

function finishLoad ()
		
	print ( "lua - finishLoad" )
end

function loadError ( error )
	
	print ( "lua - loadError" )
	print ( error )
end

function shouldLoad ( requestURL, navType )
	
	print ( "lua - shouldLoad" )
	print ( requestURL )
	if requestURL == "https://google.com" then 
		return false
	end
	if navType == MOAIWebView.NAVIGATION_LINK_CLICKED then 
		return false
	end
	return true
end

webView = MOAIWebView.new ()
webView:setListener ( MOAIWebView.WEB_VIEW_DID_START_LOAD, startLoad )
webView:setListener ( MOAIWebView.WEB_VIEW_DID_FINISH_LOAD, finishLoad )
webView:setListener ( MOAIWebView.DID_FAIL_LOAD_WITH_ERROR, loadError )
webView:setListener ( MOAIWebView.SHOULD_START_LOAD_WITH_REQUEST, shouldLoad )

webView:loadRequest ( "https://google.com" )




