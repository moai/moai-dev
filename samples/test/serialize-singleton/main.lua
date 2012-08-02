----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.extend (

	"MOAIFoo",
	
	-- extend the class
	function ( class, super )
		print ( "extend class" )
	end
)

print ( MOAISim.getClassName ())
print ( MOAIFoo.getClassName ())

print ( '\nSERIALIZING\n' )

serializer = MOAISerializer.new ()
serializer:serialize ( MOAISim )
serializer:serialize ( MOAIFoo )
testStr = serializer:exportToString ()

file = io.open ( 'out.lua', 'w' )
file:write ( testStr )
file:close ()

-- time to deserialize the objects
dofile ( 'out.lua' )


