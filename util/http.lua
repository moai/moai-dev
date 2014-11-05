module ( 'http', package.seeall )

require ( 'util' )

local HTTP_TIMEOUT		= 15
local HTTP_USER_AGENT	= 'Plumzi'

local DELETE	= MOAIHttpTask.HTTP_DELETE
local GET		= MOAIHttpTask.HTTP_GET
local HEAD		= MOAIHttpTask.HTTP_HEAD
local POST		= MOAIHttpTask.HTTP_POST
local PUT		= MOAIHttpTask.HTTP_PUT

--==============================================================
-- implementation
--==============================================================

		delete			= nil
local	escapeUrl		= nil
local	encodeParams	= nil
		get				= nil
		getVerbName		= nil
		is200			= nil
		is400			= nil
		is500			= nil
		perform			= nil
		post			= nil
		put				= nil

----------------------------------------------------
delete = function ( url, params, headers, timeout )
	return perform ( DELETE, url, params, nil, headers, timeout )
end

----------------------------------------------------------------
escapeUrl = function ( str )

     if str then

		 -- convert line endings
		 str = string.gsub ( str, "\n", "\r\n" )

		 -- escape all special characters
		 str = string.gsub ( str, "([^%w ])",
			 function ( c )
					 return "%" .. string.format ( "%02X", string.byte ( c ))
			 end
		 )

		 -- convert spaces to "+" symbols
		 str = string.gsub ( str, " ", "+" )
     end

     return str
end

----------------------------------------------------------------
encodeParams = function ( t )

	if not t then return '' end

	local s = ''
	for k, v in pairs ( t ) do
		s = s .. '&' .. escapeUrl ( k ) .. '=' .. escapeUrl ( v )
	end

	if #s > 0 then
		return '?' .. string.sub ( s, 2 ) -- remove first '&'
	end

	return ''
end

----------------------------------------------------------------
get = function ( url, params, headers, timeout )
	return perform ( GET, url, params, nil, headers, timeout )
end

----------------------------------------------------------------
getVerbName = function ( verb )

	if verb == DELETE then return 'DELETE' end
	if verb == GET then return 'GET' end
	if verb == HEAD then return 'HEAD' end
	if verb == POST then return 'POST' end
	if verb == PUT then return 'PUT' end

	return 'UNKNOWN'
end

----------------------------------------------------------------
is200 = function ( status )
	return status and ( 200 <= status ) and ( status < 300 ) or false
end

----------------------------------------------------------------
is400 = function ( status )
	return status and ( 400 <= status ) and ( status < 500 ) or false
end

----------------------------------------------------------------
is500 = function ( status )
	return status and 500 <= status or false
end

----------------------------------------------------------------
perform = function ( verb, url, params, body, headers, timeout )

	url = params and url .. encodeParams ( params ) or url


	local httptask = MOAIHttpTask.new ()
	httptask:setUrl ( url )
	httptask:setVerb ( verb )
	httptask:setBody ( body )
	httptask:setTimeout ( timeout or HTTP_TIMEOUT )
	httptask:setFollowRedirects ( true )

	httptask:setHeader ( 'user-agent', HTTP_USER_AGENT )

	if headers then
		for k, v in pairs ( headers ) do
			httptask:setHeader ( k, v )
		end
	else
		httptask:setHeader ( 'accept', 'application/json' )
		httptask:setHeader ( 'content-type', 'application/json' )
	end

	httptask:performSync ()

	return httptask:getResponseCode (), httptask:getString ()
end

----------------------------------------------------------------
post = function ( url, params, json, headers, timeout )
	return perform ( POST, url, params, json, headers, timeout )
end

----------------------------------------------------------------
put = function ( url, params, json, headers, timeout )
	return perform ( PUT, url, params, json, headers, timeout )
end

