--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

----------------------------------------------------------------
-- tapjoy.lua - version 1.0 Beta
-- this is a first pass at support for tapjoy adds and offer walls
-- we plan to extend to to cover all tapjoy's features, and all as
-- support other platforms besides iOS
----------------------------------------------------------------
if MOAIWebView == nil then
	error ( "currently, tapjoy is only supported on iOS. Support for Android and other platforms coming soon.", 2 )
	return
end

module ( ..., package.seeall )

require "crypto"
require "url"
require "util"

--==============================================================
-- util functions
--==============================================================

----------------------------------------------------------------
local function shouldStartLoadWithRequestListener ( webView, request, navType )
	
	if request == "https://ws.tapjoyads.com/dismiss" then
		webView:hideWebView ()
		return false
	end
	
	if navType == MOAIWebView.NAVIGATION_LINK_CLICKED then
		MOAIWebView.openUrlInSafari ( request )
		return false
	end

	return true
end

----------------------------------------------------------------
local function createPropFromBannerCallback ( task )

	if task:getSize () > 0 then
	
		local jsonTable = MOAIJsonParser.decode ( task:getString () )
	
		local texData = MOAIDataBuffer.new ()
		texData:setString ( MOAIDataBuffer.base64Decode ( jsonTable.Image ))
		
		local texture = MOAITexture.new ()
		texture:load ( texData )
		
		local texW, texH = texture:getSize ()
		
		local gfxQuad = MOAIGfxQuad2D.new ()
		gfxQuad:setTexture ( texture )
		gfxQuad:setRect ( -( texW / 2 ), -( texH / 2 ), ( texW / 2 ), ( texH / 2 ))

		local prop = MOAIProp2D.new ()
		prop:setDeck ( gfxQuad )
	
		if util.isFunc ( task.userDoneCallback ) then
			task.userDoneCallback ( task, prop )
		end
	end
end

----------------------------------------------------------------
local function createWebViewFromFeaturedCallback ( task )

	if task:getSize () > 0 then
		
		local jsonTable = MOAIJsonParser.decode ( task:getString () )
		
		local width, height = MOAIEnvironment.getViewSize ()
		
		if MOAIEnvironment.isRetinaDisplay () then
			width = width / 2
			height = height / 2
		end
		
		local webView = MOAIWebView.new ()
		webView:setListener ( MOAIWebView.SHOULD_START_LOAD_WITH_REQUEST, shouldStartLoadWithRequestListener )
		
		webView:setListener ( MOAIWebView.WEB_VIEW_DID_FINISH_LOAD,
			function ( self )
				if util.isFunc ( task.userDoneCallback ) then
					task.userDoneCallback ( task, self )
				end
			end
		)
		
		webView:initWebView ( 0, 0, width, height, true )	
		webView:loadRequest ( jsonTable.OfferArray[ 1 ].FullScreenAdURL )
	end
end

----------------------------------------------------------------
local function createWebViewFromOfferCallback ( task )

	if task:getSize () > 0 then
		
		local width, height = MOAIEnvironment.getViewSize ()

		if MOAIEnvironment.isRetinaDisplay () then
			width = width / 2
			height = height / 2
		end
		
		local webView = MOAIWebView.new ()
		webView:setListener ( MOAIWebView.SHOULD_START_LOAD_WITH_REQUEST, shouldStartLoadWithRequestListener )
		
		webView:setListener ( MOAIWebView.WEB_VIEW_DID_FINISH_LOAD,
			function ( self )
				if util.isFunc ( task.userDoneCallback ) then
					task.userDoneCallback ( task, self )
				end
			end
		)
		
		webView:hasToolBar ( true ) 
		webView:initWebView ( 0, 0, width, height, true )
		webView:loadHTML ( task:getString () )
	end
end

----------------------------------------------------------------
local function getTimeStampString ()

	return tostring ( os.time () )
end

----------------------------------------------------------------
local function getVerifier ( ... )

	local verifier = arg [ 1 ] 
	
	for i,v in ipairs ( arg ) do
		if i ~= 1 then
			verifier = verifier .. v .. ":"
		end
	end
	
	return crypto.evp.digest ( "sha256", verifier )
end

----------------------------------------------------------------
local function httpGetTask ( url, callback, userDoneCallback )
	
	local task = MOAIHttpTask.new ()
	task.userDoneCallback = userDoneCallback
	task:setCallback ( callback )
	task:httpGet ( url )
end

--==============================================================
-- tapjoy api
--==============================================================
local BANNER_AD_BASE_URL = "https://ws.tapjoyads.com/display_ad.json"
local FEATURE_AD_BASE_URL = "https://ws.tapjoyads.com/get_offers/featured"
local OFFERS_WEB_BASE_URL = "https://ws.tapjoyads.com/get_offers/webpage"

local mAppId
local mCallbackUrl
local mPubId
local mAppSecretKey
local mUdid

