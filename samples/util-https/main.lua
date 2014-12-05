----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 32, 32 )

function onFinish ( task, responseCode )

	print ( "onFinish" )
	print ( responseCode )

	if ( task:getSize ()) then
		print ( task:getString ())
	else
		print ( "nothing" )
	end
end

task = MOAIHttpTask.new ( "GSLHttpTask" )

task:setCallback ( onFinish )
task:httpGet ( "https://my.webfaction.com/" )

