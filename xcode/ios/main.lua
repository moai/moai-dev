----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'HELLO FROM LUA' )

if MOAIFileSystem.checkFileExists ( 'lua/main.lua' ) then
	MOAIFileSystem.setWorkingDirectory ( 'lua' )
	dofile ( 'main.lua' )
end
