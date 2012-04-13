----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

workingDir = MOAIFileSystem.getWorkingDirectory () 
MOAIFileSystem.setWorkingDirectory ( ".." )
require "testhelpers"
MOAIFileSystem.setWorkingDirectory ( workingDir )
MOAISim.openWindow ( "test", 512, 512 )

-- affirmPath
local function affirmPathTest ( path )
	MOAIFileSystem.affirmPath ( path )
	test.evaluate ( MOAIFileSystem.checkPathExists ( path ),
		"Affirm path: " .. path )
end

-- checkFileExists
local function checkFileExistsTest ( path, exists )
	test.evaluate ( MOAIFileSystem.checkFileExists ( path ) == exists,
		"Check if file exists: " .. path )
end

-- checkPathExists
local function checkPathExistsTest ( path, exists )
	test.evaluate ( MOAIFileSystem.checkPathExists ( path ) == exists,
		"Check if path exists: " .. path )
end

-- deleteDirectory
local function deleteDirectoryTest ( path, valid )
	MOAIFileSystem.deleteDirectory ( path )
	test.evaluate ( MOAIFileSystem.checkPathExists ( path ) ~= valid,
		"Delete directory: " .. path )
end

-- deleteFile
local function deleteFileTest ( path )
	MOAIFileSystem.deleteFile ( path )
	test.evaluate ( not MOAIFileSystem.checkFileExists ( path ),
		"Delete file: " .. path )
end

-- getAbsoluteDirectoryPath
local function getAbsoluteDirectoryPathTest ( path, output )
	test.evaluate ( MOAIFileSystem.getAbsoluteDirectoryPath ( path ) == output,
		"Getting absolute directory path of path: " .. path )
end

-- getAbsoluteFilePath
local function getAbsoluteFilePathTest ( path, output )
	test.evaluate ( MOAIFileSystem.getAbsoluteFilePath ( path ) == output,
		"Getting absolute file path of path: " .. path )
end

-- getRelativePath
local function getRelativePathTest ( path, output )
	test.evaluate ( MOAIFileSystem.getRelativePath ( path ) == output,
		"Getting relative path of path: " .. path )
end

-- getWorkingDirectory
-- setWorkingDirectory
local function getSetWorkingDirectoryTest ()
	before = MOAIFileSystem.getWorkingDirectory ()
	
	MOAIFileSystem.setWorkingDirectory ( "assets" )
	test.evaluate ( MOAIFileSystem.getWorkingDirectory () == before .. "assets/",
		"Get/set working directory assets" )
	
	MOAIFileSystem.setWorkingDirectory ( "levels" )
	test.evaluate ( MOAIFileSystem.getWorkingDirectory () == before .. "assets/levels/",
		"Get/set working directory assets/levels" )
	
	MOAIFileSystem.setWorkingDirectory ( "../../" )
	test.evaluate ( MOAIFileSystem.getWorkingDirectory () == before,
		"Get/set working directory (root)" )
	
	MOAIFileSystem.setWorkingDirectory ( before )
end

-- listDirectories
local function listDirectoriesTest ()
	dirs = MOAIFileSystem.listDirectories ( "assets/levels" )
	test.evaluate ( dirs [ 1 ] == nil,
					"List directories in directory assets/levels" )
	
	dirs = MOAIFileSystem.listDirectories ( "assets" )
	test.evaluate ( dirs [ 1 ] == "assets/levels",
					"List directories in directory assets" )
end

-- listFiles
local function listFilesTest ()
	files = MOAIFileSystem.listFiles ( "assets/levels" )
	
	test.evaluate ( files [ 1 ] == "assets/levels/main.lua" and
					files [ 2 ] == "assets/levels/run.bat" and
					files [ 3 ] == "assets/levels/testhelpers.lua",
					"List files in directory assets/levels" )
	
	files = MOAIFileSystem.listFiles ( "assets" )
	test.evaluate ( files [ 1 ] == nil,
					"List files in directory assets" )
end

-- mountVirtualDirectory
local function mountVirtualDirectoryTest ( path, archive, output )
	if archive then
		test.evaluate ( MOAIFileSystem.mountVirtualDirectory ( path, archive ) == output,
			"Mounting virtual directory with path " .. path ", archive " .. archive )
	else
		test.evaluate ( MOAIFileSystem.mountVirtualDirectory ( path ) == output,
			"Mounting virtual directory with path " .. path )
	end
