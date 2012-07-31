----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, Android!" )

function onRemoteRegistrationComplete ( code, token )
	print ( "onRemoteRegistrationComplete: " )
	
	if ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_REGISTERED ) then
		print ( "registered: " .. token )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_UNREGISTERED ) then
		print ( "unregistered" )
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
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 720, 5.0 )
