local mode = ...
mode = mode or unpack ( arg )

local FILENAME = 'main.lua'

if MOAIFileSystem.checkFileExists ( FILENAME ) then

	local chunk, err = loadfile ( FILENAME )

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
				local suite = env.TEST_SUITE or string.match ( MOAIFileSystem.getWorkingDirectory (), "[/]?([^/]+)[/]+$" )
				MOAITestMgr.test ( suite, func )
			end
		end
	else
		print ( err )
	end
end