end

-- rename
local function renameTest ( old, new, result )
	test.evaluate ( MOAIFileSystem.rename ( old, new ) == result,
		"Renaming file " .. old .. " to " .. new )
end

local function createFile ( name )
	file = io.open ( name, "w" )
	file:close ()
end

-- tests

zipfs  = false
runall = true

if zipfs then
	MOAIFileSystem.mountVirtualDirectory ( "testzip", "test.zip" )
	checkPathExistsTest ( "testzip/assets/", true )
	checkFileExistsTest ( "testzip/assets/levels/main.lua", true )
	checkPathExistsTest ( "testzip/assets", true )
	checkPathExistsTest ( "testzip/assetss/", false )
	checkFileExistsTest ( "testzip/assetss/levels/main.lua", false )
end

if runall then
	-- affirmPath
	affirmPathTest ( "assets" )
	affirmPathTest ( "test1/" )
	affirmPathTest ( "../filesystem" )
	affirmPathTest ( "../../../../" )
	affirmPathTest ( "../.." )
	
	-- checkFileExists
	createFile ( "test2.txt" )
	checkFileExistsTest ( "test2.txt", true )
	checkFileExistsTest ( "test3.txt", false )
	checkFileExistsTest ( "assets/../test2.txt", true )
	renameTest ( "test2.txt", "test3.txt", true )
	checkFileExistsTest ( "test2.txt", false )
	checkFileExistsTest ( "test3.txt", true )
	checkFileExistsTest ( "assets/levels/main.lua", true )
	deleteFileTest ( "test3.txt" )
	
	-- checkPathExists
	checkPathExistsTest ( "test1", true )
	checkPathExistsTest ( "test1/", true )
	deleteDirectoryTest ( "test1", true )
	checkPathExistsTest ( "test1", false )
	checkPathExistsTest ( "assets/levels", true )
	checkPathExistsTest ( "assets/assets/../levels", true )
	
	-- deleteDirectory
	affirmPathTest ( "../test" )
	deleteDirectoryTest ( "../test", true )
	affirmPathTest ( "assets/levels/art" )
	deleteDirectoryTest ( "assets/levels/art", true )
	affirmPathTest ( "assets/data" )
	createFile ( "assets/data/test1.txt" )
	createFile ( "assets/data/test2.txt" )
	deleteDirectoryTest ( "assets/data", false )
	
	-- deleteFile
	deleteFileTest ( "assets/data/test2.txt" )
	deleteFileTest ( "assets/../assets/../assets/data/test1.txt" )
	deleteDirectoryTest ( "assets/data", true )
	
	-- getAbsoluteDirectoryPath
	dir = MOAIFileSystem.getWorkingDirectory ()
	getAbsoluteDirectoryPathTest ( "assets/levels",
		dir .. "assets/levels/" )
	getAbsoluteDirectoryPathTest ( "../transform",
		string.sub ( dir, 0, -12 ) .. "transform/" )
	getAbsoluteDirectoryPathTest ( "../transform/",
		string.sub ( dir, 0, -12 ) .. "transform/" )
	
	-- getAbsoluteFilePath
	getAbsoluteFilePathTest ( "assets/levels/main.lua",
		dir .. "assets/levels/main.lua" )
	getAbsoluteFilePathTest ( "../transform/main.lua",
		string.sub ( dir, 0, -12 ) .. "transform/main.lua" )
	
	-- getRelativePath
	getRelativePathTest ( "main.lua", "./main.lua" )
	getRelativePathTest ( "../transform/main.lua", "../transform/main.lua" )
	
	-- getWorkingDirectory
	-- setWorkingDirectory
	getSetWorkingDirectoryTest ()
	
	-- listDirectories
	listDirectoriesTest ()
	
	-- listFiles
	listFilesTest ()
	
	-- mountVirtualDirectory
	mountVirtualDirectoryTest ( "assets", nil, true )
	mountVirtualDirectoryTest ( "assets/levels", nil, true )
	
	-- rename
	renameTest ( "assets", "test", true )
	renameTest ( "assets", "test", false )
	renameTest ( "test", "assets", true )
	affirmPathTest ( "assets/data" )
	renameTest ( "assets/data", "assets/levels", false )
	deleteDirectoryTest ( "assets/data" )
	
end

local results = MOAIThread.new ()
results:run ( test.printResults )
