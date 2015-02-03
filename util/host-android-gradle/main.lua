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
    ApplicationId = "com.getmoai.MoaiTemplate",
    SdkDir = "",
    NdkDir = ""..(os.getenv("ANDROID_NDK") and MOAIFileSystem.getAbsoluteDirectoryPath(os.getenv("ANDROID_NDK"))),
    Modules = {},
    LuaSrc = MOAI_SDK_HOME..'samples/hello-moai',
    FBAppId = 0,
    GMSAppId = 0,
    SchemeName ="moaischeme"
}


MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)
local config = {}
local configFile = false
config.OUTPUT_DIR                       = INVOKE_DIR..'hosts/android-studio/'
config.LIB_SOURCE                      = MOAI_SDK_HOME..'lib/android/'
config.USE_SYMLINK                      = false

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
   
	for entry in util.iterateFiles(MOAI_SDK_HOME..'host-templates/android/studio/MoaiTemplate', false, true) do
			local fullpath = string.format ( '%s/%s',MOAI_SDK_HOME..'host-templates/android/studio/MoaiTemplate' , entry )
			print( string.format( '%s -> %s', fullpath, output..entry ))
			MOAIFileSystem.copy(fullpath, output..entry)
	end

    
    local hostmodules = MOAI_SDK_HOME..'src/host-modules'
    MOAIFileSystem.copy(MOAI_SDK_HOME..'src/moai-android/moai.cpp', output..'app/src/main/jni/moai.cpp')

    MOAIFileSystem.copy(hostmodules, output..'/app/src/main/jni/host-modules')

    --don't want these ones
    MOAIFileSystem.deleteFile(output..'app/src/main/jni/host-modules/aku_modules_ios.h')
    MOAIFileSystem.deleteFile(output..'app/src/main/jni/host-modules/aku_modules_ios_config.h')
    MOAIFileSystem.deleteFile(output..'app/src/main/jni/host-modules/aku_modules_ios.mm')

    --java classes
  --  files = util.listFiles( MOAI_SDK_HOME..'src/moai-android', ".java")
  --  for k, entry in pairs ( files ) do
  --  	local fullpath = string.format ( '%s/%s',MOAI_SDK_HOME..'src/moai-android' , entry )
--		print( string.format( '%s -> %s', fullpath, output..'app/src/main/java/com/ziplinegames/moai/'..entry ))
	--	MOAIFileSystem.copy(fullpath, output..'app/src/main/java/com/ziplinegames/moai/'..entry)
--    end
  --  MOAIFileSystem.deleteFile(output..'app/src/main/java/com/ziplinegames/moai/MoaiActivity.java')
--    MOAIFileSystem.deleteFile(output..'app/src/main/java/com/ziplinegames/moai/MoaiView.java')

    --lua
    local hostsourcedir = output..'app/src/main/java/com/getmoai/androidhost/'
    MOAIFileSystem.affirmPath(hostsourcedir)
    MOAIFileSystem.copy(MOAI_SDK_HOME..'src/moai-android/MoaiActivity.java', hostsourcedir..'MoaiActivity.java')
    MOAIFileSystem.copy(MOAI_SDK_HOME..'src/moai-android/MoaiView.java', hostsourcedir..'MoaiView.java')
    
    --[[
	for entry in util.iterateFiles(MOAI_SDK_HOME..'samples/hello-moai', false, true) do
			local fullpath = string.format ( '%s/%s',MOAI_SDK_HOME..'samples/hello-moai' , entry )
			print( string.format( '%s -> %s', fullpath, output..'lua/'..entry ))
			MOAIFileSystem.copy(fullpath, output..'lua/'..entry)
	end
    ]]
   


end

applyConfigFile = function(configFile)
  print("reading config from "..configFile)
  util.dofileWithEnvironment(configFile, hostconfig)
  
  --copy host specific settings to main config
  if (hostconfig["HostSettings"] and hostconfig["HostSettings"]["android-gradle"]) then
    for k,v in pairs(hostconfig["HostSettings"]["android-gradle"]) do
      hostconfig[k] = v
    end
  end
  
  hostconfig["HostSettings"] = nil
  
end

