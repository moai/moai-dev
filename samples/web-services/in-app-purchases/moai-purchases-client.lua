----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

----------------------------------------------------------------
-- moai-purchases-client.lua - version 1.0 Beta
-- this is a first pass at support for in-app purchases throuhg
-- Moai Cloud.
----------------------------------------------------------------

-- Make this a lua module
module ( ..., package.seeall )

----------------------------------------------------------------
-- escapes a url string and returns the new string
----------------------------------------------------------------
function escape ( str )

	if str then
		
		-- convert line endings
		str = string.gsub ( str, "\n", "\r\n" )
		
		-- escape all special characters
		str = string.gsub ( str, "([^%w ])",
			function ( c ) 
				return string.format ( "%%%02X", string.byte ( c )) 
			end
		)
		
		-- convert spaces to "+" symbols
		str = string.gsub ( str, " ", "+" )
	end
	
	return str
end

----------------------------------------------------------------
local function httpGetTask ( url, callback )
	
	local task = MOAIHttpTask.new ()
	task:setCallback ( callback )
	task:httpGet ( url )
end

local function httpPostTask ( url, data, productID, callback )
	
	local task = MOAIHttpTask.new ()
	task:setCallback ( callback )
	-- add the product id ( could be nil )
	task.id = productID
	task:httpPost ( url, data )
end

local function requestCallback ( task )
	print ( task:getString ())
end

--==============================================================
-- moai purchases api
--==============================================================
local MOAI_CLOUD_BASE_URL = "http://ec2-50-18-87-104.us-west-1.compute.amazonaws.com:6767/dlc_eric"
local MOAI_CLOUD_PRODUCT_DL_REQ = "?action=get_dlc"
local MOAI_CLOUD_PRODUCT_LIST_REQ = "?action=get_products"
local MOAI_CLOUD_PRODUCT_INFO_REQ = "?action=get_product"
local MOAI_CLOUD_RECEIPT_VERIFY = "?action=verify_itunes_receipt"

----------------------------------------------------------------
-- init the dlc client with needed info
----------------------------------------------------------------
function init ( ) -- add init arguments ( appID, devID, secretKey, ...?? )

	--appID = moaiAppID
	--secretKey = moaiSecretKey
end


----------------------------------------------------------------
-- if a receipt for the product has been verified, start the product transfer
----------------------------------------------------------------
function requestProductDownload ( productName ) 

	-- build URL to send to Moai Cloud
	urlStr = MOAI_CLOUD_BASE_URL .. MOAI_CLOUD_PRODUCT_DL_REQ .. "&pid=" .. productName
	httpGetTask ( urlStr, requestCallback )
	-- figure type of file, save where needed..
end

----------------------------------------------------------------
-- Gets the available products form Moai Cloud
----------------------------------------------------------------
function requestProductInfo ()
	
	-- build URL to send to Moai Cloud
	urlStr = MOAI_CLOUD_BASE_URL .. MOAI_CLOUD_PRODUCT_INFO_REQ .. "&pid=" .. productName 
	httpGetTask ( urlStr, requestCallback )
	-- convert JSON to LUA table, return lua table of products ( prod.name, prod.cost, prod.smallIcon, prod.bigIcon, etc.. )
end

----------------------------------------------------------------
-- this will be for virtual currency items that are on Moai Cloud and NOT Apple/Google stores
----------------------------------------------------------------
function requestProductList ( onProductListReceived ) 
	
	-- build URL to send to Moai Cloud
	urlStr = MOAI_CLOUD_BASE_URL .. MOAI_CLOUD_PRODUCT_LIST_REQ 
	httpGetTask ( urlStr, onProductListReceived or requestCallback )
	-- convert JSON to LUA table, return lua table of products
end

----------------------------------------------------------------
-- sends a receipt received from Apple/Google to Moai Cloud for verification
----------------------------------------------------------------
function sendReceiptForVerification ( receiptData, productID, onVerified ) 
	
	-- build URL to send to Moai Cloud
	urlStr = MOAI_CLOUD_BASE_URL
	
	postData = "action=verify_itunes_receipt&receipt=" .. receiptData
		
	httpPostTask ( urlStr, postData, productID, onVerified or requestCallback )
end

