----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function dumpHeader ( filename, headername, outname )

	print ( filename )
	print ( headername )
	print ( outname )
	
	-- dump the function to Lua bytecode
	local compiled = string.dump ( loadfile ( filename ))
	
	-- convert the Lua bytecode to a cpp header called 'bundled_lua' with 12 columns
	local header = MOAIDataBuffer.toCppHeader ( compiled, headername, 12 )
	
	-- write the header to a file
	local file = io.open ( outname, 'wb' )
	file:write ( header )
	file:close ()

end

dumpHeader ( 'moai.lua', 'moai_lua', 'moai_lua.h' )




