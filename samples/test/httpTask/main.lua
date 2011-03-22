
function onFinish ( task )

	print ( "onFinish" )

	if ( task:getSize ()) then
		print ( task:getText ())
	else
		print ( "nothing" )
	end
end

task = MOAIHttpTask.new ( "GSLHttpTask" )

task:setCallback ( onFinish )
task:httpGet ( "http://www.w3schools.com/XML/note.xml" )

MOAISim.openWindow ( "httpTask", 320, 480 )
