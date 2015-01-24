WORKING_DIR = MOAIFileSystem.getWorkingDirectory () 

PROJECT_DIR = WORKING_DIR .. 'project'
STAGING_DIR = WORKING_DIR .. 'staging'
TESTING_DIR = WORKING_DIR .. 'testing'

MOAISim.openWindow ( "test", 640, 480 )

gLog = {}

local bootstrap = function ( mode )

	local filename = 'main.lua'

	if MOAIFileSystem.checkFileExists ( filename ) then

		local chunk, err = loadfile ( filename )

		if chunk then

			local env = setmetatable ({}, { __index = _G })
			setfenv ( chunk, env )
			chunk ()

			local func = env [ mode ]

			if func then

				if mode == 'stage' then
					func ()
				end

				if mode == 'test' then
					MOAITestMgr.test ( env.TEST_SUITE, func )
					table.insert ( gLog, MOAITestMgr.results ())
				end
			end
		else
			print ( err )
		end
	end
end

local runTests = function ()

	if not MOAIFileSystem.checkPathExists ( PROJECT_DIR ) then return end

	local oldPath = MOAIFileSystem.getWorkingDirectory ()

	local dirs = MOAIFileSystem.listDirectories ( PROJECT_DIR ) or {}
	for i, subdir in ipairs ( dirs ) do

		MOAITestMgr.suite ( subdir )

		local projectDir = PROJECT_DIR .. subdir
		local stagingDir = STAGING_DIR .. subdir
		local testingDir = TESTING_DIR .. subdir

		if not MOAIFileSystem.checkFileExists ( stagingDir ) then
			MOAIFileSystem.copy ( projectDir, stagingDir )
			MOAIFileSystem.setWorkingDirectory ( stagingDir )
			bootstrap ( 'stage' )
		end

		MOAIFileSystem.deleteDirectory ( testingDir, true )
		MOAIFileSystem.copy ( stagingDir, testingDir )

		MOAIFileSystem.setWorkingDirectory ( testingDir )
		bootstrap ( 'test' )
	end

	MOAIFileSystem.setWorkingDirectory ( oldPath )
end

--MOAITestMgr.setProjectDir ( PROJECT_DIR )
--MOAITestMgr.setStagingDir ( STAGING_DIR )
--MOAITestMgr.setTestingDir ( TESTING_DIR )

--MOAITestMgr.setStagingFunc ( function () bootstrap ( 'stage' ) end )
--MOAITestMgr.setTestingFunc ( function () bootstrap ( 'test' ) end )

--MOAITestMgr.runTests ()

local thread = MOAICoroutine.new ()
thread:run ( function ()

	runTests ()

	local fileStream = MOAIFileStream.new ()
	fileStream:open ( 'log.json', MOAIFileStream.READ_WRITE_NEW )
	fileStream:write ( MOAIJsonParser.encode ( gLog, MOAIJsonParser.JSON_INDENT ))
	fileStream:close ()

end )
