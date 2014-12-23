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
    
	for entry in util.iterateFiles(MOAI_SDK_HOME..'samples/hello-moai', false, true) do
			local fullpath = string.format ( '%s/%s',MOAI_SDK_HOME..'samples/hello-moai' , entry )
			print( string.format( '%s -> %s', fullpath, output..'lua/'..entry ))
			MOAIFileSystem.copy(fullpath, output..'lua/'..entry)
	end
    
    local hostfiles = {
            [ hostsourcedir..'MoaiActivity.java' ] = true,
            [ hostsourcedir..'MoaiView.java' ] = true
          }

    --libroot
  util.replaceInFiles ({
	  [ util.wrap(pairs, hostfiles) ]  = {
      ['@PACKAGE@'] = 'com.getmoai.androidhost',
      ['@WORKING_DIR@'] = 'bundle/assets'
    },
		[ output .. 'gradle.properties' ] = {
			[ 'moaiLibRoot=[^\n]+' ]= "moaiLibRoot=./lib",
			[ 'moaiLuaRoot=[^\n]+' ]= "moaiLuaRoot=./lua"
		} 
	})


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
