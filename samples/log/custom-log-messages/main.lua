----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_FileNotFound_S,			'CUSTOM %s NOT FOUND MESSAGE', MOAILogMgr.LOG_ERROR )
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_ParamTypeMismatch,		'CUSTOM PARAM TYPE MISMATCH MESSAGE', MOAILogMgr.LOG_ERROR )
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_IndexOutOfRange_DDD,	'CUSTOM INDEX %d OUT OF RANGE [%d, %d] MESSAGE', MOAILogMgr.LOG_ERROR )
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAINode_AttributeNotFound,	'CUSTOM ATTRIBUTE NOT FOUND MESSAGE', MOAILogMgr.LOG_ERROR )

-- type mismatch
gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setRect ( 1, 2, 'something' )

-- missing file
texture = MOAITexture.new ()
texture:load ( 'foo.png' )

-- create an object to test the out of range message
brushLib = MOAIGfxQuadDeck2D.new ()
brushLib:reserve ( 2 )
brushLib:setRect ( 0, 0, 0, 0, 0 ) -- error
brushLib:setRect ( 1, 0, 0, 0, 0 )
brushLib:setRect ( 2, 0, 0, 0, 0 )
brushLib:setRect ( 3, 0, 0, 0, 0 ) -- error

-- attributes
transform = MOAITransform.new ()
transform:getAttr ( 76 )

-- disable attribute not found message
print ( 'disabling attribute not found message...' )
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAINode_AttributeNotFound )
transform:getAttr ( 76 )

