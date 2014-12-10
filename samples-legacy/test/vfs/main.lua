----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'foo' )
MOAIFileSystem.mountVirtualDirectory ( 'vfs-test', 'vfs-test.zip' )
print ( 'Checking if file exists: ' .. tostring ( MOAIFileSystem.checkFileExists ( 'vfs-test/main.lua' )))
MOAIFileSystem.setWorkingDirectory ( 'vfs-test' )
dofile ( 'main.lua' )
