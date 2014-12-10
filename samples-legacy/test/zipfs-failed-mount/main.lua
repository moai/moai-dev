----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

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
print ( "Mounting test.zip: " .. tostring ( MOAIFileSystem.mountVirtualDirectory ( "tropics", "tropics.zip" )))

print ( "\nListing files" )
files = MOAIFileSystem.listFiles ( "tropics" )
printTable ( files )

print ( "\nListing directories" )
files = MOAIFileSystem.listDirectories ( "tropics" )
printTable ( files )