----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

function printTable ( t, tableName, indentationLevel )
		
	if type ( t ) ~= "table" then
		print ( "WARNING: printTable received \"" .. type ( t ) .. "\" instead of table. Skipping." )
		return
	end
	
	local topLevel = false
	
	if ( not tableName ) and ( not indentationLevel ) then
		
		topLevel = true
		indentationLevel = 1
		
		print ( "\n----------------------------------------------------------------" )
		print ( tostring ( t ) .. "\n" )
	else
		print ( "\n" .. string.rep ( "\t", indentationLevel - 1 ) .. tableName .. " = {" )
	end
	
	if t then
		for k,v in pairs ( t ) do
			
			if ( type ( v ) == "table" ) then 
				
				printTable ( v, tostring ( k ), indentationLevel + 1 )
				
			elseif ( type ( v ) == "string" ) then
				
				print ( string.rep ( "\t", indentationLevel ) .. tostring ( k ) .. " = \"" .. tostring ( v ) .. "\"," )
			else
			
				print ( string.rep ( "\t", indentationLevel ) .. tostring ( k ) .. " = " .. tostring ( v ) .. "," )
			end
		end
	end
	
	if topLevel then
		print ( "\n----------------------------------------------------------------\n" )
	else
		print ( string.rep ( "\t", indentationLevel - 1 ) .. "},\n" )
	end
end

-- broken code here
print ( "Mounting test.zip: " .. tostring ( MOAIFileSystem.mountVirtualDirectory ( "testzip", "test.zip" )))

file = io.open ( "testzip/assets/levels/main.lua", "r" )
print ( "File open success: " .. tostring ( file ) )

print ( "\nListing files in textzip/assets/levels" )
files = MOAIFileSystem.listFiles ( "testzip/assets/levels" )
printTable ( files )

print ( "\nListing directories in testzip/assets" )
files = MOAIFileSystem.listDirectories ( "testzip/assets" )
printTable ( files )