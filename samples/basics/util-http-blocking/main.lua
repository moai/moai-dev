----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function onFinish ( task, responseCode )

	print ( "onFinish" )
	print ( responseCode )

	if ( task:getSize ()) then
		print ( task:getString ())
	else
		print ( "nothing" )
	end
end

task = MOAIHttpTask.new ()

task:setCallback ( onFinish )
task:httpGet ( "www.cnn.com", 0, false, true )

