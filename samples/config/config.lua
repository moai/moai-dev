----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAILogMgr.setLogLevel ( MOAILogMgr.LOG_STATUS )

MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_FileNotFound_S, 'File not found: %s' )
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_IndexNoReserved, 'Nothing reserved' )
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_IndexOutOfRange_DDD, 'Index %d is out of acceptable range [%d, %d)' )
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_NewIsUnsupported, 'Method \'new\' is unsupported. Instances of this class are created by the engine or through another interface.' )
MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAI_ParamTypeMismatch, 'Param type mismatch; check function call' )

MOAILogMgr.registerLogMessage ( MOAILogMgr.MOAINode_AttributeNotFound, 'No such attribute' )