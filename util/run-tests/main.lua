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

EXEC_DIR		= nil
EXEC_MODE		= nil

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	-- project folder
	-- staging folder
	-- test folder
	
	if param then

		if escape == 'stage' or escape == 'test' then
			EXEC_DIR = util.getAbsoluteDirPath ( param, INVOKE_DIR )
			EXEC_MODE = escape
		end

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

--==============================================================
-- util
--==============================================================

local		execute

----------------------------------------------------------------
execute = function ( mode, path )

	os.execute ( string.format ( 'moaiutil run-tests --%s %s', mode, path ))
end

--==============================================================
-- main
--==============================================================

if EXEC_DIR and EXEC_MODE then

	local execFile = EXEC_DIR .. 'main.lua'

	if MOAIFileSystem.checkFileExists ( execFile ) then

		MOAIFileSystem.setWorkingDirectory ( EXEC_DIR )

		local chunk = loadfile ( execFile )
		local env = setmetatable ({}, { __index = _G })
		setfenv ( chunk, env )
		chunk ()

		local func = env [ EXEC_MODE ]

		if func then
			func ()
		end
	end

	os.exit ( 0 )
end

print ( 'PROJECT_DIR', PROJECT_DIR )
print ( 'STAGING_DIR', STAGING_DIR )
print ( 'TESTING_DIR', TESTING_DIR )

MOAIFileSystem.affirmPath ( STAGING_DIR )
MOAIFileSystem.affirmPath ( TESTING_DIR )

if MOAIFileSystem.checkPathExists ( PROJECT_DIR ) then

	local dirs = MOAIFileSystem.listDirectories ( PROJECT_DIR ) or {}
	for i, subdir in ipairs ( dirs ) do
		print ( subdir )

		local projectDir = PROJECT_DIR .. subdir
		local stagingDir = STAGING_DIR .. subdir
		local testingDir = TESTING_DIR .. subdir

		if not MOAIFileSystem.checkFileExists ( stagingDir ) then
			MOAIFileSystem.copy ( projectDir, stagingDir )
			execute ( 'stage', stagingDir )
		end

		MOAIFileSystem.deleteDirectory ( testingDir, true )
		MOAIFileSystem.copy ( stagingDir, testingDir )

		execute ( 'test', testingDir )
	end
end
