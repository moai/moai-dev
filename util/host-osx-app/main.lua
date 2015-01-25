--==============================================================
-- setup
--==============================================================



function table.contains(table, element)
  for _, value in pairs(table) do
    if value == element then
      return true
    end
  end
  return false
end

--==============================================================
-- args
--==============================================================

local config = {}

config.OUTPUT_DIR                       = INVOKE_DIR..'hosts/osx/'
config.LIB_SOURCE                      = MOAI_SDK_HOME..'lib/osx'
config.USE_SYMLINK                      = false

MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)

for i, escape, param, iter in util.iterateCommandLine ( arg or {}) do

	if escape == 's' or escape == 'use-symlink' then
		config.USE_SYMLINK = true
	end
	if (param) then
		if escape == 'o' or escape == 'output-dir' then
			config.OUTPUT_DIR = MOAIFileSystem.getAbsoluteDirectoryPath(param)
		end

	    if escape == 'l' or escape == 'lib-source' then
			config.LIB_SOURCE = MOAIFileSystem.getAbsoluteDirectoryPath(param)
		end
	end
end

--==============================================================
-- actions
--==============================================================

local copyhostfiles 
local copylib
local linklib

copyhostfiles = function() 
	local output = config.OUTPUT_DIR
	print("Creating ",output)
    MOAIFileSystem.affirmPath(output)


    
	for  entry in util.iterateFiles(MOAI_SDK_HOME..'host-templates/osx/Moai Template', false, true) do
			local fullpath = string.format ( '%s/%s',MOAI_SDK_HOME..'host-templates/osx/Moai Template' , entry )
			print( string.format( '%s -> %s', fullpath, output..entry ))
			MOAIFileSystem.copy(fullpath, output..entry)
	end
    --we want the copy from src
	MOAIFileSystem.deleteDirectory(output..'host-sdl',true)
	MOAIFileSystem.deleteDirectory(output..'host-modules',true)

    local sdlhost = MOAI_SDK_HOME..'src/host-sdl'
    local hostmodules = MOAI_SDK_HOME..'src/host-modules'
    MOAIFileSystem.copy(sdlhost, output..'app/host-sdl')
    MOAIFileSystem.copy(hostmodules, output..'app/host-modules')

    --don't want these ones
    MOAIFileSystem.deleteFile(output..'Moai Template/host-modules/aku_modules_ios.h')
    MOAIFileSystem.deleteFile(output..'Moai Template/host-modules/aku_modules_ios_config.h')
    MOAIFileSystem.deleteFile(output..'Moai Template/host-modules/aku_modules_ios.mm')

end

copylib = function() 
	MOAIFileSystem.copy(config.LIB_SOURCE, config.OUTPUT_DIR..'lib' )
end

linklib = function() 
	local isWindows = MOAIEnvironment.osBrand == 'Windows'
	local cmd = isWindows and 'mklink /D "'..config.OUTPUT_DIR..'lib" "'..config.LIB_SOURCE..'"' 
	                      or 'ln -s "'..config.LIB_SOURCE..'" "'..config.OUTPUT_DIR..'lib"'
	if os.execute(cmd) > 0 then
	   print ("Error creating link, try running as administrator")
	end

end

copyhostfiles()
if (config.USE_SYMLINK) then
	linklib()
else
	copylib()
end
