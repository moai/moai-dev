----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------


function test ()
	
	MOAITestMgr.beginTest ( 'MOAIMemStream' )

	---------------------------------------------------------------------------
	stream = MOAIMemStream.new ()
	stream:open ()
	
	i = 0
	
	while 100 > i do 	
		stream:writeFloat ( i )
		i = i + 1
	end
	
	stream:flush ()
	
	stream:seek ( 240 )
	print ( stream:readFloat ())
	
	stream:seek ( -4 ) 
	print ( stream:readFloat ())

	
	---------------------------------------------------------------------------
	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
