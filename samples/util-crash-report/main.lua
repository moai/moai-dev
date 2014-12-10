----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "Slots", 100, 100 )

local CrittercismInit = false

local function newTraceback ( message )
	
	-- get trace
	print ( "calling traceback" )
	local trace = message .. '\n' .. debug.traceback ()

	-- crittercism
	if CrittercismInit then
		print "calling crittercism"
		MOAICrittercism.leaveBreadcrumb ( trace )
	end

	-- print
	print ( trace )
	
	-- force ze crash
	if CrittercismInit and MOAICrittercism.forceException then
		MOAICrittercism.forceException ()
	end
	
	print ( "done custom traceback" )
end

if MOAICrittercism then
	--CrittercismInit = true
	--MOAICrittercism.init ( "appId", "appKey", "appSecret" )
end
	
MOAISim.setTraceback ( newTraceback )

function test1()
	local test = nil
	test.fail ()
end

function test2()
	test1()
end

function test3()
	test2()
end

function test4()
	test3()
end

function test5()
	test4()
end

local thread = MOAIThread.new ()
thread:run ( test5 )
