
function stage ()
	print ( 'staging' )
end

function test ()
	print ( "intest" )
	MOAITestMgr.beginTest ( 'test1' )
	MOAITestMgr.comment ( 'this is a comment' )
	MOAITestMgr.comment ( 'things are going well' )
	MOAITestMgr.endTest ( true )
end

print "ttttttttest1"
MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.SAMPLE )
