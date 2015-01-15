--==============================================================
-- args
--==============================================================

-- Visit every folder; generate staging if none; copy staging to test; run test and append to log.

-- Same as above with separte process for each run.

-- Run in starting process.
-- Run in separate processes.
-- Run with new context between tests.


PROJECT_DIR = util.getAbsoluteDirPath ( 'project', INVOKE_DIR )
STAGING_DIR = util.getAbsoluteDirPath ( 'staging', INVOKE_DIR )
TESTING_DIR = util.getAbsoluteDirPath ( 'testing', INVOKE_DIR )

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	-- project folder
	-- staging folder
	-- test folder
	
	if param then

		if escape == 'p' or escape == 'project' then
			PROJECT_DIR =  util.getAbsoluteDirPath ( param, INVOKE_DIR )
		end

		if escape == 's' or escape == 'staging' then
			STAGING_DIR =  util.getAbsoluteDirPath ( param, INVOKE_DIR )
		end

		if escape == 't' or escape == 'testing' then
			TESTING_DIR =  util.getAbsoluteDirPath ( param, INVOKE_DIR )
		end
	end
end

print ( 'PROJECT_DIR', PROJECT_DIR )
print ( 'STAGING_DIR', STAGING_DIR )
print ( 'TESTING_DIR', TESTING_DIR )

MOAIFileSystem.affirmPath ( STAGING_DIR )
MOAIFileSystem.affirmPath ( TESTING_DIR )

if MOAIFileSystem.checkPathExists ( PROJECT_DIR ) then

	print ( 'we are in business!' )

	local dirs = MOAIFileSystem.listDirectories ( PROJECT_DIR ) or {}
	for i, subdir in ipairs ( dirs ) do
		print ( subdir )

		local projectDir = PROJECT_DIR .. subdir
		local stagingDir = STAGING_DIR .. subdir
		local testingDir = TESTING_DIR .. subdir

		if not MOAIFileSystem.checkFileExists ( stagingDir ) then
			MOAIFileSystem.copy ( projectDir, stagingDir )
			MOAIFileSystem.deleteDirectory ( testingDir, true )

			
		end

		if not MOAIFileSystem.checkFileExists ( testingDir ) then
			MOAIFileSystem.copy ( stagingDir, testingDir )
		end
	end
end
