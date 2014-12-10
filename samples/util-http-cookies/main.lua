----------------------------------------------------------------
-- Copyright (c) 2010-2012 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 32, 32 )
task = nil

function onFinish ( task, responseCode )

	print ( "onFinish" )
	print ( responseCode )

	if ( task:getSize ()) then
		print ( task:getString ())
	else
		print ( "nothing" )
	end
	
	sendHTTPRequest ( "www.google.com" )
end

function sendHTTPRequest ( url )

	local task = MOAIHttpTask.new ()
	
	task:setVerb ( MOAIHttpTask.HTTP_GET )
	task:setUrl ( url )
	task:setCallback ( onFinish )
	task:setUserAgent ( "Moai" )
	task:setHeader ( "Foo", "foo" )
	task:setHeader ( "Bar", "bar" )
	task:setHeader ( "Baz", "baz" )
	task:setCookieSrc ( "cookie.txt" )
	task:setCookieDst ( "cookie.txt" )
	task:setVerbose ( true )
	task:performAsync ()
end

sendHTTPRequest ( "www.google.com" )
