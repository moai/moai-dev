require ( 'util' )
require ( 'http' )

--==============================================================
-- setup
--==============================================================

INVOKE_DIR      = MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 1 ])
MOAI_SDK_HOME   = MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 2 ])

local HELP = {
    [ 'help' ]  = true,
    [ '?' ]     = true,
    [ '-?' ]    = true,
    [ '--?' ]   = true,
}

----------------------------------------------------------------
local isCommand = function ( param )
    return param and MOAIFileSystem.checkFileExists ( string.format ( '%sutil/%s/main.lua', MOAI_SDK_HOME, param ))
end

local param1 = arg [ 3 ] or ''
local param2 = arg [ 4 ] or ''

if not arg [ 3 ] or HELP [ arg [ 3 ]] then

    arg [ 3 ] = 'help'
end

if isCommand ( arg [ 3 ]) then

    if arg [ 4 ] and HELP [ arg [ 4 ]] then
        arg [ 4 ] = arg [ 3 ]
        arg [ 3 ] = 'help'
    end

    MOAI_CMD = arg [ 3 ]
    SCRIPT_DIR = string.format ( '%sutil/%s/', MOAI_SDK_HOME, MOAI_CMD )

    MOAIFileSystem.setWorkingDirectory ( SCRIPT_DIR )
    dofile ( 'main.lua' ) 
else

    print ( 'could not find command:', arg [ 3 ])
end
