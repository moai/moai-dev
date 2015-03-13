----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function dumpBytecodeAsHeader ( filename, headername, outname )

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

function dumpZippedTextAsHeader ( filename, headername, outname )

	print ( filename )
	print ( headername )
	print ( outname )
	
	-- dump the function to Lua bytecode
	local data = io.open ( filename, "r" ):read ( '*all' )
	data = data .. string.char ( 0 ) -- null terminate the string
	data = MOAIDataBuffer.deflate ( data, 9 )
	
	-- convert the Lua bytecode to a cpp header called 'bundled_lua' with 12 columns
	local header = MOAIDataBuffer.toCppHeader ( data, headername, 12 )
	
	-- write the header to a file
	local file = io.open ( outname, 'wb' )
	file:write ( header )
	file:close ()

end

dumpZippedTextAsHeader ( 'moai.lua', 'moai_lua', 'moai_lua.h' )
dumpZippedTextAsHeader ( 'moai_test_mgr.lua', 'moai_test_mgr_lua', 'moai_test_mgr_lua.h' )