----------------------------------------------------------------
function init ( appId, appSecretkey, publisherId )

	-- validate parameters
	if appId == nil then
		error ( "tapjoy.init is missing required \"appId\" parameter", 2 )
	end

	if appSecretkey == nil then
		error ( "tapjoy.init is missing required \"appSecretkey\" parameter", 2 )
	end

	-- init module level variables	
	mAppId = appId
	mAppSecretKey = appSecretkey
	mUdid = MOAIEnvironment.getUDID ()
	mPubId = publisherId or mUdid
end

----------------------------------------------------------------
-- valid values for bannerSize are: "320x50", "640x100", "768x90"
----------------------------------------------------------------
function getBannerAdProp ( bannerSize, callbackFunc )

	if ( not mAppId ) or ( not mAppSecretKey ) then
		error ( "tapjoy.getBannerAdProp called before tapjoy.init was called with valid parameters", 2 )
	end

	local timeStamp = getTimeStampString ()
	
	local parameters = url.encode {
		app_id					= mAppId,
		udid					= mUdid,
		publisher_user_id		= mPubId,
		size					= bannerSize,
		time_stamp				= timeStamp,
		verifier				= getVerifier ( mAppId, mUdid, timeStamp, mAppSecretKey ),
		device_type				= MOAIEnvironment.getDevModel (),
		os_version				= MOAIEnvironment.getOSVersion (),
		country_code			= MOAIEnvironment.getCountryCode (),
		language_code			= MOAIEnvironment.getLanguageCode (),
		app_version				= MOAIEnvironment.getAppVersion (),
		carrier_name			= MOAIEnvironment.getCarrierName (),
		carrier_country_code	= MOAIEnvironment.getCarrierISOCountryCode (),
		mobile_country_code		= MOAIEnvironment.getCarrierMobileCountryCode (),
		mobile_network_code		= MOAIEnvironment.getCarrierMobileNetworkCode (),
	}
	
	httpGetTask ( BANNER_AD_BASE_URL .. "?" .. parameters, createPropFromBannerCallback, callbackFunc )
end

----------------------------------------------------------------
function showFeaturedAppWebView ( callbackFunc )
	
	if ( not mAppId ) or ( not mAppSecretKey ) then
		error ( "tapjoy.showFeaturedAppWebView called before tapjoy.init was called with valid parameters", 2 )
	end

	local timeStamp = getTimeStampString ()

	local parameters = url.encode {
		app_id					= mAppId,
		udid					= mUdid,
		publisher_user_id		= mPubId,
		time_stamp				= timeStamp,
		json					= "1",
		verifier				= getVerifier ( mAppId, mUdid, timeStamp, mAppSecretKey ),
		device_type				= MOAIEnvironment.getDevModel (),
		os_version				= MOAIEnvironment.getOSVersion (),
		country_code			= MOAIEnvironment.getCountryCode (),
		language_code			= MOAIEnvironment.getLanguageCode (),
		app_version				= MOAIEnvironment.getAppVersion (),
		carrier_name			= MOAIEnvironment.getCarrierName (),
		carrier_country_code	= MOAIEnvironment.getCarrierISOCountryCode (),
		mobile_country_code		= MOAIEnvironment.getCarrierMobileCountryCode (),
		mobile_network_code		= MOAIEnvironment.getCarrierMobileNetworkCode (),
	}		
		
	httpGetTask ( FEATURE_AD_BASE_URL .. "?" .. parameters, createWebViewFromFeaturedCallback, callbackFunc )
end

----------------------------------------------------------------
function showOffersWebView ( callbackFunc )

	if ( not mAppId ) or ( not mAppSecretKey ) then
		error ( "tapjoy.showOffersWebView called before tapjoy.init was called with valid parameters", 2 )
	end

	local timeStamp = getTimeStampString ()
	
	local parameters = url.encode {
		app_id					= mAppId,
		udid					= mUdid,
		publisher_user_id		= mPubId,
		time_stamp				= timeStamp,
		verifier				= getVerifier ( mAppId, mUdid, timeStamp, mAppSecretKey ),
		device_type				= MOAIEnvironment.getDevModel (),
		os_version				= MOAIEnvironment.getOSVersion (),
		country_code			= MOAIEnvironment.getCountryCode (),
		language_code			= MOAIEnvironment.getLanguageCode (),
		app_version				= MOAIEnvironment.getAppVersion (),
		carrier_name			= MOAIEnvironment.getCarrierName (),
		carrier_country_code	= MOAIEnvironment.getCarrierISOCountryCode (),
		mobile_country_code		= MOAIEnvironment.getCarrierMobileCountryCode (),
		mobile_network_code		= MOAIEnvironment.getCarrierMobileNetworkCode (),
	}
	
	httpGetTask ( OFFERS_WEB_BASE_URL .. "?" .. parameters, createWebViewFromOfferCallback, callbackFunc )
end
