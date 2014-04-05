----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAISim.forceGarbageCollection ()
MOAISim.setHistogramEnabled ( true )

prop1 = MOAIProp2D.new ()
prop1.me = prop

prop2 = MOAIProp2D.new ()
prop2.me = function () prop2:something () end

prop1.prop2 = prop2
prop2.prop1 = prop1

prop1 = nil
prop2 = nil

print ( "--------------------------------" )
MOAISim.reportHistogram ()

print ( "--------------------------------" )
MOAISim.forceGarbageCollection ()
MOAISim.reportHistogram ()	



