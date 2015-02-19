
local WORKING_DIR = MOAIFileSystem.getWorkingDirectory () 

local PROJECT_DIR = WORKING_DIR .. 'project/'
local STAGING_DIR = WORKING_DIR .. 'staging/'
local TESTING_DIR = WORKING_DIR .. 'testing/'

MOAISim.openWindow ( "test", 640, 480 )

gLog = {}

local bootstrap = function ( mode )

	print ( 'bootstrap', MOAIFileSystem.getWorkingDirectory ())

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
					print ( 'TEST' )
					MOAITestMgr.test ( env.TEST_SUITE, func )
					table.insert ( gLog, MOAITestMgr.results ())
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

MOAITestMgr.setStagingFunc ( function () bootstrap ( 'stage' ) end )
MOAITestMgr.setTestingFunc ( function () bootstrap ( 'test' ) end )

local main = function ()

	MOAITestMgr.runTests ()

	local fileStream = MOAIFileStream.new ()
	fileStream:open ( 'log.json', MOAIFileStream.READ_WRITE_NEW )
	fileStream:write ( MOAIJsonParser.encode ( gLog, MOAIJsonParser.JSON_INDENT ))
	fileStream:close ()

	os.exit ( 0 )
end

local thread = coroutine.create ( main )
MOAISim.setListener ( MOAISim.EVENT_STEP, function () coroutine.resume ( thread ) end )
