----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

module ( "test", package.seeall )

local eps = .000001
local pass = 0
local fail = 0

function evaluate ( condition, str )
	print ( ( condition and "PASS" or "FAILED"), str )
	if condition then
		pass = pass + 1
	else
		fail = fail + 1
	end
end

function epsilon ( val1, val2 )
	return math.abs ( val1 - val2 ) <= eps
end

local threads = {}
local index = 0
function requestThread ()
	local i = index
	index = index + 1
	
	threads [ i ] = MOAIThread.new ()
	
	return threads [ i ]
end

function printResults ()
	print ( "\n" .. pass .. " tests passed, " .. fail .. " tests failed." )
end
