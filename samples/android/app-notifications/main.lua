----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, Android!" )

function onRemoteRegistrationComplete ( code, registration )
	print ( "onRemoteRegistrationComplete: " )
	
	if ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_REGISTERED ) then
		print ( "registered: " .. registration )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_UNREGISTERED ) then
		print ( "unregistered" )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_ERROR_SERVICE_NOT_AVAILABLE ) then
		print ( "registration failed: Service not available" )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_ERROR_ACCOUNT_MISSING ) then
		print ( "registration failed: Google account missing" )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_ERROR_AUTHENTICATION_FAILED ) then
		print ( "registration failed: Authentication failed" )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_ERROR_TOO_MANY_REGISTRATIONS ) then
		print ( "registration failed: Too many registrations" )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_ERROR_INVALID_SENDER ) then
		print ( "registration failed: Invalid sender" )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_ERROR_PHONE_REGISTRATION_ERROR ) then
		print ( "registration failed: Phone registration error" )
	else
		print ( "registration failed: Unknown error" )
	end
end

function onRemoteMessageReceived ( bundle )
	print ( "onRemoteMessageReceived: " )
	
	for key, value in pairs ( bundle ) do 
		print ( key .. " = " .. value ) 
	end
end

MOAINotifications.setListener ( MOAINotifications.REMOTE_NOTIFICATION_REGISTRATION_COMPLETE, onRemoteRegistrationComplete )
MOAINotifications.setListener ( MOAINotifications.REMOTE_NOTIFICATION_MESSAGE_RECEIVED, onRemoteMessageReceived )

MOAINotifications.registerForRemoteNotifications ( "SET YOUR GOOGLE C2DM SENDER EMAIL ALIAS HERE" )
-- MOAINotifications.unregisterForRemoteNotifications ()

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

prop:moveRot ( 720, 5.0 )
