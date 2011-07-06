----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

package.path = package.path .. ";../../src/lua-modules/?.lua"
require "tapjoy"

tapjoy.init ( "c38e3b26-3966-4519-82e4-615df57f809f", "lPo9KaVidLxQINpohxw5" )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

-----------------------------------------------------------------
-- Offers WebView example
-----------------------------------------------------------------

--function shouldStart ()

--	return true
--end

--webView = MOAIWebView.new ()
--webView:initWebView ( 20, 20, 280, 210, false )
--webView:setListener ( MOAIWebView.SHOULD_START_LOAD_WITH_REQUEST, shouldStart )

--webViewTwo = MOAIWebView.new ()
--webViewTwo:initWebView ( 20, 250, 280, 210, false )

--function httpCallback ( httpTask )

--	if ( httpTask:getSize () > 0 ) then	
--		webView:loadHTML ( httpTask:getString ())
--		webViewTwo:loadRequest ( "http://www.google.com" )
--	end
--end

--tapjoy.getOffersWeb ( httpCallback )

-----------------------------------------------------------------
-- End Offers WebView example
-----------------------------------------------------------------

-----------------------------------------------------------------
-- Banner ad example
-----------------------------------------------------------------

--function bannerCreatedCallback ( bannerProp )

--	layer:insertProp ( bannerProp )
--end

--tapjoy.getBannerAdProp ( "320x50", bannerCreatedCallback )

-----------------------------------------------------------------
-- End Banner ad example
-----------------------------------------------------------------

-----------------------------------------------------------------
-- Featured app ad example
-----------------------------------------------------------------

function webviewCreatedCallback ( webView )

	webView:showWebView ( )
end

tapjoy.getFeaturedAppWebView ( webviewCreatedCallback )

-----------------------------------------------------------------
-- End Featured app ad example
-----------------------------------------------------------------