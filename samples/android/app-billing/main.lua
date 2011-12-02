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
		
		if MOAIApp.requestPurchase ( 'sword_001', 'this is a payload available to developers that is returned as part of the purchase state changed callback' ) then
--		if MOAIApp.restoreTransactions () then
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

function onPurchaseResponseReceived ( id, code )
	print ( "onPurchaseResponseReceived: " .. id )
	
	if ( code == MOAIApp.BILLING_RESULT_OK ) then
		print ( "purchase request received" )
	elseif ( code == MOAIApp.BILLING_RESULT_USER_CANCELED ) then
		print ( "user canceled purchase" )
	else
		print ( "purchase failed" )
	end
end

function onPurchaseStateChanged ( id, code, order, notification, payload )
	print ( "onPurchaseStateChanged: " .. id )

	if ( code == MOAIApp.BILLING_STATE_ITEM_PURCHASED ) then
		print ( "item has been purchased" )
	elseif ( code == MOAIApp.BILLING_STATE_ITEM_REFUNDED ) then
		print ( "item has been refunded" )
	else
		print ( "purchase was canceled" )
	end

	if ( notification ~= 0 ) then
		if MOAIApp.confirmNotification ( notification ) ~= true then
			print ( "failed to confirm notification" )
		end
	end
end

function onRestoreResponseReceived ( code )
	print ( "onRestoreResponseReceived: " )
	
	if ( code == MOAIApp.BILLING_RESULT_OK ) then
		print ( "restore request received" )
	else
		print ( "restore failed" )
	end
end

MOAIApp.setListener ( MOAIApp.CHECK_BILLING_SUPPORTED, onBillingSupported )
MOAIApp.setListener ( MOAIApp.PURCHASE_RESPONSE_RECEIVED, onPurchaseResponseReceived )
MOAIApp.setListener ( MOAIApp.PURCHASE_STATE_CHANGED, onPurchaseStateChanged )
MOAIApp.setListener ( MOAIApp.RESTORE_RESPONSE_RECEIVED, onRestoreResponseReceived )

MOAIApp.setMarketPublicKey ( "SET YOUR ANDROID MARKET PUBLIC KEY HERE. SEE https://market.android.com/publish/editProfile" )

if MOAIApp.checkBillingSupported () ~= true then
	print ( "cannot connect to billing service" )
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
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 3.0 )
