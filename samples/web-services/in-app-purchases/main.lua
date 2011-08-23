----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------
----------------------------------------------------------------
-- MOAI App Setup
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

MOAIApp.setAppIconBadgeNumber ( 0 )
MOAIApp.registerForRemoteNotifications ( MOAIApp.REMOTE_NOTIFICATION_BADGE + MOAIApp.REMOTE_NOTIFICATION_ALERT )

-- Bring in the module to talk to Moai Cloud in-app purchase server
purchase = require "moai-purchases-client"

----------------------------------------------------------------
-- Moai Cloud setup
----------------------------------------------------------------
function onProductListReturned ( httpTask )

	cloudProducts = MOAIJsonParser.decode ( httpTask:getString ())	-- build a request table for apple
	
	products = {}
	if cloudProducts.id ~= nil then
	
		table.insert ( products, cloudProducts.id )
	else
		for k, v in pairs ( cloudProducts ) do
			
			table.insert ( products, v.id )
		end
	end
	
	-- request products from Apple, the rest is handled in callbacks
	MOAIApp.requestProductIdentifiers ( products )	
end

function onReceiptVerified ( httpTask )
	
	-- receipt verified, download dlc
	-- id added to httpTask during moai-purchases-client stuffs
	print ( "receipt for product: " .. httpTask.id )
	--purchase.requestProductDownload ( httpTask.id )
	print ( "receipt verification: " .. httpTask:getString ())
end

----------------------------------------------------------------
-- Store Kit setup
----------------------------------------------------------------
function onPaymentQueueTransaction ( transaction )

	print ( 'onPaymentQueueTransaction' )
	
	-- verify receipt once transaction is done
	if transaction.transactionState == 1 then
		receiptData = MOAIDataBuffer.base64Encode ( transaction.transactionReceipt )
		purchase.sendReceiptForVerification ( receiptData, transaction.payment.productIdentifier, onReceiptVerified )
	end
end

function onProductRequestResponse ( products )
	print ( 'onProductRequestResponse' )
	
	-- Buy each product available from Apple
	for k, v in pairs ( products ) do
		MOAIApp.requestPaymentForProduct ( v.productIdentifier )
	end
end

----------------------------------------------------------------
-- MOAI In App Purchases example
----------------------------------------------------------------

-- Check that we can make Store Kit payments - if we can, kick off a request - verify - download chain of events
if MOAIApp.canMakePayments () then

	print ( "can make payments" )

	MOAIApp.setListener ( MOAIApp.PAYMENT_QUEUE_TRANSACTION, onPaymentQueueTransaction )
	MOAIApp.setListener ( MOAIApp.PRODUCT_REQUEST_RESPONSE, onProductRequestResponse )

	-- get products available on cloud
	purchase.requestProductList ( onProductListReturned )
end

----------------------------------------------------------------
