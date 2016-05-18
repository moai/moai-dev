--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

	package.path = "?;?.lua"

	----------------------------------------------------------------
	-- this function supports all the ways a user could call print
	----------------------------------------------------------------
	print = function ( ... )
		
        local arguments = {...} 
		local argCount = #arguments
		
		if argCount == 0 then
			MOAILogMgr.log ( "" )
			return
		end
		
		local output = tostring ( arguments [ 1 ])
		
		for i = 2, argCount do
			output = output .. "\t" .. tostring ( arguments [ i ])
		end
		
		MOAILogMgr.log ( output )
	end

	print ( "load:" .. "\t" .. "running init.lua" )
	
	----------------------------------------------------------------
	-- error function that actually prints the error
	----------------------------------------------------------------
	local superError = error
	
	error = function ( message, level )

	print ( "error: " .. message )
	print ( debug.traceback ( "", 2 ))
	superError ( message, level or 1 )
end
