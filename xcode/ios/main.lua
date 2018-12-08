----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'HELLO FROM LUA' )

local SAMPLE_NAME = 'debugLines-partition'
local SAMPLE_PATH = 'lua/' .. SAMPLE_NAME

print ( SAMPLE_PATH )

if MOAIFileSystem.checkFileExists ( SAMPLE_PATH .. '/main.lua' ) then
    print ( 'RUNNING SAMPLE' )
	MOAIFileSystem.setWorkingDirectory ( SAMPLE_PATH )
	dofile ( 'main.lua' )
end
