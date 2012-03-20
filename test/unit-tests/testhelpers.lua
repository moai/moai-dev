----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

module ( "test", package.seeall )

local eps = .1
local pass = 0
local fail = 0

local threads = {}
local count = 1

local failedtests = {}

function epsilon ( val1, val2 )
	return math.abs ( val1 - val2 ) <= eps
end

function evaluate ( condition, str )
	print ( ( condition and "PASS" or "FAILED"), str )
	if condition then
		pass = pass + 1
	else
		fail = fail + 1
		table.insert ( failedtests, "FAILED " .. str )
	end
end

local function makeLog ( t, tableName, indentationLevel )
	file = io.open ( "out.log", "w" )
	
	file:write ( "Failed tests:\n" )
	
	if type ( t ) ~= "table" then
		file:write ( "WARNING: printTable received \"" .. type ( t ) .. "\" instead of table. Skipping." )
		return
	end
	
	local topLevel = false
	
	if ( not tableName ) and ( not indentationLevel ) then
		
		topLevel = true
		indentationLevel = 1
		
		file:write ( "\n----------------------------------------------------------------" )
		file:write ( tostring ( t ) .. "\n" )
	else
		file:write ( "\n" .. string.rep ( "\t", indentationLevel - 1 ) .. tableName .. " = {" )
	end
	
	if t then
		for k,v in pairs ( t ) do
			
			if ( type ( v ) == "table" ) then 
				
				makeLog ( v, tostring ( k ), indentationLevel + 1 )
				
			elseif ( type ( v ) == "string" ) then
				
				file:write ( string.rep ( "\t", indentationLevel ) .. tostring ( k ) .. " = \"" .. tostring ( v ) .. "\",\n" )
			else
			
				file:write ( string.rep ( "\t", indentationLevel ) .. tostring ( k ) .. " = " .. tostring ( v ) .. ",\n" )
			end
		end
	end
	
	if topLevel then
		file:write ( "\n----------------------------------------------------------------\n" )
	else
		file:write ( string.rep ( "\t", indentationLevel - 1 ) .. "},\n" )
	end
	
	file:flush ()
	
	file:close ()
end

function printResults ()
	for i = 1, count - 1 do
		while threads [ i ]:isBusy () do
			coroutine.yield ()
		end
	end
	
	print ( "\n" .. pass .. " tests passed, " .. fail .. " tests failed." )
	
	if fail > 0 then
		print ( "Log file created." )
		makeLog ( failedtests )
	end
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

function requestThread ()
	local i = count
	count = count + 1
	
	threads [ i ] = MOAIThread.new ()
	
	return threads [ i ]
end