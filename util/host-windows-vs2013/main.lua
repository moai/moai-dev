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
    CompanyName = "Zipline Games",
    ExeDescription = ""
}


local config = {}

config.OUTPUT_DIR                      = INVOKE_DIR..'hosts/vs2013/'
config.LIB_SOURCE                      = MOAI_SDK_HOME..'lib/windows/vs2013/Distribute/'
config.USE_SYMLINK                     = false

MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)

local configFile = false
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

copyhostfiles = function() 
	local output = config.OUTPUT_DIR
	print("Creating ",output)
    MOAIFileSystem.affirmPath(output)


    local files = MOAIFileSystem.listFiles( MOAI_SDK_HOME..'host-templates/windows/vs2013')
	for k, entry in pairs ( files ) do
			local fullpath = string.format ( '%s/%s',MOAI_SDK_HOME..'host-templates/windows/vs2013' , entry )
			print( string.format( '%s -> %s', fullpath, output..entry ))
			MOAIFileSystem.copy(fullpath, output..entry)
	end

    local sdlhost = MOAI_SDK_HOME..'src/host-sdl'
    local hostmodules = MOAI_SDK_HOME..'src/host-modules'
    MOAIFileSystem.copy(sdlhost, output..'host-sdl')
    MOAIFileSystem.copy(hostmodules, output..'host-modules')

    --don't want these ones
    MOAIFileSystem.deleteFile(output..'host-modules/aku_modules_ios.h')
    MOAIFileSystem.deleteFile(output..'host-modules/aku_modules_ios_config.h')
    MOAIFileSystem.deleteFile(output..'host-modules/aku_modules_ios.mm')
    MOAIFileSystem.deleteFile(output..'host-modules/aku_modules_android.h')
    MOAIFileSystem.deleteFile(output..'host-modules/aku_modules_android_config.h')
    MOAIFileSystem.deleteFile(output..'host-modules/aku_modules_android.cpp')

end

configureHost = function()
  print("\n\nApplying config from "..configFile..":")
  for k,v in pairs(hostconfig) do
    print (k..": ", v)
  end
  
  local asUTF16Pattern = function(str)
      local newstr = ""
      for i = 1, #str do
        local c = str:sub(i,i)
        newstr = newstr.."."..c
      end
      return newstr
  end

  local asUTF16 = function(str)
      local newstr = ""
      for i = 1, #str do
        local c = str:sub(i,i)
        newstr = newstr.."\0"..c
      end
      return newstr
  end
  
  local utfReplace = function(source, target, replace)
    return source:gsub(asUTF16Pattern(target),asUTF16(replace))
  end
  
  
  --get our resource file contents
  local rcfile = io.open(config.OUTPUT_DIR.."moai.rc","rb")
  if (not rcfile) then error("failed to open moai.rc") end
  local contents = rcfile:read("*all")
  rcfile:close()

  --apply substitutions
  contents = utfReplace(contents, '<Product name>', hostconfig.AppName)
  contents = utfReplace(contents, '<Company name>', hostconfig.CompanyName)
  contents = utfReplace(contents, '<File description>', hostconfig.ExeDescription)

  --dump back to file
  rcfile = io.open(config.OUTPUT_DIR.."moai.rc","wb")
  rcfile:write(contents)
  io.close(rcfile)
  
  
  --app icon
  if (hostconfig.Icon) then
    local icon = MOAIFileSystem.getAbsoluteFilePath(hostconfig.Icon)
    if (MOAIFileSystem.checkFileExists(icon)) then
      MOAIFileSystem.copy(icon, config.OUTPUT_DIR.."appicon.ico")
    else
      error("Could not find specified icon :"..icon.." - skipping")
    end
  end
 
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

applyConfigFile = function(configFile)
  util.dofileWithEnvironment(configFile, hostconfig)
 
  
  --copy host specific settings to main config
  if (hostconfig["HostSettings"] and hostconfig["HostSettings"]["windows"]) then
    for k,v in pairs(hostconfig["HostSettings"]["windows"]) do
      hostconfig[k] = v
    end
  end
  
  hostconfig["HostSettings"] = nil
  
end


applyConfigFile(configFile)

copyhostfiles()

configureHost()


if (config.USE_SYMLINK) then
	linklib()
else
	copylib()
end
