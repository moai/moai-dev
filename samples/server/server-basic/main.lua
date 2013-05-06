----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'HELLO, WEB!' )

function onHttpRequest ( self, method, uri, queryString, headers )

	print ( 'HTTP REQUEST' )
	
	for k, v in pairs ( headers ) do
		print ( k, v )
	end
	
	return 200, 'text/plain', string.format ( '%s %s%s', method, uri, queryString or '' )
end

server = MOAIHttpServer.new ()
server:setListener ( MOAIHttpServer.EVENT_HANDLE_REQUEST, onHttpRequest )
server:start ({
	listening_ports = '8080',
})
