local m = {
}

local function getSaveDir()
    -- local saveDir = MOAIEnvironment.documentDirectory .. "/Saves/"
     -- MOAIFileSystem.affirmPath(saveDir)
	 local saveDir = MOAIFileSystem.getWorkingDirectory ()
    return saveDir
end

function m:saveTable(saveTable, fileName)
    local saveDir = getSaveDir()
    local jsonTable = MOAIJsonParser.encode( saveTable )
    local dataBuffer = MOAIDataBuffer.new()
    dataBuffer:setString( jsonTable )   
    dataBuffer:deflate()             
    dataBuffer:save( saveDir .. fileName .. ".sav", false )
end

function m:loadTable(fileName)
    local saveDir = getSaveDir()
    local dataBuffer = MOAIDataBuffer.new()
    dataBuffer:load( saveDir .. fileName .. ".sav" )
    -- dataBuffer:inflate()
    local jsonTable = dataBuffer:getString()
    return MOAIJsonParser.decode( jsonTable )
end

function m:loadTableInflate(fileName)
    local saveDir = getSaveDir()
    local dataBuffer = MOAIDataBuffer.new()
    dataBuffer:load( saveDir .. fileName .. ".sav" )
    dataBuffer:inflate()
    local jsonTable = dataBuffer:getString()
    return MOAIJsonParser.decode( jsonTable )
end

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

return m