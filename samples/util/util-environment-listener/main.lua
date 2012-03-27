----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

print ( 'screenWidth', MOAIEnvironment.screenWidth )
print ( 'screenHeight', MOAIEnvironment.screenHeight )

function onEventValueChanged ( key, value )
	print ( key, value )
end

MOAIEnvironment.setListener ( MOAIEnvironment.EVENT_VALUE_CHANGED, onEventValueChanged )
MOAIEnvironment.setValue ( 'appDisplayName', 'foo' )