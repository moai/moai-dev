----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

task = nil

function onFinish ( task, responseCode )

	print ( "onFinish" )
	print ( responseCode )

	cookies = task:getResponseHeader("Set-Cookie")
	
	print ( "cookies " .. cookies )
	
end

function sendHTTPRequest ( url )

	task = MOAIHttpTask.new ()
	
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
	task:performSync ()

end

sendHTTPRequest ( "www.google.com" )
