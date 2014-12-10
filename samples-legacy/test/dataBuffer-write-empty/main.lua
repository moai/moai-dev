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

while 100 > i do 	-- Testing 8 bit read and write capabilities 
	stream:write8 ( 10, 12, -14 )
	if stream:getLength () ~= i * 3 + 3 then
		break
	end 
	
	i = i + 1
end

stream:flush ()

stream:seek ( 35 )	
print ( stream:read8 ())