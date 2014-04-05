function stage ()
	print ( 'staging' )
end

function test ()
	MOAITestMgr.beginTest ( 'test2' )
	MOAITestMgr.comment ( 'something is wrong...' )
	MOAITestMgr.endTest ( false )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.SAMPLE )
