--==============================================================
-- args
--==============================================================

-- Visit every folder; generate staging if none; copy staging to test; run test and append to log.

-- Same as above with separte process for each run.

-- Run in starting process.
-- Run in separate processes.
-- Run with new context between tests.

SDK_TEST_DIR = MOAI_SDK_HOME .. 'test/'

PROJECT_DIR = util.getAbsoluteDirPath ( 'project', SDK_TEST_DIR )
STAGING_DIR = util.getAbsoluteDirPath ( 'staging', SDK_TEST_DIR )
TESTING_DIR = util.getAbsoluteDirPath ( 'testing', SDK_TEST_DIR )

BOOTSTRAP		= nil

----------------------------------------------------------------
for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do
	
	-- project folder
	-- staging folder
	-- test folder
	
	if param then

		if escape == 'b' then
			BOOTSTRAP = param
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

local		bootstrap
local		execute

----------------------------------------------------------------
bootstrap = function ( mode )

	local filename = 'main.lua'

	if MOAIFileSystem.checkFileExists ( filename ) then

		local chunk = loadfile ( filename )
		local env = setmetatable ({}, { __index = _G })
		setfenv ( chunk, env )
		chunk ()

		local func = env [ mode ]

		if func then
			func ()
		end
	end
end

----------------------------------------------------------------
execute = function ( mode )

	os.execute ( string.format ( 'moaiutil run-tests -b %s', mode ))
end

--==============================================================
-- main
--==============================================================

if BOOTSTRAP then
	bootstap ( BOOTSTRAP )
	os.exit ( 0 )
end

print ( 'PROJECT_DIR', PROJECT_DIR )
print ( 'STAGING_DIR', STAGING_DIR )
print ( 'TESTING_DIR', TESTING_DIR )

MOAITestMgr.setProjectDir ( PROJECT_DIR )
MOAITestMgr.setStagingDir ( STAGING_DIR )
MOAITestMgr.setTestingDir ( TESTING_DIR )

MOAITestMgr.setStagingFunc ( function () execute ( 'stage' ) end )
MOAITestMgr.setTestingFunc ( function () execute ( 'test' ) end )

MOAITestMgr.runTests ()
