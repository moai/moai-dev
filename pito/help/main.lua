--==============================================================
-- args
--==============================================================

----------------------------------------------------------------
local checkHelp = function ( command )

	if command and COMMANDS [ command ] then
		local filename = string.format ( '%s/help.txt', COMMANDS [ command ])
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

	local sorted = {}
	for command in pairs ( COMMANDS ) do
		table.insert ( sorted, command )
	end
	table.sort ( sorted )

	for i, command in ipairs ( sorted ) do
		printHelp ( command )
	end
end
