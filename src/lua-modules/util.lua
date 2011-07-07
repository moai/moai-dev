--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

----------------------------------------------------------------
-- util.lua - version 1.0 Beta
----------------------------------------------------------------
module ( ..., package.seeall )

----------------------------------------------------------------
-- returns true if value is a function
----------------------------------------------------------------
function isFunc ( value )

	return type ( value ) == "function"
end

----------------------------------------------------------------
-- returns true if the file exists
----------------------------------------------------------------
function fileExists ( filename )

	local success, file = pcall ( io.open, filename )
	
	if ( not success ) or ( file == nil ) then 
		return false
	end
	
	io.close ( file ) 
	return true
end
