--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

--============================================================--
-- MOAITestMgr
--============================================================--
MOAITestMgr.extend (

	'MOAITestMgr',
	
	----------------------------------------------------------------
	function ( class, superClass )

		local stagingFunc		= function () end
		local testingFunc		= function () end

		local projectDir		= './project/'
		local stagingDir		= './staging/'
		local testingDir		= './testing/'

		local superError		= superClass.error
		local superPopTest		= superClass.pop_test
		local superPushTest		= superClass.push_test

		class.error				= function () assert ( false ) end
		class.pop_test			= function () assert ( false ) end
		class.push_test			= function () assert ( false ) end

		--MOAITestMgr.setProjectDir ( PROJECT_DIR )
		--MOAITestMgr.setStagingDir ( STAGING_DIR )
		--MOAITestMgr.setTestingDir ( TESTING_DIR )

		--MOAITestMgr.setStagingFunc ( function () bootstrap ( 'stage' ) end )
		--MOAITestMgr.setTestingFunc ( function () bootstrap ( 'test' ) end )

		--MOAITestMgr.runTests ()

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.runTests ()

			if not MOAIFileSystem.checkPathExists ( projectDir ) then return end

			local oldPath = MOAIFileSystem.getWorkingDirectory ()

			local dirs = MOAIFileSystem.listDirectories ( projectDir ) or {}
			for i, subdir in ipairs ( dirs ) do

				MOAITestMgr.suite ( subdir )

				local projectSubDir = projectDir .. subdir
				local stagingSubDir = stagingDir .. subdir
				local testingSubDir = testingDir .. subdir

				if not MOAIFileSystem.checkPathExists ( stagingSubDir ) then
					MOAIFileSystem.copy ( projectSubDir, stagingSubDir )
					MOAIFileSystem.setWorkingDirectory ( stagingSubDir )
					print ( 'STAGING', stagingSubDir )
					stagingFunc ()
				end

				MOAIFileSystem.deleteDirectory ( testingSubDir, true )
				MOAIFileSystem.copy ( stagingSubDir, testingSubDir )

				MOAIFileSystem.setWorkingDirectory ( testingSubDir )
				print ( 'TESTING', testingSubDir )
				testingFunc ()
			end

			MOAIFileSystem.setWorkingDirectory ( oldPath )
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.setProjectDir ( dir )

			projectDir = MOAIFileSystem.getAbsoluteDirectoryPath ( dir )
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.setStagingDir ( dir )

			stagingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( dir )
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.setStagingFunc ( f )

			stagingFunc = f
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.setTestingDir ( dir )

			testingDir = MOAIFileSystem.getAbsoluteDirectoryPath ( dir )
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.setTestingFunc ( f )

			testingFunc = f
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.test ( s, f  )
			
			superPushTest ( s )

			local co = coroutine.create ( f )

			while true do

				local success, err = coroutine.resume ( co )

				if not success then
					superError ( err, co )
				end

				if coroutine.status ( co ) == 'dead' then break end
				coroutine.yield ()
			end

			superPopTest ()
		end

	end
)
