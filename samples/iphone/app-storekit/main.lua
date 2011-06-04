----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, iPhone!" )

print ( iphone.documents )
print ( iphone.caches )
print ( iphone.resources )

function onPaymentQueueTransaction ( transaction )

	print ( 'onPaymentQueueTransaction' )
end

function onProductRequestResponse ( products )

	print ( 'onProductRequestResponse' )
	
	for k, v in pairs ( products ) do
	
		print ( 'product:' )
		
		print ( v.localizedTitle )
		print ( v.localizedDescription )
		print ( v.price )
		print ( v.priceLocale )
		print ( v.productIdentifier )
		
		print ( '\n' )
		
		MOAIApp.requestPaymentForProduct ( v.productIdentifier )
	end
end

if MOAIApp.canMakePayments () then

	print ( "can make payments" )

	MOAIApp.setListener ( MOAIApp.PAYMENT_QUEUE_TRANSACTION, onPaymentQueueTransaction )
	MOAIApp.setListener ( MOAIApp.PRODUCT_REQUEST_RESPONSE, onProductRequestResponse )

	products = {}
	table.insert ( products, 'test_consumable_01' )

	MOAIApp.requestProductIdentifiers ( products )
end


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
