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
	MOAITestMgr.comment ( 'staging MOAIFooMgr' )
end



function test ()
	
	MOAITestMgr.beginTest ( 'MOAIFooMgr' )
	success = true
	
	---------------------------------------------------------------------------
	
	MOAIFooMgr.singletonHello ()
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.endTest ( success )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
