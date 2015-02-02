----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

func1 = function ()
	foo.bar = baz
end

func2 = function ()
	func1 ()
end

func3 = function ()
	func2 ()
end

func3 ()

thread = MOAICoroutine.new ()
thread:run ( function () func3 () end )