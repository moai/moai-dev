--==============================================================
-- Copyright (c) 2010-2012 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

module ( "cloud", package.seeall )

----------------------------------------------------------------
----------------------------------------------------------------
-- variables
----------------------------------------------------------------
local CLOUD_URL_BASE = "http://services.moaicloud.com/adam/wolfclicker/"
local CLOUD_CLIENT_KEY	= ""

local USE_CLIENT_KEY = false

--------------------------------------------------------------------
-- global helpers
--------------------------------------------------------------------
function escape ( str )

    str = string.gsub ( str, "([&=+%c])", 
		function ( c )
          return string.format( "%%%02X", string.byte ( c ))
        end
	)
	
    str = string.gsub ( str, " ", "+" )

    return str
end

----------------------------------------------------------------
function encode ( t )

	local s = ""
	
	for k,v in pairs ( t ) do
		s = s .. "&" .. escape ( k ) .. "=" .. escape ( v )
	end
	
	return string.sub ( s, 2 ) -- remove first '&'
end
--------------------------------------------------------------------
-- local helpers
--------------------------------------------------------------------
local function createTask ()
	
	local task = {}
	
	task.isFinished = false
	
	task.waitFinish = function ( self )
		while ( not self.isFinished ) do
			coroutine.yield ()
		end
		return self.result, self.responseCode
	end
	
	task.callback = function ( self )
		return function ( task, responseCode )
			self.responseCode = responseCode
			self.result = MOAIJsonParser.decode ( task:getString ())
			self.isFinished = true
		end
	end
	
	return task
end

--------------------------------------------------------------------
-- exposed functions
--------------------------------------------------------------------
function createGetTask ( urlExt, data, debug )
	
	local task = createTask ()
	
	if not data then
		data = {}
	end
	
	if USE_CLIENT_KEY then
		data.clientkey = CLOUD_CLIENT_KEY
	end
	
	task.httptask = MOAIHttpTask.new ()
	task.httptask:setCallback ( task:callback ())
	task.httptask:setUserAgent ( "Moai" )
	
	if debug then
		print ( CLOUD_URL_BASE .. urlExt .. "?" .. encode ( data ))
	end
	
	task.httptask:setUrl ( CLOUD_URL_BASE .. urlExt .. "?" .. encode ( data ))
	task.httptask:performAsync ()

	return task
end

----------------------------------------------------------------
function createPostTask ( urlExt, data, debug )
	
	local task = createTask ()
	
	task.httptask = MOAIHttpTask.new ()
	task.httptask:setCallback ( task:callback ())
	task.httptask:setUserAgent ( "Moai" )
	
	if USE_CLIENT_KEY then
		task.httpTask:setHeader ( "x-clientkey=" .. CLOUD_CLIENT_KEY )
	end
	
	if debug then
		print ( CLOUD_URL_BASE .. urlExt .. encode ( data ))
	end
	
	task.httptask:setUrl ( CLOUD_URL_BASE .. urlExt )
	task.httptask:setBody ( MOAIJsonParser.encode ( data ) )
	task.httptask:performAsync ()
	
	return task
end


