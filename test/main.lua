WORKING_DIR = MOAIFileSystem.getWorkingDirectory () 

PROJECT_DIR = WORKING_DIR .. 'project'
STAGING_DIR = WORKING_DIR .. 'staging'
TESTING_DIR = WORKING_DIR .. 'testing'

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
				func ()
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

MOAITestMgr.runTests ()
