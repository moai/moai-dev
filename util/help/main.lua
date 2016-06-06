--==============================================================
-- args
--==============================================================

local UTIL_DIR = string.format ( '%s%s/', MOAI_SDK_HOME, 'util' )

----------------------------------------------------------------
local checkHelp = function ( command )

	if command then
		local filename = string.format ( '%s%s/help.txt', UTIL_DIR, command )
		return MOAIFileSystem.checkFileExists ( filename ), filename
	end

	return false
end

----------------------------------------------------------------
local printHelp = function ( command )

	local exists, filename = checkHelp ( command )

	if exists then

		local help = MOAIFileSystem.loadFile ( filename )

		if help then
			help = string.match ( help, '^%s*(.-)%s*$' )
			print ( help )
			print ()
		end
	end
end

command = arg [ 4 ]

if checkHelp ( command ) then

	printHelp ( command )
else

	print ( 'pito - the MOAI toolbelt - general usage:' )
	print ()

	local directories = MOAIFileSystem.listDirectories ( UTIL_DIR )

	for i, directory in ipairs ( directories ) do
		printHelp ( directory )
	end
end
