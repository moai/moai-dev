----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------
	
buffer = MOAIDataBuffer.new () 
buffer:load ( 'test.txt' ) 
stream = MOAIDataBufferStream.new () 

stream:open ( buffer )

i = 0
while 100 > i do 	
	stream:write8 ( 10, i + 12, -( i + 14 ) ) 
	i = i + 1
end
	
print ( 'test1' )
stream:close ()
print ( 'test2' )