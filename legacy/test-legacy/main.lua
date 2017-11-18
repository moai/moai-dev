
local WORKING_DIR = MOAIFileSystem.getWorkingDirectory () 

local PROJECT_DIR = WORKING_DIR .. 'project/'
local STAGING_DIR = WORKING_DIR .. 'staging/'
local TESTING_DIR = WORKING_DIR .. 'testing/'

--MOAISim.openWindow ( "test", 320, 320 )

local bootstrap = function ( mode )
	os.execute ( string.format ( 'moai ../../bootstrap.lua %s', mode ))
	--loadfile ( '../../bootstrap.lua' )( mode )
end

MOAITestMgr.setProjectDir ( PROJECT_DIR )
MOAITestMgr.setStagingDir ( STAGING_DIR )
MOAITestMgr.setTestingDir ( TESTING_DIR )

MOAITestMgr.setStagingFunc ( function () bootstrap ( 'stage' ) end )
MOAITestMgr.setTestingFunc ( function () bootstrap ( 'test' ) end )

MOAITestMgr.runTests ()
