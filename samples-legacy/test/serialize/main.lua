----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAITransform.extend (

	"MOAIFoo",

	-- extend the instance interface
	function ( interface, super )
		print ( "extend interface" )
	end,
	
	-- extend the class
	function ( class, super )
		print ( "extend class" )
	end
)

print ( MOAIFoo )

transform = MOAITransform.new ()
print ( transform )
print ( transform:getClassName ())
print ( transform:getClass ())

foo = MOAIFoo.new ()
print ( foo )
print ( foo:getClassName ())
print ( foo:getClass ())

print ( '\nSERIALIZING\n' )

foo.test1 = 123
foo.test2 = "one two three"

serializer = MOAISerializer.new ()
serializer:serialize ( foo )
testStr = serializer:exportToString ()

file = io.open ( 'out.lua', 'w' )
file:write ( testStr )
file:close ()

-- time to deserialize the objects
foo = dofile ( 'out.lua' )

print ( '' )

-- verify the test data
print ( "foo test data" )
print ( foo.test1 )
print ( foo.test2 )
print ( '' )

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

memberTable = getmetatable ( foo )
printTable ( "memberTable", memberTable )

refTable = getmetatable ( memberTable )
printTable ( "refTable", refTable )

interfaceTable = getmetatable ( refTable )
printTable ( "interfaceTable", interfaceTable )
