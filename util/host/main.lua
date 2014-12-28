function usage()
  print([[
    
    Moai Host Utility. Run from project root folder (outside lua source)
    
    Usage: moaiutil host <subcommand> <args>
    
    Subcommands:
      host list - Lists available hosts
      host init - Creates a template host config file used by subsequent commands
      host build <hostname> - Creates (if it doesn't exist) in hosts folder, and (re)builds the host named <hostname>. 
      host run <hostname> - Creates (if it doesn't exists) in hosts folder, (re)builds and runs the host named <hostname>. 
      host create <hostname> - Creates the host in the hosts folder (removing old host) based on latest config settings.
    ]])
  
end



local subcommand = arg[4]

if (subcommand == "" or subcommand == nil) then
  usage()
  return
end


--TODO probably should iterate over folders and extract this information so we don't need to keep it up to date
local hosts = { 
    ["android-gradle"] = "host-android-gradle",
    ["html"] = "host-html",
    ["ios"] = "host-ios",
    ["linux"] = "host-linux",
    ["osx"] = "host-osx-app",
    ["windows"] = "host-windows-vs2013"
  }

if (subcommand == "list") then
  print("Available Hosts:")
  for k,v in pairs(hosts) do
    print("  "..k)
  end
  return
end


if MOAIFileSystem.checkFileExists(subcommand..'.lua') then
  dofile(subcommand..'.lua')
else
  print('host command "'..subcommand..'" not found')
  usage()
end





