print ( 'HELLO FROM LUA!' )

local testFunc = function ( p )

	print ( p )

	local t = {
		a = 'a',
		b = 'b',
		c = 'c',
	}

	print ( t )
	print ( t.a, t.b, t.c )
end

testFunc ( 'call 1' )
testFunc ( 'call 2' )
