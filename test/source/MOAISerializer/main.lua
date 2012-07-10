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
	
	serializer:exportToFile ( 'testTable.txt' )
	
	print ( serializer:exportToString ())
	
	MOAISerializer.serializeToFile ( 'testSerialize.txt', testTable )
	
	print ( MOAISerializer.serializeToString ( testTable ))
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.endTest ( success )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
