--==============================================================
-- util
--==============================================================

local checkHelp				= nil
local printCommandHelp		= nil
local printHelpFile			= nil

----------------------------------------------------------------
checkHelp = function ( command )

	if command and COMMANDS [ command ] then
		local filename = string.format ( '%s/help.txt', COMMANDS [ command ])
		return MOAIFileSystem.checkFileExists ( filename ), filename
	end

	return false
end

----------------------------------------------------------------
printCommandHelp = function ( command )

	local exists, filename = checkHelp ( command )

	if exists then
		printHelpFile ( filename )
	end
end

----------------------------------------------------------------
printHelpFile = function ( filename )

	local help = MOAIFileSystem.loadFile ( filename )

	if help then
		help = string.match ( help, '^%s*(.-)%s*$' )
		print ( help )
		print ()
	end
end

--==============================================================
-- main
--==============================================================

command = arg [ 4 ]

print ()

if checkHelp ( command ) then

	printCommandHelp ( command )
else

	printHelpFile ( 'pito.txt' )

	local sorted = {}
	for command in pairs ( COMMANDS ) do
		table.insert ( sorted, command )
	end
	table.sort ( sorted )

	for i, command in ipairs ( sorted ) do
		printCommandHelp ( command )
	end
end
