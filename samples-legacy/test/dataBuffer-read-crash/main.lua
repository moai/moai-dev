----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

buffer = MOAIDataBuffer.new ()
buffer:load ( 'test.txt' )
stream = MOAIDataBufferStream.new ()
stream:open ( buffer )

stream:seek ()

stream:write8 ( 10 )
stream:write8 ( 10 )
stream:write8 ( 10 )
stream:write8 ( 10 )
stream:write8 ( 10 )

stream:close ()