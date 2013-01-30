--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

print ( "hello!" )

require "apsalar"

function progress ( record ) 

	print ( "progress!" )
end

function failure ( record, response ) 

	print ( "failure!" ) 
end

function finish () 

	print ( "finish!" ) 
end

apsalar.start ( "moaisample", "wizbSwNt" ) -- plug in your own account values to see the results on apsalar.com

apsalar.event ( "Application Start" )
apsalar.event ( "Event With Data Attached", { name = "foo", stuff = "bar", baz = 1 })

local queue = apsalar.getQueue ()
queue:setProgressCallback ( progress )
queue:setFailureCallback ( failure )
queue:setFinishCallback ( finish )

queue:send ( 1 )	-- send a certain number
-- queue:send ()	-- send entire queue
