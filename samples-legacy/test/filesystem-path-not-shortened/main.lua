----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

dir = MOAIFileSystem.getWorkingDirectory ()

print ( MOAIFileSystem.getAbsoluteDirectoryPath ( '..' ))
print ()

MOAIFileSystem.setWorkingDirectory ( '..' )

print ( MOAIFileSystem.getWorkingDirectory ())
print ()

-- Not sure if this is a bug or not, if I pass an absolute path
-- to getAbsoluteDirectoryPath it also doesn't shorten the path,
-- however it also doesn't appear to just tack the full absolute
-- path onto the working directory either. The absolute path it
-- gives back looks fine if you pass it an absolute path without
-- a .. in it.

print ( MOAIFileSystem.getAbsoluteDirectoryPath ( dir .. '../../' ))
print ()
print ( MOAIFileSystem.getAbsoluteDirectoryPath ( dir ))