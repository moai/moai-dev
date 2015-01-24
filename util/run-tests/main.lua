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

MODE_TESTING			= 'test'
MODE_STAGING			= 'stage'
TEST_LOG_FILENAME		= 'log.lua'

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

local log = {}

----------------------------------------------------------------
bootstrap = function ( mode )

	MOAIFileSystem.setWorkingDirectory ( INVOKE_DIR )
	print ( MOAIFileSystem.getWorkingDirectory ())

	local filename = 'main.lua'

	if MOAIFileSystem.checkFileExists ( filename ) then

		local chunk = loadfile ( filename )
		local env = setmetatable ({}, { __index = _G })
		setfenv ( chunk, env )
		chunk ()

		local func = env [ mode ]

		if func then

			if mode == MODE_STAGING then
				func ()
			end

			if mode == MODE_TESTING then

				local runTest = function ()
					local folder = string.match ( INVOKE_DIR, "[/]?([^/]+)[/]+$" )
					MOAITestMgr.test ( env.TEST_SUITE or folder, func )
					MOAISerializer.serializeToFile ( TEST_LOG_FILENAME, MOAITestMgr.results ())
					os.exit ( 0 )
				end

				if env.WIN_WIDTH and env.WIN_HEIGHT then
					MOAISim.openWindow ( "test", env.WIN_WIDTH, env.WIN_HEIGHT )
					local thread = MOAICoroutine.new ()
					thread:run ( runTest )
					return
				end

				runTest ()

			end
		end
	end

	os.exit ( 0 )
end

----------------------------------------------------------------
execute = function ( mode )

	os.execute ( string.format ( 'moaiutil run-tests -b %s', mode ))

	if mode == 'test' then
		if MOAIFileSystem.checkFileExists ( TEST_LOG_FILENAME ) then
			table.insert ( log, dofile ( TEST_LOG_FILENAME ))
		end
	end
end

--==============================================================
-- main
--==============================================================

if BOOTSTRAP then
	bootstrap ( BOOTSTRAP )
	return
end

print ( 'PROJECT_DIR', PROJECT_DIR )
print ( 'STAGING_DIR', STAGING_DIR )
print ( 'TESTING_DIR', TESTING_DIR )

MOAITestMgr.setProjectDir ( PROJECT_DIR )
MOAITestMgr.setStagingDir ( STAGING_DIR )
MOAITestMgr.setTestingDir ( TESTING_DIR )

MOAITestMgr.setStagingFunc ( function () execute ( MODE_STAGING ) end )
MOAITestMgr.setTestingFunc ( function () execute ( MODE_TESTING ) end )

MOAITestMgr.runTests ()

local fileStream = MOAIFileStream.new ()
fileStream:open ( INVOKE_DIR .. 'log.json', MOAIFileStream.READ_WRITE_NEW )
fileStream:write ( MOAIJsonParser.encode ( log, MOAIJsonParser.JSON_INDENT ))
fileStream:close ()
