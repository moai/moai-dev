----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, iPhone!" )

function onError ( error )
	
	print ( 'onError' )
	print ( error )
end

function onRegister ( token )

	print ( 'onRegister' )
	print ( token )
end

function onRemoteNotification ( userInfoTable )

	print ( 'onRemoteNotification' )
	print ( userInfoTable )
end

MOAIApp.setListener ( MOAIApp.ERROR, onError )
MOAIApp.setListener ( MOAIApp.DID_REGISTER, onRegister )
MOAIApp.setListener ( MOAIApp.REMOTE_NOTIFICATION, onRemoteNotification )

MOAIApp.setAppIconBadgeNumber ( 0 )
MOAIApp.registerForRemoteNotifications ( MOAIApp.REMOTE_NOTIFICATION_BADGE + MOAIApp.REMOTE_NOTIFICATION_ALERT )