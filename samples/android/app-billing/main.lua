----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, Android!" )

function onBillingSupported ( supported )

	print ( "onBillingSupported: " )

	if ( supported ) then

		print ( "billing is supported" )
		
		if MOAIBilling.requestPurchase ( 'sword_001', 'this is a payload available to developers that is returned as part of the purchase state changed callback' ) then
--		if MOAIBilling.restoreTransactions () then

			print ( "purchase successfully requested" )
--			print ( "restore transactions successfully requested" )
		else

			print ( "requesting purchase failed" )
--			print ( "requesting transaction restore failed" )
		end
	else

		print ( "billing is not supported" )
	end
end

function onPurchaseResponseReceived ( code, id )

	print ( "onPurchaseResponseReceived: " .. id )
	
	if ( code == MOAIBilling.BILLING_RESULT_SUCCESS ) then

		print ( "purchase request received" )
	elseif ( code == MOAIBilling.BILLING_RESULT_USER_CANCELED ) then

		print ( "user canceled purchase" )
	else

		print ( "purchase failed" )
	end
end

function onPurchaseStateChanged ( code, id, order, user, notification, payload )

	print ( "onPurchaseStateChanged: " .. id )

	if ( code == MOAIBilling.BILLING_PURCHASE_STATE_ITEM_PURCHASED ) then

		print ( "item has been purchased" )
	elseif ( code == MOAIBilling.BILLING_PURCHASE_STATE_ITEM_REFUNDED ) then

		print ( "item has been refunded" )
	else

		print ( "purchase was canceled" )
	end

	if ( notification ~= nil ) then

		if MOAIBilling.confirmNotification ( notification ) ~= true then

			print ( "failed to confirm notification" )
		end
	end
end

function onRestoreResponseReceived ( code, more, offset )

	print ( "onRestoreResponseReceived: " )

	if ( code == MOAIBilling.BILLING_RESULT_SUCCESS ) then

		print ( "restore request received" )
	
		if ( more ) then
	
			MOAIBilling.restoreTransactions ( offset )
		end
	else

		print ( "restore request failed" )
	end
end

MOAIBilling.setListener ( MOAIBilling.CHECK_BILLING_SUPPORTED, onBillingSupported )
MOAIBilling.setListener ( MOAIBilling.PURCHASE_RESPONSE_RECEIVED, onPurchaseResponseReceived )
MOAIBilling.setListener ( MOAIBilling.PURCHASE_STATE_CHANGED, onPurchaseStateChanged )
MOAIBilling.setListener ( MOAIBilling.RESTORE_RESPONSE_RECEIVED, onRestoreResponseReceived )

MOAIBilling.setPublicKey ( "SET YOUR ANDROID MARKET PUBLIC KEY HERE. SEE https://market.android.com/publish/editProfile" )

if not MOAIBilling.setBillingProvider ( MOAIBilling.BILLING_PROVIDER_GOOGLE ) then

	print ( "unable to set billing provider" )
else

	if not MOAIBilling.checkBillingSupported () then

		print ( "check billing supported failed" )
	end
end

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

prop:moveRot ( 360, 3.0 )
