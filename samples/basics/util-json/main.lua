----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

object = {}

object.foo = "str1"
object.bar = 3
object.baz = 123.456

str = MOAIJsonParser.encode ( object )

print ( str )

object = MOAIJsonParser.decode ( str )

for key, value in pairs ( object ) do
	print ( key .. " = " .. value )
end
