----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

function foo ()

	bar ()
end

function bar ()

	baz ()
end

function baz ()

	print ( "foo!" )
	local i = {}
	if i > 0 then
		print ( "yay!" )
	end
end

--moai.debugCall ( foo )

thread = MOAIThread.new ()
thread:run ( foo )

MOAISim.openWindow ( "cathead", 320, 480 )
