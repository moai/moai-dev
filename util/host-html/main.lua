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

local hostconfig = {
    AppName = "Moai Template",
    Ram = 128
}

local configFile = false
local config = {}

config.OUTPUT_DIR                       = INVOKE_DIR..'hosts/html/'
config.LIB_SOURCE                      = MOAI_SDK_HOME..'lib/html'
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
    
    if escape == 'c' or escape == 'config' then
      configFile = MOAIFileSystem.getAbsoluteFilePath(param)
    end
	end
end

--==============================================================
-- actions
--==============================================================

local copyhostfiles 
local copylib
local linklib
local applyConfigFile
local configureHost




-- Copies all files from the HTML host template to the output directory
copyhostfiles = function() 
	local output = config.OUTPUT_DIR
	print("Creating ",output)
    MOAIFileSystem.affirmPath(output)
	for  entry in util.iterateFiles(MOAI_SDK_HOME..'host-templates/html', false, true) do
			local fullpath = string.format ( '%s/%s',MOAI_SDK_HOME..'host-templates/html' , entry )
			print( string.format( '%s -> %s', fullpath, output..entry ))
			MOAIFileSystem.copy(fullpath, output..entry)
	end
  
  --make our executables executable again
  util.makeExecutable(output..'run.sh')
  util.makeExecutable(output..'build.sh')
  util.makeExecutable(output..'buildrom.sh')
end

-- Copies the generated moaijs.js file to the output directory
copylib = function() 
	print( string.format( 'Copying %s -> %s', config.LIB_SOURCE, config.OUTPUT_DIR..'/www/lib' ))
	MOAIFileSystem.copy(config.LIB_SOURCE, config.OUTPUT_DIR..'/www/lib' )
end

-- Creates a symbolic link that makes the generated moaijs.js file show up in the output directory
linklib = function() 
	print( string.format( 'Sym-linking %s -> %s', config.LIB_SOURCE, config.OUTPUT_DIR..'/www/lib' ))
	local isWindows = MOAIEnvironment.osBrand == 'Windows'
	local cmd = isWindows and 'mklink /D "'..config.OUTPUT_DIR..'/www/lib" "'..config.LIB_SOURCE..'"' 
	                      or 'ln -s "'..config.LIB_SOURCE..'" "'..config.OUTPUT_DIR..'/www/lib"'
	if os.execute(cmd) > 0 then
	   print ("Error creating link, try running as administrator")
	end

end

applyConfigFile = function(configFile)
  util.dofileWithEnvironment(configFile, hostconfig)
  
  --copy host specific settings to main config
  if (hostconfig["HostSettings"] and hostconfig["HostSettings"]["html"]) then
    for k,v in pairs(hostconfig["HostSettings"]["html"]) do
      hostconfig[k] = v
    end
  end
  
  --don't need the rest
  hostconfig["HostSettings"] = nil
end

configureHost = function()
  local output = config.OUTPUT_DIR
  print("\n\nApplying config from "..configFile..":")
  for k,v in pairs(hostconfig) do
    print (k..": ", v)
  end
  
  local oldworkingdir = MOAIFileSystem.getWorkingDirectory()
  --get lua path relative to config file as absolute
  MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)
  
  local luasrc = MOAIFileSystem.getAbsoluteDirectoryPath(hostconfig['LuaSrc'])
  --change to host output folder

  MOAIFileSystem.setWorkingDirectory(output)
  --now get lua path (currently absolute) as relative to buildrom.bat
  luasrc = MOAIFileSystem.getRelativePath(luasrc, MOAIFileSystem.getWorkingDirectory() )
  
  if (not luasrc) then
    error("Error configuring lua source folder "..hostconfig['LuaSrc'])
  end
  --restore working dir
  MOAIFileSystem.setWorkingDirectory(oldworkingdir)
  
  local buildromfiles = { 
     [output..'build.bat'] = true,
     [output..'build.sh'] = true
  }
    --libroot
  util.replaceInFiles ({
    [ util.wrap(pairs, buildromfiles) ]  = {
      ['@SRC@'] = luasrc,
    },
    [ output..'www/moai.html'] = {
      ['data%-title%=".-"'] = 'data-title="'..hostconfig.AppName..'"',
      ['data%-ram%=".-"'] = 'data-ram="'..hostconfig.Ram..'"'
    }
  })
  
  
  
    
  if (hostconfig['Background']) then
    print("copying background")
    local background = hostconfig['Background']
    --icons
    MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)
  
    if MOAIFileSystem.checkFileExists(background) then
      MOAIFileSystem.copy(background, config.OUTPUT_DIR.."www/background.jpg")
    else
      error("Background specified in config not found : "..background)
    end
  
  end
end



applyConfigFile(configFile)
copyhostfiles()
configureHost()


if (config.USE_SYMLINK) then
	linklib()
else
	copylib()
end
