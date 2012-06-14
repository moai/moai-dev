dofile ( 'util.lua' )

rootDir = MOAIFileSystem.getWorkingDirectory ()
results = rootDir .. '.results'
allResults = rootDir .. 'testing-results'
MOAIFileSystem.deleteFile ( allResults )

-- run the C++ tests
stagingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'staging-cpp' )
if MOAIFileSystem.checkPathExists ( stagingDir ) == true then

	testingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'testing-cpp' )
	MOAIFileSystem.affirmPath ( testingDir )

	local testList = MOAITestMgr.getTestList ()
	for i, testname in ipairs ( testList ) do
		
		local from = stagingDir .. testname .. '/'
		local to = testingDir .. testname .. '/'
		
		if MOAIFileSystem.checkPathExists ( to ) == true then
			MOAIFileSystem.deleteDirectory ( to )
		end
		
		if MOAIFileSystem.checkPathExists ( from ) == true then
			MOAIFileSystem.copy ( from, to )
			MOAIFileSystem.setWorkingDirectory ( to )
			
			if MOAITestMgr.checkFilter () then
				os.execute ( string.format ( '%%MOAI_BIN%%\\moai-test -r "%s" -t "%s"', results, testname ))
			end
			
			MOAIFileSystem.setWorkingDirectory ( rootDir )
			gatherResults ( results, allResults )
		end
	end
end

-- run the Lua tests
stagingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'staging-lua' )
if MOAIFileSystem.checkPathExists ( stagingDir ) == true then

	testingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'testing-lua' )
	MOAIFileSystem.affirmPath ( testingDir )

	stagingList = MOAIFileSystem.listDirectories ( stagingDir )
	for i, dirname in ipairs ( stagingList ) do
		
		local from = stagingDir .. dirname .. '/'
		local to = testingDir .. dirname .. '/'

		if MOAIFileSystem.checkPathExists ( to ) == true then
			MOAIFileSystem.deleteDirectory ( to )
		end
		
		MOAIFileSystem.copy ( from, to )
		MOAIFileSystem.setWorkingDirectory ( to )
		
		if MOAITestMgr.checkFilter () then
			os.execute ( string.format ( '%%MOAI_BIN%%\\moai-test -r "%s" main.lua', results ))
		end
		
		MOAIFileSystem.setWorkingDirectory ( rootDir )
		gatherResults ( results, allResults )
	end
end

MOAIFileSystem.deleteFile ( results )
