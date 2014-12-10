----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

data = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce ultricies porttitor leo.'

data = MOAIDataBuffer.base64Encode ( data )
print ( data )

data = MOAIDataBuffer.base64Decode ( data )
print ( data )

