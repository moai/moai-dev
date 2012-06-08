----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- abspath = MOAIFileSystem.getAbsoluteDirectoryPath ( 'test/blue' )
-- print ( abspath )
-- print ()

-- relpath = MOAIFileSystem.getRelativePath ( abspath )
-- print ( relpath )
-- print ()

-- abspath = MOAIFileSystem.getAbsoluteFilePath ( 'test/blue/dummy.txt' )
-- print ( abspath )
-- print ()

-- relpath = MOAIFileSystem.getRelativePath ( abspath )
-- print ( relpath )
-- print ()

dir = MOAIFileSystem.getWorkingDirectory ()

relpath = MOAIFileSystem.getRelativePath ( dir .. 'test/../test/' )
print ( relpath )
print ()