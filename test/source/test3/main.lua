function stage ()
	print ( 'staging' )
end

function test ()
	MOAITestMgr.beginTest ( 'test3' )
	MOAITestMgr.comment ( 'everything seems OK' )
	MOAITestMgr.endTest ( true )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.SAMPLE )
