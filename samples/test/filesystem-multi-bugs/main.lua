----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local testall = true
local test = 1

MOAISim.openWindow ( "test", 320, 480 )

----------------------------------------------------------------
-- test = 1
-- If the inputted path includes a .. and no trailing / 
-- (ie. "../test"), then the code where the trailing / is added 
-- (ZLFileSystem.cpp, line 258) fails and so the path is invalid.
-- This causes affirmPath () to not create the directory like 
-- it's supposed to.
-- *Note: "../test" works fine with deleteDirectory () and
-- checkPathExists ().
function affirmPathTest ()
	print ( "\n-------------------------------------------------------------------------------" )
	print ( "affirmPath test...\n" )

	MOAIFileSystem.setWorkingDirectory ( 'test' )
	
	local path = "../affirm"
	
	if MOAIFileSystem.deleteDirectory ( path ) == false then
		print ( "Failed to clear the test folder, it probably contains files. Required pre-conditions for this test are not met." )
		return
	end
	
	print ( "Affirm Path: " .. path )
	MOAIFileSystem.affirmPath ( path )
	
	print ( "Success: " .. tostring ( MOAIFileSystem.checkPathExists ( path )))
	
	MOAIFileSystem.setWorkingDirectory ( '../' )
end

----------------------------------------------------------------
-- test = 2
-- Similar to test 1, if you call getAbsoluteDirectoryPath ()
-- with .. in the input path and no trailing /, then the trailing
-- / is not added properly to the returned value. This is not
-- consistent with calling it without a .. and no trailing /.
-- ie. calling it with "test" will add the trailing / but
-- "../test" will not add it.
function getAbsoluteDirectoryPathTest ()
	print ( "\n-------------------------------------------------------------------------------" )
	print ( "getAbsoluteDirectoryPath test...\n" )

	local path     = "../test"
	local fullpath = MOAIFileSystem.getAbsoluteDirectoryPath ( path )
	local pathlen  = string.len ( fullpath )
	local lastchar = string.sub ( fullpath, pathlen, pathlen )
	
	print ( "Absolute directory path to: " .. path )
	print ( fullpath )
	print ( "Success: " .. tostring ( lastchar == "/" ))
end

----------------------------------------------------------------
-- test = 3
-- listDirectories () returns a table with the proper number of
-- elements, but it fails to append the directory names to each
-- element.
function listDirectoriesTest ()
	print ( "\n-------------------------------------------------------------------------------" )
	print ( "listDirectories test...\n" )
	
	local dirs = MOAIFileSystem.listDirectories ( "test" )
	
	print ( "Listing directories in directory: test" )
	print ( dirs )
	print ( dirs [ 1 ], dirs [ 2 ], dirs [ 3 ] )
end

----------------------------------------------------------------
-- test = 4
-- Similar to test 3, listFiles () fails to append the file
-- name to each element in the table returned.
function listFilesTest ()
	print ( "\n-------------------------------------------------------------------------------" )
	print ( "listFiles test...\n" )
	
	local files = MOAIFileSystem.listFiles ( "test" )
	
	print ( "Listing files in directory: test" )
	print ( files )
	print ( files [ 1 ], files [ 2 ], files [ 3 ] )
end

----------------------------------------------------------------
if test == 1 or testall then
	affirmPathTest ()
end
if test == 2 or testall then
	getAbsoluteDirectoryPathTest ()
end
if test == 3 or testall then
	listDirectoriesTest ()
end
if test == 4 or testall then
	listFilesTest ()
end