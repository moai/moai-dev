----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function test ()
	
	MOAITestMgr.beginTest ( 'MOAIDataBufferStream' )
	
	---------------------------------------------------------------
	
	buffer = MOAIDataBuffer.new () 
    buffer:load ( 'test.txt' ) 
    stream = MOAIDataBufferStream.new () 
	 
    stream:open ( buffer )

	---------------------------------------------------------------------------
	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
