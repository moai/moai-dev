
function stage ()
	print ( 'staging threadTest' )
end

function threadTest ()
	MOAITestMgr.beginTest ( 'Thread Test' )
	MOAITestMgr.comment ( 'This is only a test.' )
	MOAITestMgr.endTest ( true )
end

function test ()
	thread = MOAIThread.new ()
	thread:run ( threadTest )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.SAMPLE )
