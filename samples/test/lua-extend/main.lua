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

prop = MOAIProp2D.new ()
prop.test1 = 123
prop.test2 = "foo bar baz"

print ( prop )
print ( prop.getBounds )
print ( prop.test1 )
print ( prop.test2 )
print ( '' )

refTable = getmetatable ( prop )
printTable ( "refTable", refTable )

memberTable = getmetatable ( refTable )
printTable ( "memberTable", memberTable )

interfaceTable = getmetatable ( memberTable )
printTable ( "interfaceTable", interfaceTable )

prop = MOAIProp.new ()
prop.anotherTest1 = 456
prop.anotherTest2 = "one two three"

print ( prop )
print ( prop.getBounds )
print ( prop.anotherTest1 )
print ( prop.anotherTest2 )
print ( '' )

refTable = getmetatable ( prop )
printTable ( "refTable", refTable )

memberTable = getmetatable ( refTable )
printTable ( "memberTable", memberTable )

interfaceTable = getmetatable ( memberTable )
printTable ( "interfaceTable", interfaceTable )