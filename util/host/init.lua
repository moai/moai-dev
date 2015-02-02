--Creates config file in current folder


if (MOAIFileSystem.checkFileExists("hostconfig.lua")) then
  print("Error: there is already a hostconfig.lua")
  return
end

--get config defaults
local config = {}
util.dofileWithEnvironment(SCRIPT_DIR.."/defaultconfig.lua")

--TODO use defaults for platform to find values for sdk/ndk directory, poke around for the lua folder etc by looking for main.lua in subfolders

if (MOAIFileSystem.copy(SCRIPT_DIR.."/defaultconfig.lua", INVOKE_DIR.."/hostconfig.lua")) then
  print("Default config created in hostconfig.lua")
else
  print("Error copying default config")
end




