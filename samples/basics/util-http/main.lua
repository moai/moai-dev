----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function onFinish ( task )

	print ( "onFinish" )

	if ( task:getSize ()) then
		print ( task:getString ())
	else
		print ( "nothing" )
	end
end

task = MOAIHttpTask.new ( "GSLHttpTask" )

task:setCallback ( onFinish )
task:httpGet ( "http://www.w3schools.com/XML/note.xml" )

MOAISim.openWindow ( "httpTask", 32, 32 )