--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

----------------------------------------------------------------
-- url.lua - version 1.0 Beta
----------------------------------------------------------------
module ( ..., package.seeall )

----------------------------------------------------------------
-- escapes a url string and returns the new string
----------------------------------------------------------------
function escape ( str )

	if str then
		
		-- convert line endings
		str = string.gsub ( str, "\n", "\r\n" )
		
		-- escape all special characters
		str = string.gsub ( str, "([^%w ])",
			function ( c ) 
				return string.format ( "%%%02X", string.byte ( c )) 
			end
		)
		
		-- convert spaces to "+" symbols
		str = string.gsub ( str, " ", "+" )
	end
	
	return str
end

----------------------------------------------------------------
-- converts a Lua table into a url encoded string of parameters
----------------------------------------------------------------
function encode ( t )

	local s = ""
	
	for k,v in pairs ( t ) do
		s = s .. "&" .. escape ( k ) .. "=" .. escape ( v )
	end
	
	return string.sub ( s, 2 ) -- remove first '&'
end
