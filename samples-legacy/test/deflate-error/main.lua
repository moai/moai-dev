----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

data = { stupid = true, excellency = 46, pi = 3.14159 }

print ( data )
print ( data.stupid, data.excellency, data.pi )

local serializer = MOAISerializer.new ()
serializer:serialize ( data )

local saveString = serializer:exportToString ()
saveString = string.dump ( loadstring ( saveString, '' ))
saveString = MOAIDataBuffer.base64Encode ( saveString )
saveString = MOAIDataBuffer.deflate ( saveString, 9 )
local saveFile = io.open ( "test.lua", 'wb' )
saveFile:write ( saveString )
saveFile:close ()

print ( "serialized, zipped, encoded.  Decompressing..." )
local loadFile = io.open ( "test.lua", 'rb' )
local loadString = loadFile:read ( '*a' )
loadFile:close ()
loadString = MOAIDataBuffer.inflate ( loadString )
loadString = MOAIDataBuffer.base64Decode ( loadString )
data = loadstring ( loadString ) ()

print ( data )
print ( data.stupid, data.excellency, data.pi )
