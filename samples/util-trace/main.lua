----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAITrace.start ()

print ( 'foo' )
print ( 'bar' )
print ( 'baz' )

local testFunc = function ()

	print ( 'testFunc' )
end

testFunc ()
testFunc ()
testFunc ()

local loop = function ()

	print ( 'co foo' )
	coroutine.yield ()

	print ( 'co bar' )
	coroutine.yield ()

	print ( 'co baz' )
	coroutine.yield ()

end

co = coroutine.create ( loop )
coroutine.resume ( co )
coroutine.resume ( co )
coroutine.resume ( co )

MOAITrace.reportTrace ()
