stagingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'staging' )
testingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'testing' )
rootDir = MOAIFileSystem.getWorkingDirectory ()

results = rootDir .. '.results'
allResults = rootDir .. 'test-results'

MOAIFileSystem.affirmPath ( stagingDir )
MOAIFileSystem.affirmPath ( testingDir )

stagingList = MOAIFileSystem.listDirectories ( stagingDir )

command = string.format ( '%%MOAI_BIN%%\\moai-test -r "%s" main.lua', results )

MOAIFileSystem.deleteFile ( allResults )

for i, dirname in ipairs ( stagingList ) do
	
	local from = stagingDir .. dirname
	local to = testingDir .. dirname

	if MOAIFileSystem.checkPathExists ( to ) == true then
		MOAIFileSystem.deleteDirectory ( to )
	end
	
	MOAIFileSystem.copy ( from, to )
	
	MOAIFileSystem.setWorkingDirectory ( to )
	os.execute ( command )
	MOAIFileSystem.setWorkingDirectory ( rootDir )
	
	if MOAIFileSystem.checkFileExists ( results ) then
		
		inFile = MOAIFileStream.new ()
		outFile = MOAIFileStream.new ()
		
		inFile:open ( results, MOAIFileStream.READ )
		outFile:open ( allResults, MOAIFileStream.READ_WRITE_AFFIRM )
		
		outFile:seek ( 0, MOAIFileStream.SEEK_END )
		outFile:writeStream ( inFile )
		
		inFile:close ()
		outFile:close ()
	end
end

MOAIFileSystem.deleteFile ( results )
