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
local hostconfig = {
  AppName = "Moai Template",
  CompanyName = "Zipline Games"
}

local configFile = false

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
    MOAIFileSystem.deleteFile(output..'app/host-modules/aku_modules_ios.h')
    MOAIFileSystem.deleteFile(output..'app/host-modules/aku_modules_ios_config.h')
    MOAIFileSystem.deleteFile(output..'app/host-modules/aku_modules_ios.mm')
    MOAIFileSystem.deleteFile(output..'app/host-modules/aku_modules_android.h')
    MOAIFileSystem.deleteFile(output..'app/host-modules/aku_modules_android_config.h')
    MOAIFileSystem.deleteFile(output..'app/host-modules/aku_modules_android.cpp')
    
    
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
  if (hostconfig["HostSettings"] and hostconfig["HostSettings"]["osx"]) then
    for k,v in pairs(hostconfig["HostSettings"]["osx"]) do
      hostconfig[k] = v
    end
  end
  
  hostconfig["HostSettings"] = nil
  
end

configureHost = function()
  local output = config.OUTPUT_DIR
  --get lua folder path (relative to xcode project)
  local fullLua = MOAIFileSystem.getAbsoluteDirectoryPath(hostconfig['LuaSrc'])
  local relativeLua = MOAIFileSystem.getRelativePath( fullLua, output )
  local relativeLua = string.match(relativeLua, "(.*)/$") --strip trailing slash
  
  local luafolder = string.match(fullLua, ".*/([^/]-)/$") --ensure to ignore trailing slash
  
  local projectfiles = {
    [ output..'Moai Template.xcodeproj/project.xcworkspace/contents.xcworkspacedata' ] = true,
    [ output..'Moai Template.xcodeproj/xcshareddata/xcschemes/Moai Template.xcscheme' ] = true
    
  }

  util.replaceInFiles ({
    [ output..'Moai Template.xcodeproj/project.pbxproj' ] = {
        --our lua path
        ['(63157D5E19FCB4F0009018A3%C-name = ")([^"]-)(".-path = ")([^"]-)(".*)'] = "%1"..luafolder.."%3"..relativeLua.."%5",
        --our app name
        ['Moai Template'] = hostconfig['AppName']
    },
    [ util.wrap(pairs, projectfiles) ] = {
        ['Moai Template'] = hostconfig['AppName']
    },
    [ output..'bootstrap.lua'] = {
        ['setWorkingDirectory%(.-%)'] = 'setWorkingDirectory("'..luafolder..'")'
    }
  })

  if (hostconfig['AppName'] ~= 'Moai Template') then
    --rename the info plist
    MOAIFileSystem.copy(output..'app/Moai Template-Info.plist', output..'app/'..hostconfig['AppName']..'-Info.plist')
    MOAIFileSystem.deleteFile(output..'app/Moai Template-Info.plist')
    
    --rename the scheme
    MOAIFileSystem.copy(output..'Moai Template.xcodeproj/xcshareddata/xcschemes/Moai Template.xcscheme', 
                        output..'Moai Template.xcodeproj/xcshareddata/xcschemes/'..hostconfig['AppName']..'.xcscheme')  
    MOAIFileSystem.deleteFile(output..'Moai Template.xcodeproj/xcshareddata/xcschemes/Moai Template.xcscheme')
    
    --rename the project file too````
    MOAIFileSystem.copy(output..'Moai Template.xcodeproj', output..hostconfig['AppName']..'.xcodeproj')
    MOAIFileSystem.deleteDirectory(output..'Moai Template.xcodeproj', true)
  end 
  
  --icon
  if (hostconfig['Icon']) then
    local icon = MOAIFileSystem.getAbsoluteFilePath(hostconfig['Icon'])
    if (MOAIFileSystem.checkFileExists(icon)) then
      MOAIFileSystem.copy(icon, config.OUTPUT_DIR.."app/Icon-120.png")
    else
      error("Could not find specified icon :"..icon.." - skipping")
    end
  end

end
--63157D5E19FCB4F0009018A3

applyConfigFile(configFile)
copyhostfiles()
configureHost()


if (config.USE_SYMLINK) then
	linklib()
else
	copylib()
end
