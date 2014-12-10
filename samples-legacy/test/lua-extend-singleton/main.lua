----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end

function printTable ( name, t )

	printf ( '--------------------------------\n' )
	printf ( '%s: %s\n', name, tostring ( t ))
	for k, v in pairs ( t ) do
		printf ( '  %s: %s\n', tostring ( k ), tostring ( v ))
	end
	printf ( '\n' )
end

print ( MOAISim.extend )

MOAISim.extend (

	'MOAIFoo',
	
	-- extend the class
	function ( class, super )
		print ( "extend class" )
	end
)

print ( MOAISim )
print ( MOAISim.getClassName ())
classTable = getmetatable ( MOAISim )
printTable ( "classTable", classTable )

print ( MOAIFoo )
print ( MOAIFoo.getClassName ())
classTable = getmetatable ( MOAIFoo )
printTable ( "classTable", classTable )
