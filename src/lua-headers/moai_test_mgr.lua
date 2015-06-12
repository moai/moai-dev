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

		local thread			= nil

		local dummyFunc			= function () assert ( false ) end

		class.error				= dummyFunc
		class.popTest			= dummyFunc
		class.pushTest			= dummyFunc
		class.setStepFunc		= dummyFunc
		class.suite				= dummyFunc

		local step = function ( co )
			local success, err = coroutine.resume ( co )
			if not success then
				print ( err )
				os.exit ( 1 )
			end
		end

		-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
		function class.runTests ()

			if not MOAIFileSystem.checkPathExists ( projectDir ) then
				os.exit ( 0 )
			end

			local main = function ()

				local log = {}

				local oldPath = MOAIFileSystem.getWorkingDirectory ()

				local dirs = MOAIFileSystem.listDirectories ( projectDir ) or {}
				for i, subdir in ipairs ( dirs ) do

					superClass.suite ( subdir )

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

					local results = MOAIJsonParser.decodeFromFile ( 'log.json' )
					if results then
						table.insert ( log, results )
					end
				end

				MOAIFileSystem.setWorkingDirectory ( oldPath )
				MOAIJsonParser.encodeToFile ( 'log.json', log )

				os.exit ( 0 )
			end

			thread = coroutine.create ( main )
			step ( thread )
			superClass.setStepFunc ( function () step ( thread ) end )
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

			local multiProc = false

			local main = function ()

				superClass.pushTest ( s )

				local co = coroutine.create ( f )
				
				while true do

					local success, err = coroutine.resume ( co )

					if not success then
						superClass.error ( err, co )
					end

					if coroutine.status ( co ) == 'dead' then break end
					coroutine.yield ()
				end

				local more = superClass.popTest ()

				if multiProc and not more then
					os.exit ( 0 )
				end
			end

			if thread then
				main ()
			else
				multiProc = true
				MOAITestMgr.standalone ()
				thread = coroutine.create ( main )
				step ( thread )
				superClass.setStepFunc ( function () step ( thread ) end )
			end
		end

	end
)
