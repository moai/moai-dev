WORKING_DIR = MOAIFileSystem.getWorkingDirectory () 

PROJECT_DIR = WORKING_DIR .. 'project'
STAGING_DIR = WORKING_DIR .. 'staging'
TESTING_DIR = WORKING_DIR .. 'testing'

local log = {}

local bootstrap = function ( mode, processResults )

	local filename = 'main.lua'

	if MOAIFileSystem.checkFileExists ( filename ) then

		local chunk, err = loadfile ( filename )

		if chunk then

			local env = setmetatable ({}, { __index = _G })
			setfenv ( chunk, env )
			chunk ()

			local func = env [ mode ]

			if func then

				local suiteName = processResults and ( env.TEST_SUITE or MOAIFileSystem.getWorkingDirectory ())

				func ()

				if suiteName then
					local results = {
						suite = suiteName,
						log = MOAITestMgr.results (),
					}
					table.insert ( log, results )
				end
			end
		else
			print ( err )
		end
	end
end

MOAITestMgr.setProjectDir ( PROJECT_DIR )
MOAITestMgr.setStagingDir ( STAGING_DIR )
MOAITestMgr.setTestingDir ( TESTING_DIR )

MOAITestMgr.setStagingFunc ( function () bootstrap ( 'stage', false ) end )
MOAITestMgr.setTestingFunc ( function () bootstrap ( 'test', true ) end )

MOAITestMgr.runTests ()

local fileStream = MOAIFileStream.new ()
fileStream:open ( 'log.json', MOAIFileStream.READ_WRITE_NEW )
fileStream:write ( MOAIJsonParser.encode ( log, MOAIJsonParser.JSON_INDENT ))
fileStream:close ()
