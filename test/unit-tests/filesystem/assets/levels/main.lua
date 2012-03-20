----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

workingDir = MOAIFileSystem.getWorkingDirectory () 
MOAIFileSystem.setWorkingDirectory ( ".." )
require "testhelpers"
MOAIFileSystem.setWorkingDirectory ( workingDir )

-- listFiles

files = MOAIFileSystem.listFiles ( "assets/levels" )
test.printTable ( files )
