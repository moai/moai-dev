----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

scope = MOAIScope.new ()

function printScope ( scope )
	print ( 'SCOPE: {' )
	for k, v in pairs ( scope:getMemberTable ()) do
		print ( '', k, v )
	end
	print ( '}' )
	print ( 'retained:', scope:countRetained ())
	print ()
end

scope [ 1 ] 	= 'one'
scope [ 2 ] 	= 'two'
scope [ 3 ] 	= 'three'
scope.foo 		= 'foo'
scope.bar 		= 'bar'
scope.baz		= MOAIAnimCurve.new ()

printScope ( scope )

scope [ 2 ] 	= nil
scope.bar 		= nil
scope.baz		= MOAIAction.new ()

printScope ( scope )

scope.baz		= nil

printScope ( scope )

scope.baz		= MOAIAnimCurve.new ()

printScope ( scope )

scope:purge ()

printScope ( scope )
