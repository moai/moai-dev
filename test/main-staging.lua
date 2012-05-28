sourceDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'source' )
stagingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( 'staging' )
rootDir = MOAIFileSystem.getWorkingDirectory ()

results = rootDir .. '.results'

MOAIFileSystem.affirmPath ( stagingDir )

sourceList = MOAIFileSystem.listDirectories ( sourceDir )
stagingList = MOAIFileSystem.listDirectories ( stagingDir )

command = string.format ( '%%MOAI_BIN%%\\moai-test -s -r "%s" main.lua', results )

for i, dirname in ipairs ( sourceList ) do
	if stagingList [ dirname ] == nil then
		
		local from = sourceDir .. dirname
		local to = stagingDir .. dirname

		MOAIFileSystem.copy ( from, to )
		
		MOAIFileSystem.setWorkingDirectory ( to )
		os.execute ( command )
	end
end

MOAIFileSystem.setWorkingDirectory ( rootDir )

MOAIFileSystem.deleteFile ( results )
