----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, iPhone!" )

function onRemoteRegistrationComplete ( code, token )

	print ( "onRemoteRegistrationComplete: " )
	
	if ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_REGISTERED ) then

		print ( "registered: " .. token )
	elseif ( code == MOAINotifications.REMOTE_NOTIFICATION_RESULT_UNREGISTERED ) then

		print ( "unregistered" )
	else

		print ( "registration failed" )
	end
end

function onRemoteMessageReceived ( bundle )

	print ( "onRemoteMessageReceived: " )
	
	local message = bundle.aps.alert
	
	local action, data, title
	if bundle.action then action = bundle.action end
	if bundle.data then data = bundle.data end
	if bundle.title then title = bundle.title end

	print ( "message: " .. message )
	print ( "action: " .. action )
	print ( "data: " .. data )
	print ( "title: " .. title )
end

MOAINotifications.setListener ( MOAINotifications.REMOTE_NOTIFICATION_REGISTRATION_COMPLETE, onRemoteRegistrationComplete )
MOAINotifications.setListener ( MOAINotifications.REMOTE_NOTIFICATION_MESSAGE_RECEIVED, onRemoteMessageReceived )
MOAINotifications.setAppIconBadgeNumber ( 0 )
MOAINotifications.registerForRemoteNotifications ( MOAINotifications.REMOTE_NOTIFICATION_BADGE + MOAINotifications.REMOTE_NOTIFICATION_ALERT )

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

prop:moveRot ( 360, 1.5 )
