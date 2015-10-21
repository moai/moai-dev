require ( 'util' )
require ( 'http' )

--==============================================================
-- setup
--==============================================================

INVOKE_DIR      = MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 1 ])
MOAI_SDK_HOME   = MOAIFileSystem.getAbsoluteDirectoryPath ( arg [ 2 ])
MOAI_CMD        = arg [ 3 ]
SCRIPT_DIR      = string.format ( '%sutil/%s/', MOAI_SDK_HOME, MOAI_CMD )

local usageText={}
usageText["prepare"] = [[
    PREPARE to use the toolbelt:
        MOAI SDK has a toolbox environment script (env.sh) for use prior to loading the toolbelt:
            source $MOAI_SDK_HOME/env.sh
        (Presumably you found it already.)
]]

usageText["init"] = [[
    INITIALIZE Project folder:              moaiutil init
        Run from your project folder to initialize a moaiutil-managed set of host projects.
        Creates a hostconfig.lua file, which you must edit to describe your MOAI project.
        Example:
            mkdir newMOAIProject && cd newMOAIProject   # use the toolbelt in your own project workspace
            moaiutil init 
            vi hostconfig.lua                           #&etc.
]]

usageText["host"] = [[
    CREATE and Manage MOAI Projects:        moaiutil host <subcommand> <args>
        Subcommands:
            host list - Lists available hosts
            host init - Creates a template host config file used by subsequent commands
            host build <hostname> - Creates (if it doesn't exist) in hosts folder, and (re)builds the host named <hostname>. 
            host run <hostname> - Creates (if it doesn't exists) in hosts folder, (re)builds and runs the host named <hostname>. 
            host create <hostname> - Creates the host in the hosts folder (removing old host) based on latest config settings.
        Example:
            cd newMOAIProject && moaiutil host create ios && moaiutil host create android-studio && moaiutil host create host osx-app && #etc.
]]

usageText["make-lua-docs"] = [[
    MAKE Lua Developer Documentation:       moaiutil make-lua-docs 
        Creates compact documentation for the Lua-side of the MOAI API.
]]

usageText["make-cpp-docs"] = [[
    MAKE Developer Documentation:           moaiutil make-cpp-docs
        Creates compact documentation from the MOAI C/C++ codebase.
        Example:    
            cd newMOAIProject && moaiutil make-cpp-docs
            find moai-sdk/ #&etc.
]]

usageText["package-cmake-sdk"] = [[
    PACKAGE CMake-based SDK:                moaiutil package-cmake-sdk
        Creates a distributable SDK based on cmake-driven MOAI build.
        Example:
            cd newMOAIProject && moaiutil package-cmake-sdk
            find moai-sdk/ #&etc.
]]

usageText["package-sdk"] = [[
    PACKAGE the MOAI SDK:                   moaiutil package-sdk
        Create the standard release of the MOAI SDK.
]]

usageText["run-samples"] = [[
    RUN Samples:                            moaiutil run-samples
        Run the MOAI samples.
]]

usageText["run-tests"] = [[
    RUN Tests:                              moaiutil run-tests
        Run the Test suite.
]]

usageText["sdk-version"] = [[
    GET Version:                            moaiutil sdk-version
        Obtain the MOAI SDK Version info for the current configuration.
]]

function usage(subSection)
    print ("MOAI Utility Toolbelt " , subSection)
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

