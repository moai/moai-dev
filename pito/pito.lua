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

COMMANDS = {}

local addCommands               = nil
local addCommandsFromConfig     = nil
local isCommand                 = nil

----------------------------------------------------------------
local addCommands = function ( path )

    local directories = MOAIFileSystem.listDirectories ( path )

    for i, directory in ipairs ( directories or {}) do
        COMMANDS [ directory ] = string.format ( '%s/%s', path, directory )
    end
end

----------------------------------------------------------------
local addCommandsFromConfig = function ( commandFolder, config )

    local filename = string.format ( '%s/%s', commandFolder, config )

    if MOAIFileSystem.checkFileExists ( filename ) then

        local paths = util.tokenize ( MOAIFileSystem.loadFile ( filename ), ' .;' )

        for i = #paths, 1, -1 do

            local path = paths [ i ]
            if not util.isAbsPath ( path ) then
                path = MOAIFileSystem.getAbsoluteDirectoryPath ( string.format ( '%s/%s', commandFolder, paths [ i ]))
            end

            addCommands ( path )
        end
    end
end

----------------------------------------------------------------
local isCommand = function ( param )

    local path = COMMANDS [ param ]
    return path and MOAIFileSystem.checkFileExists ( string.format ( '%s/main.lua', path ))
end

--==============================================================
-- main
--==============================================================

local commandFolder = MOAIFileSystem.getAbsoluteDirectoryPath ( string.format ( '%sutil', MOAI_SDK_HOME ))

addCommands ( commandFolder )

while #commandFolder > 0 do

    addCommands ( string.format ( '%s/%s', commandFolder, 'pito' ))
    addCommandsFromConfig ( commandFolder, '.pito' )
    addCommandsFromConfig ( commandFolder, '.pito.local' )

    local lastDivider = string.find ( commandFolder, '/[^/]*$' )
    commandFolder = string.sub ( commandFolder, 1, lastDivider and lastDivider - 1 or 1 )
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

    if isCommand ( arg [ 3 ]) then

        SCRIPT_DIR  = COMMANDS [ arg [ 3 ]]
        MOAI_CMD    = SCRIPT_DIR and arg [ 3 ]
        

        MOAIFileSystem.setWorkingDirectory ( SCRIPT_DIR )
        dofile ( 'main.lua' )

        return
    end
end

print ( 'could not find command:', arg [ 3 ])
