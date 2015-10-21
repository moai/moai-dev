require ( 'util' )
require ( 'http' )

--==============================================================
-- setup
--==============================================================

INVOKE_DIR      = MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 1 ])
MOAI_SDK_HOME   = MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 2 ])
MOAI_CMD        = arg [ 3 ]
SCRIPT_DIR      = string.format ( '%sutil/%s/', MOAI_SDK_HOME, MOAI_CMD or "help" )

local usageText={}
usageText["wut"] = [[
    pito wut
        Contemplate your pito.
]]

usageText["environment"] = [[
    pito environment 
        This command will give you the needed environment variables to use pito.
        Example:
            /absolute/path/to/moai_sdk/util/pito environment
            (Follow instructions)
]]

usageText["init"] = [[
    pito init
        Run from your project folder to initialize a pito-managed set of host projects.
        Creates a hostconfig.lua file, which you must edit to describe your MOAI project.
        Example:
            mkdir newMOAIProject && cd newMOAIProject   # use the toolbelt in a new project
            pito init 
            vi hostconfig.lua                           #&etc.
]]

usageText["host"] = [[
    pito host <subcommand> <args>
        Subcommands:
            host list - Lists available hosts
            host init - Creates a template host config file used by subsequent commands
            host build <hostname>  - Creates (if it doesn't exist) in hosts folder, and 
                                     (re)builds the host named <hostname>. 
            host run <hostname>    - Creates (if it doesn't exists) in hosts folder, 
                                     (re)builds and runs the host named <hostname>. 
            host create <hostname> - Creates the host in the hosts folder (removing old 
                                     host) based on latest config settings.
        Example:
            cd newMOAIProject && pito host create ios && \
                                 pito host create android-studio && \
                                 pito host create host osx-app && #etc.
]]

usageText["make-lua-docs"] = [[
    pito make-lua-docs 
        Creates compact documentation for the Lua-side of the MOAI API.
]]

usageText["make-cpp-docs"] = [[
    pito make-cpp-docs
        Creates compact documentation from the MOAI C/C++ codebase.
        Example:    
            cd newMOAIProject && pito make-cpp-docs
            find moai-sdk/ #&etc.
]]

usageText["package-cmake-sdk"] = [[
    pito package-cmake-sdk
        Creates a distributable SDK based on cmake-driven MOAI build.
        Example:
            cd newMOAIProject && pito package-cmake-sdk
            find moai-sdk/ #&etc.
]]

usageText["package-sdk"] = [[
    pito package-sdk
        Create the standard release of the MOAI SDK.
]]

usageText["run-samples"] = [[
    pito run-samples
        Run the MOAI samples.
]]

usageText["run-tests"] = [[
    pito run-tests
        Run the Test suite.
]]

usageText["sdk-version"] = [[
    pito sdk-version
        Obtain the MOAI SDK Version info for the current configuration.
]]

function usage(subSection)
    print ("pito - the MOAI toolbelt - ", subSection or "general usage:")
    if (subSection) and (usageText[subSection])  then
        print(usageText[subSection])
    else
        for i,v in pairs(usageText) do
            print(v)
        end
    end
end

MOAIFileSystem.setWorkingDirectory ( SCRIPT_DIR )

if MOAIFileSystem.checkFileExists('main.lua') then
    dofile ( 'main.lua' )
else
    usage()
end

