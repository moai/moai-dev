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
usageText["environment"] = [[
    Cmd: moaiutil environment 
        This command will give you the needed environment variables to use moaiutil.
        Example:
            /absolute/path/to/moai_sdk/util/moaiutil environment
            (Follow instructions)
]]

usageText["init"] = [[
    Cmd: moaiutil init
        Run from your project folder to initialize a moaiutil-managed set of host projects.
        Creates a hostconfig.lua file, which you must edit to describe your MOAI project.
        Example:
            mkdir newMOAIProject && cd newMOAIProject   # use the toolbelt in a new project
            moaiutil init 
            vi hostconfig.lua                           #&etc.
]]

usageText["host"] = [[
    Cmd: moaiutil host <subcommand> <args>
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
            cd newMOAIProject && moaiutil host create ios && \
                                 moaiutil host create android-studio && \
                                 moaiutil host create host osx-app && #etc.
]]

usageText["make-lua-docs"] = [[
    Cmd: moaiutil make-lua-docs 
        Creates compact documentation for the Lua-side of the MOAI API.
]]

usageText["make-cpp-docs"] = [[
    Cmd: moaiutil make-cpp-docs
        Creates compact documentation from the MOAI C/C++ codebase.
        Example:    
            cd newMOAIProject && moaiutil make-cpp-docs
            find moai-sdk/ #&etc.
]]

usageText["package-cmake-sdk"] = [[
    Cmd: moaiutil package-cmake-sdk
        Creates a distributable SDK based on cmake-driven MOAI build.
        Example:
            cd newMOAIProject && moaiutil package-cmake-sdk
            find moai-sdk/ #&etc.
]]

usageText["package-sdk"] = [[
    Cmd: moaiutil package-sdk
        Create the standard release of the MOAI SDK.
]]

usageText["run-samples"] = [[
    Cmd: moaiutil run-samples
        Run the MOAI samples.
]]

usageText["run-tests"] = [[
    Cmd: moaiutil run-tests
        Run the Test suite.
]]

usageText["sdk-version"] = [[
    Cmd: moaiutil sdk-version
        Obtain the MOAI SDK Version info for the current configuration.
]]

function usage(subSection)
    print ("MOAI Utility Toolbelt - ", subSection or "general usage:")
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

