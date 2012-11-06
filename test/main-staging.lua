dofile ( 'util.lua' )

rootDir = MOAIFileSystem.getWorkingDirectory ()
filter = '.filter'
results = rootDir .. '.results'
allResults = rootDir .. 'staging-results'
MOAIFileSystem.deleteFile ( allResults )

-- stage the C++ tests
stagingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'staging-cpp' )
MOAIFileSystem.affirmPath ( stagingDir )

testList = MOAITestMgr.getTestList ()
for i, testname in ipairs ( testList ) do
	
	local path = stagingDir .. testname .. '/'
	
	if MOAIFileSystem.checkPathExists ( path ) == false then
		MOAIFileSystem.affirmPath ( path )
		MOAIFileSystem.setWorkingDirectory ( path )
		
		os.execute ( string.format ( '%%MOAI_STAGING_VSNINE%%\\moai-test.exe -s -f "%s" -r "%s" -t "%s"', filter, results, testname ))

		MOAIFileSystem.setWorkingDirectory ( rootDir )
		gatherResults ( results, allResults )
	end
end

-- stage the Lua tests
sourceDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'source' )
print ( sourceDir )
if MOAIFileSystem.checkPathExists ( sourceDir ) == true then

	stagingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'staging-lua' )
	MOAIFileSystem.affirmPath ( stagingDir )

	sourceList = MOAIFileSystem.listDirectories ( sourceDir )

	for i, dirname in ipairs ( sourceList ) do

		local from = sourceDir .. dirname .. '/'
		local to = stagingDir .. dirname .. '/'

		if MOAIFileSystem.checkPathExists ( to ) == false then
			MOAIFileSystem.copy ( from, to )
			MOAIFileSystem.setWorkingDirectory ( to )
			
			os.execute ( string.format ( '%%MOAI_STAGING_VSNINE%%\\moai-test.exe -s -f "%s" -r "%s" main.lua', filter, results ))
			
			gatherResults ( results, allResults )
		end
	end
	
	MOAIFileSystem.setWorkingDirectory ( rootDir )
end

MOAIFileSystem.deleteFile ( results )
