
function stage ()
	print ( 'staging' )
end

function test ()
	MOAITestMgr.beginTest ( 'test1' )
	MOAITestMgr.comment ( 'this is a comment' )
	MOAITestMgr.comment ( 'things are going well' )
	MOAITestMgr.endTest ( true )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.SAMPLE )
