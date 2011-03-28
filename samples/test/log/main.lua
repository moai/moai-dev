print ( "hello, moai!" )

-- sign up to receive all log levels
MOAILogMgr.setLogLevel ( MOAILogMgr.LOG_STATUS )

-- register a log message
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_IndexOutOfRange_DDD, 'Index %d is out of acceptable range [%d, %d)' )

-- create an object to test the out of range message
brushLib = MOAIGfxQuadDeck2D.new ()
brushLib:reserveBrushes ( 2 )
brushLib:setRect ( 0, 0, 0, 0, 0 ) -- error
brushLib:setRect ( 1, 0, 0, 0, 0 )
brushLib:setRect ( 2, 0, 0, 0, 0 )
brushLib:setRect ( 3, 0, 0, 0, 0 ) -- error