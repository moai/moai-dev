----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------


stream = MOAIMemStream.new ()
stream:open ()
	
i = 0

while 100 > i do 	
	stream:writeFloat ( i )
	i = i + 1
end

stream:flush ()

stream:seek ()

stream:seek ( 240 )
print ( stream:readFloat ())
print ( stream:getCursor ())

stream:seek ( -4, MOAIMemStream.SEEK_CUR ) 
print ( stream:readFloat ())
print ( stream:getCursor ())

