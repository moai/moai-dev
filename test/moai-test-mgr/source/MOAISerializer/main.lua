----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function evaluate ( pass, str )
	if not pass then
		MOAITestMgr.comment ( "FAILED\t" .. str )
		success = false
	end
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAISerializer' )	
	
	testTable = {}
	
	testTable [ 'a' ] = 55
	testTable [ 'b' ] = "hello"
	testTable [ 'c' ] = true
	testTable [ 'd' ] = 2.55
	
	serializer = MOAISerializer.new ()
	
	serializer:serialize ( testTable )
	
	serializer:exportToFile ( 'output/testTable.txt' )
	
	MOAISerializer.serializeToFile ( 'output/testSerialize.txt', testTable )
	
	str1 = serializer:exportToString ()
	local file = io.open ( "serializer1.txt", "w" )
	file:write( str1 )
	file:close ()
	
	str2 = MOAISerializer.serializeToString ( testTable )
	local file = io.open ( "serializer2.txt", "w" )
	file:write ( str2 )
	file:close ()
end



function test ()
	MOAITestMgr.beginTest ( 'MOAISerializer' )
	success = true
	
	---------------------------------------------------------------------------
	testTable = {}
	
	testTable [ 'a' ] = 55
	testTable [ 'b' ] = "hello"
	testTable [ 'c' ] = true
	testTable [ 'd' ] = 2.55
	
	serializer = MOAISerializer.new ()
	
	serializer:serialize ( testTable )
	
	serializer:exportToFile ( 'output/testTable.txt' )
	
	MOAISerializer.serializeToFile ( 'output/testSerialize.txt', testTable )
	
	local file = io.open( "serializer1.txt", "r") 	 
	while true do
		local line = file:read("*l")
			if line == nil then 
				break 
			end
		print ( line )
	end
	
	local file = io.open( "serializer2.txt", "r") 	 
	while true do
		local line = file:read("*l")
			if line == nil then 
				break 
			end
		print ( line )
	end
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.endTest ( success )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
