----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'HELLO FROM LUA' )

if MOAIFileSystem.checkFileExists ( 'lua/main.lua' ) then
	MOAIFileSystem.setWorkingDirectory ( 'lua' )
	dofile ( 'main-ar.lua' )
end