configureHost = function()
  local output = config.OUTPUT_DIR
   print("\n\nApplying config from "..configFile..":")
  for k,v in pairs(hostconfig) do
    print (k..": ", v)
  end
  local hostsourcedir = output..'app/src/main/java/com/getmoai/androidhost/'
   local hostfiles = {
            [ hostsourcedir..'MoaiActivity.java' ] = true,
            [ hostsourcedir..'MoaiView.java' ] = true
          }

  local modules = hostconfig['Modules'] or {}
  
  local modulestr = ""
  
  for _,v in pairs(modules) do
    if modulestr == "" then
      modulestr = v
    else
      modulestr = modulestr..','..v
    end
  end
  print("configuring lua source root")
  local oldworkingdir = MOAIFileSystem.getWorkingDirectory()
  --get lua path relative to config file as absolute
  MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)
  
  local luasrc = MOAIFileSystem.getAbsoluteDirectoryPath(hostconfig['LuaSrc'])
  --change to host output folder

  MOAIFileSystem.setWorkingDirectory(output)
  --now get lua path (currently absolute) as relative to gradle.properties
  luasrc = MOAIFileSystem.getRelativePath(luasrc, MOAIFileSystem.getWorkingDirectory() )
  
  if (not luasrc) then
    error("Error configuring lua source folder "..hostconfig['LuaSrc'])
  end
  --restore working dir
  MOAIFileSystem.setWorkingDirectory(oldworkingdir)
  
  local patternFor = function(name) 
    return '(<string name="'..name..'">)(.-)(</string>)'
  end
  --gradle windows paths need to be double escaped 
  local escGradlePath = function(dir)
    return dir:gsub("\\","\\\\"):gsub(":","\\:")
  end
  
  
  sdkdir = escGradlePath(hostconfig['SdkDir'])
  ndkdir = escGradlePath(hostconfig['NdkDir'])
  
  
  
  print("updating template values")
    --libroot
  util.replaceInFiles ({
	  [ util.wrap(pairs, hostfiles) ]  = {
      ['@PACKAGE@'] = 'com.getmoai.androidhost', --hardcoded, this package doesn't need to be unique per app anymore
      ['@WORKING_DIR@'] = 'bundle/assets'
    },
		[ output .. 'gradle.properties' ] = {
			[ 'moaiLibRoot=[^\n]+' ]= "moaiLibRoot=./lib",
			[ 'moaiLuaRoot=[^\n]+' ]= "moaiLuaRoot="..luasrc,
      [ 'moaiModules=[^\n]+' ]= "moaiModules="..modulestr
		},
    [ output .. 'app/build.gradle'] = {
      [ 'applicationId "[^"]+"']= 'applicationId "'..hostconfig['ApplicationId']..'"'
    },
    [ output .. 'app/src/main/res/values/strings.xml'] = {
      [patternFor("app_name")] = '%1'..hostconfig['AppName']..'%3',
      [patternFor("scheme_name")] = '%1'..hostconfig['SchemeName']..'%3',
      [patternFor("fb_app_id")] = '%1'..hostconfig['FBAppId']..'%3',
      [patternFor("gms_app_id")] = '%1'..hostconfig['GMSAppId']..'%3'
    }, 
    [ output .. 'local.properties' ] = {
      [ 'sdk.dir=[^\n]+' ]= "sdk.dir="..sdkdir,
      [ 'ndk.dir=[^\n]+' ]= "ndk.dir="..ndkdir
    }
	})
  
  print("copying icons")
  --icons
  MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)
    
  if (hostconfig['Icons']) then
    for k,v in pairs(hostconfig['Icons']) do
      if v == "" then
        print("Removing default icon ",k)
        MOAIFileSystem.deleteDirectory(config.OUTPUT_DIR.."app/src/main/res/drawable-"..k, true)
      else    
        if MOAIFileSystem.checkFileExists(v) then
          MOAIFileSystem.copy(v, config.OUTPUT_DIR.."app/src/main/res/drawable-"..k.."/icon.png")
        else
          error("Icon specified in config not found : "..k.."="..v)
        end
      end
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


applyConfigFile(configFile)
copyhostfiles()
configureHost()


if (config.USE_SYMLINK) then
	linklib()
else
	copylib()
end
