----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function evaluate ( pass, str )
	if not pass then
		MOAITestMgr.comment ( "FAILED\t" .. str )
		success = false
	end
end

local function createFile ( name )
	file = io.open ( name, "w" )
	file:close ()
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAIFileSystem' )
end

function test ()
	MOAIFileSystem.deleteDirectory ( 'tests', true )
	MOAIFileSystem.affirmPath ( 'tests' )
	MOAIFileSystem.copy ( 'test.zip', 'tests/test.zip' )
	MOAIFileSystem.setWorkingDirectory ( 'tests' )

	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.affirmPath' )
	success = true
	
	MOAIFileSystem.affirmPath ( 'test1' )
	evaluate ( MOAIFileSystem.checkPathExists ( 'test1' ),
		"1 \taffirmPath ( 'test1' )" )
	
	MOAIFileSystem.affirmPath ( 'test2/' )
	evaluate ( MOAIFileSystem.checkPathExists ( 'test2/' ),
		"2 \taffirmPath ( 'test2/' )" )
	
	MOAIFileSystem.affirmPath ( '../test3' )
	evaluate ( MOAIFileSystem.checkPathExists ( '../test3' ),
		"3 \taffirmPath ( '../test3' )" )
	
	MOAIFileSystem.affirmPath ( '../../../../' )
	evaluate ( MOAIFileSystem.checkPathExists ( '../../../../' ),
		"4 \taffirmPath ( '../../../../' )" )
	
	MOAIFileSystem.affirmPath ( '../..' )
	evaluate ( MOAIFileSystem.checkPathExists ( '../..' ),
		"5 \taffirmPath ( '../..' )" )
	
	MOAIFileSystem.affirmPath ( './.' )
	evaluate ( MOAIFileSystem.checkPathExists ( './.' ),
		"6 \taffirmPath ( './.' )" )
	
	MOAIFileSystem.affirmPath ( './' )
	evaluate ( MOAIFileSystem.checkPathExists ( './' ),
		"7 \taffirmPath ( './' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.checkFileExists' )
	success = true
	
	createFile ( 'test1.txt' )
	evaluate ( MOAIFileSystem.checkFileExists ( 'test1.txt' ) == true,
		"1 \tcheckFileExists ( 'test1.txt' )" )
		
	evaluate ( MOAIFileSystem.checkFileExists ( './test1.txt' ) == true,
		"2 \tcheckFileExists ( './test1.txt' )" )
	
	evaluate ( MOAIFileSystem.checkFileExists ( 'test2.txt' ) == false,
		"3 \tcheckFileExists ( 'test2.txt' )" )
	
	evaluate ( MOAIFileSystem.checkFileExists ( 'test/../test1.txt' ) == true,
		"4 \tcheckFileExists ( 'test/../test1.txt' )" )
		
	MOAIFileSystem.rename ( 'test1.txt', 'test2.txt' )
	
	evaluate ( MOAIFileSystem.checkFileExists ( 'test1.txt' ) == false,
		"5 \tcheckFileExists ( 'test1.txt' )" )
	
	evaluate ( MOAIFileSystem.checkFileExists ( 'test2.txt' ) == true,
		"6 \tcheckFileExists ( 'test2.txt' )" )
		
	MOAIFileSystem.rename ( 'test2.txt', 'test1.txt' )
	
	evaluate ( MOAIFileSystem.checkFileExists ( 'test1' ) == false,
		"7 \tcheckFileExists ( 'test1' )" )
	
	evaluate ( MOAIFileSystem.checkFileExists ( 'test1/test2/../test2/test1.txt' ) == false,
		"8 \tcheckFileExists ( 'test1/test2/../test2/test1.txt' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.checkPathExists' )
	success = true
	
	evaluate ( MOAIFileSystem.checkPathExists ( 'test1' ) == true,
		"1 \tcheckPathExists ( 'test1' )" )
	
	evaluate ( MOAIFileSystem.checkPathExists ( 'test2/' ) == true,
		"2 \tcheckPathExists ( 'test2/' )" )
	
	evaluate ( MOAIFileSystem.checkPathExists ( '../tests' ) == true,
		"3 \tcheckPathExists ( '../tests' )" )
		
	MOAIFileSystem.deleteDirectory ( 'test1', true )
	
	evaluate ( MOAIFileSystem.checkPathExists ( 'test1' ) == false,
		"4 \tcheckPathExists ( 'test1' )" )
	
	evaluate ( MOAIFileSystem.checkPathExists ( './test2/.' ) == true,
		"5 \tcheckPathExists ( './test2/.' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	MOAITestMgr.beginTest ( 'MOAIFileSystem.copy' )
	success = true
	
	evaluate ( MOAIFileSystem.copy ( './test1.txt', 'test2.txt' ) == true,
		"1 \tMOAIFileSystem.copy ( './test1.txt', 'test2.txt' )" )
		
	evaluate ( MOAIFileSystem.copy ( 'test1.txt', 'test2' ) == false,
		"2 \tMOAIFileSystem.copy ( 'test1.txt', 'test2' )" )
	
	evaluate ( MOAIFileSystem.copy ( '../test3', 'test22/test23/' ) == true,
		"3 \tMOAIFileSystem.copy ( '../test3', 'test22/test23/' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.deleteDirectory' )
	success = true
	
	MOAIFileSystem.affirmPath ( 'test10' )
	MOAIFileSystem.affirmPath ( 'test11' )
	MOAIFileSystem.affirmPath ( '../test12' )
	MOAIFileSystem.affirmPath ( 'test13/test14/test15' )
	createFile ( 'test13/test14/test2.txt' )
	
	evaluate ( MOAIFileSystem.deleteDirectory ( './test10' ) == true,
		"1 \tMOAIFileSystem.deleteDirectory ( './test10' )" )
	
	evaluate ( MOAIFileSystem.deleteDirectory ( 'test11/.' ) == true,
		"2 \tMOAIFileSystem.deleteDirectory ( 'test11/.' )" )
		
	evaluate ( MOAIFileSystem.deleteDirectory ( '../test12' ) == true,
		"3 \tMOAIFileSystem.deleteDirectory ( '../test12' )" )
	
	evaluate ( MOAIFileSystem.deleteDirectory ( 'test13/test14/test15', false ) == true,
		"4 \tMOAIFileSystem.deleteDirectory ( 'test13/test14/test15', false )" )
	
	evaluate ( MOAIFileSystem.deleteDirectory ( 'test13/test14', false ) == false,
		"5 \tMOAIFileSystem.deleteDirectory ( 'test13/test14', false )" )
	
	evaluate ( MOAIFileSystem.deleteDirectory ( 'test13', true ) == true,
		"6 \tMOAIFileSystem.deleteDirectory ( 'test13', true )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.deleteFile' )
	success = true
	
	evaluate ( MOAIFileSystem.deleteFile ( './test1.txt' ) == true,
		"1 \tMOAIFileSystem.deleteFile ( './test1.txt' )" )
	
	file = io.open ( 'test2.txt' )
	evaluate ( MOAIFileSystem.deleteFile ( 'test2.txt' ) == false,
		"1 \tMOAIFileSystem.deleteFile ( 'test2.txt' )" )
	io.close ( file )
	
	evaluate ( MOAIFileSystem.deleteFile ( 'test3.txt' ) == true,
		"2 \tMOAIFileSystem.deleteFile ( 'test3.txt' )" )
	
	createFile ( '../test20.png' )
	evaluate ( MOAIFileSystem.deleteFile ( '../test20.png' ) == true,
		"3 \tMOAIFileSystem.deleteFile ( '../test20.png' )" )
	
	MOAIFileSystem.affirmPath ( 'test21' )
	createFile ( 'test21/test22.pdf' )
	evaluate ( MOAIFileSystem.deleteFile ( 'test21/test22.pdf' ) == true,
		"4 \tMOAIFileSystem.deleteFile ( 'test21/test22.pdf' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.getAbsoluteDirectoryPath' )
	success = true
	
	dir = MOAIFileSystem.getWorkingDirectory ()
	
	evaluate ( MOAIFileSystem.getAbsoluteDirectoryPath ( './test' ) == dir .. 'test/',
		"1 \tMOAIFileSystem.getAbsoluteDirectoryPath ( './test' )" )
		
	evaluate ( MOAIFileSystem.getAbsoluteDirectoryPath ( '../tests' ) == dir,
		"2 \tMOAIFileSystem.getAbsoluteDirectoryPath ( '../tests' )" )
	
	evaluate ( MOAIFileSystem.getAbsoluteDirectoryPath ( 'test/test/.' ) == dir .. 'test/test/',
		"3 \tMOAIFileSystem.getAbsoluteDirectoryPath ( 'test/test/.' )" )
		
	evaluate ( MOAIFileSystem.getAbsoluteDirectoryPath ( 'test/..' ) == dir,
		"4 \tMOAIFileSystem.getAbsoluteDirectoryPath ( 'test/..' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.getAbsoluteFilePath' )
	success = true
	
	evaluate ( MOAIFileSystem.getAbsoluteFilePath ( 'test2.txt' ) == dir .. 'test2.txt',
		"1 \tMOAIFileSystem.getAbsoluteFilePath ( 'test2.txt' )" )
		
	evaluate ( MOAIFileSystem.getAbsoluteFilePath ( '../tests/./test2.txt' ) == dir .. 'test2.txt',
		"2 \tMOAIFileSystem.getAbsoluteFilePath ( '../tests/./test2.txt' )" )
	
	evaluate ( MOAIFileSystem.getAbsoluteFilePath ( 'test/test2.txt' ) == dir .. 'test/test2.txt',
		"3 \tMOAIFileSystem.getAbsoluteFilePath ( 'test/test2.txt' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.getRelativePath' )
	success = true
	
	evaluate ( MOAIFileSystem.getRelativePath ( dir .. './test2.txt' ) == 'test2.txt',
		"1 \tMOAIFileSystem.getRelativePath ( dir .. './test2.txt' )" )
	
	evaluate ( MOAIFileSystem.getRelativePath ( dir .. 'test2/' ) == 'test2/',
		"2 \tMOAIFileSystem.getRelativePath ( dir .. 'test2/' )" )
	
	evaluate ( MOAIFileSystem.getRelativePath ( dir .. '../tests/../tests/../../' ) == '../../',
		"3 \tMOAIFileSystem.getRelativePath ( dir .. '../tests/../tests/../../' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.getWorkingDirectory' )
	success = true
	
	before = MOAIFileSystem.getWorkingDirectory ()
	
	evaluate ( string.sub ( before, -6, -1 ) == 'tests/',
		"1 \tMOAIFileSystem.getWorkingDirectory () \t'tests/'" )
	
	MOAIFileSystem.setWorkingDirectory ( '..' )
	bef = MOAIFileSystem.getWorkingDirectory ()
	evaluate ( string.sub ( bef, -15, -1 ) == 'MOAIFileSystem/',
		"2 \tMOAIFileSystem.getWorkingDirectory () \t'MOAIFileSystem/'" )
	
	MOAIFileSystem.setWorkingDirectory ( 'tests/test2.txt' )
	evaluate ( MOAIFileSystem.getWorkingDirectory () == bef,
		"3 \tMOAIFileSystem.getWorkingDirectory () \t'MOAIFileSystem/'" )
	
	MOAIFileSystem.setWorkingDirectory ( before )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.listDirectories' )
	success = true
	
	dirs = MOAIFileSystem.listDirectories ()
	evaluate ( dirs [ 1 ] == 'test2' and dirs [ 2 ] == 'test21' and dirs [ 3 ] == 'test22',
		"1 \tMOAIFileSystem.listDirectories ()" )
		
	dirs = MOAIFileSystem.listDirectories ( './test22' )
	evaluate ( dirs [ 1 ] == 'test23',
		"2 \tMOAIFileSystem.listDirectories ( './test22' )" )
		
	dirs = MOAIFileSystem.listDirectories ( '..' )
	evaluate ( dirs [ 1 ] == 'test3' and dirs [ 2 ] == 'tests',
		"3 \tMOAIFileSystem.listDirectories ( 'test22' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.listFiles' )
	success = true
	
	createFile ( 'test3.txt' )
	files = MOAIFileSystem.listFiles ()
	evaluate ( files [ 1 ] == 'test.zip' and files [ 2 ] == 'test2.txt' and files [ 3 ] == 'test3.txt',
		"1 \tMOAIFileSystem.listFiles ()" )
	
	files = MOAIFileSystem.listFiles ( 'test2/..' )
	evaluate ( files [ 1 ] == 'test.zip' and files [ 2 ] == 'test2.txt' and files [ 3 ] == 'test3.txt',
		"2 \tMOAIFileSystem.listFiles ( 'test2/..' )" )
		
	createFile ( 'test22/test23/test.pdf' )
	createFile ( 'test22/test23/test.png' )
	createFile ( 'test22/test23/test.txt' )
	files = MOAIFileSystem.listFiles ( 'test22/test23/./' )
	evaluate ( files [ 1 ] == 'test.pdf' and files [ 2 ] == 'test.png' and files [ 3 ] == 'test.txt',
		"3 \tMOAIFileSystem.listFiles ( 'test22/test23/./' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.mountVirtualDirectory' )
	success = true
	
	evaluate ( MOAIFileSystem.mountVirtualDirectory ( 'testzip', 'test.zip' ),
		"1 \tMOAIFileSystem.mountVirtualDirectory ( 'testzip', 'test.zip' )" )
	
	evaluate ( MOAIFileSystem.checkFileExists ( 'testzip/./test.pdf' ),
		"2 \tMOAIFileSystem.checkFileExists ( 'testzip/./test.pdf' )" )
	
	evaluate ( MOAIFileSystem.checkPathExists ( 'testzip/test1' ),
		"3 \tMOAIFileSystem.checkPathExists ( 'testzip/test1' )" )
	
	evaluate ( MOAIFileSystem.getAbsoluteFilePath ( 'testzip/test.pdf' ) == dir .. 'testzip/test.pdf',
		"4 \tMOAIFileSystem.getAbsoluteFilePath ( 'testzip/test.pdf' )" )
	
	evaluate ( MOAIFileSystem.getAbsoluteDirectoryPath ( 'testzip/test1' ) == dir .. 'testzip/test1/',
		"5 \tMOAIFileSystem.getAbsoluteDirectoryPath ( 'testzip/test1' )" )
	
	filepath = MOAIFileSystem.getAbsoluteFilePath ( 'testzip/test.pdf' )
	evaluate ( MOAIFileSystem.getRelativePath ( filepath ) == 'testzip/test.pdf',
		"6 \tMOAIFileSystem.getRelativePath ( filepath )" )
	
	dirpath = MOAIFileSystem.getAbsoluteDirectoryPath ( 'testzip/test1' )
	evaluate ( MOAIFileSystem.getRelativePath ( dirpath ) == 'testzip/test1/',
		"7 \tMOAIFileSystem.getRelativePath ( dirpath )" )
		
	evaluate ( MOAIFileSystem.setWorkingDirectory ( 'testzip' ),
		"8 \tMOAIFileSystem.setWorkingDirectory ( 'testzip' )" )
	
	evaluate ( MOAIFileSystem.getWorkingDirectory () == dir .. 'testzip/',
		"9 \tMOAIFileSystem.getWorkingDirectory ()" )
	MOAIFileSystem.setWorkingDirectory ( dir )
	
	dirs = MOAIFileSystem.listDirectories ( 'testzip' )
	evaluate ( dirs [ 2 ] == 'test1/' and dirs [ 1 ] == 'test2/',
		"10 \tMOAIFileSystem.listDirectories ( 'testzip' )" )
	
	files = MOAIFileSystem.listFiles ( 'testzip' )
	evaluate ( files [ 4 ] == 'test.bmp' and files [ 3 ] == 'test.pdf' and files [ 2 ] == 'test.rtf' and files [ 1 ] == 'test.txt',
		"11 \tMOAIFileSystem.listFiles ( 'testzip' )" )
		
	evaluate ( MOAIFileSystem.checkPathExists ( 'testzip' ),
		"12 \tMOAIFileSystem.checkPathExists ( 'testzip' )" )
		
	evaluate ( MOAIFileSystem.checkPathExists ( 'testzip/' ),
		"13 \tMOAIFileSystem.checkPathExists ( 'testzip/' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.rename' )
	success = true
	
	evaluate ( MOAIFileSystem.rename ( './test2.txt', 'test1.txt' ),
		"1 \tMOAIFileSystem.rename ( './test2.txt', 'test1.txt' )" )
	
	evaluate ( MOAIFileSystem.checkFileExists ( 'test2.txt' ) == false,
		"2 \tMOAIFileSystem.checkFileExists ( 'test2.txt' )" )
	
	evaluate ( MOAIFileSystem.rename ( 'test1.txt', 'test3.txt' ) == false,
		"3 \tMOAIFileSystem.rename ( 'test1.txt', 'test3.txt' )" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
	MOAITestMgr.beginTest ( 'MOAIFileSystem.setWorkingDirectory' )
	success = true
	bef = MOAIFileSystem.getWorkingDirectory ()
	
	evaluate ( MOAIFileSystem.setWorkingDirectory ( '../' ),
		"1 \tMOAIFileSystem.setWorkingDirectory ( '../' )" )
	
	evaluate ( MOAIFileSystem.getWorkingDirectory () == MOAIFileSystem.getAbsoluteDirectoryPath ( bef .. '../' ),
		"2 \tMOAIFileSystem.getWorkingDirectory ()" )
	
	evaluate ( MOAIFileSystem.setWorkingDirectory ( './' ),
		"3 \tMOAIFileSystem.setWorkingDirectory ( './' )" )
	
	evaluate ( MOAIFileSystem.getWorkingDirectory () == MOAIFileSystem.getAbsoluteDirectoryPath ( bef .. '../' ),
		"4 \tMOAIFileSystem.getWorkingDirectory ()" )
	
	MOAITestMgr.endTest ( success )
	
	---------------------------------------------------------------------------
	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
