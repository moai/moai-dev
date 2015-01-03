print("\n\n") --give us some room

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

hostsFolder = INVOKE_DIR..'/hosts/'

--TODO probably should iterate over folders and extract this information so we don't need to keep it up to date
hosts = { 
    ["android-gradle"] = "host-android-gradle",
    ["html"] = "host-html",
    ["ios"] = "host-ios",
    ["linux"] = "host-linux",
    ["osx"] = "host-osx-app",
    ["windows"] = "host-windows-vs2013"
  }

function printValidHosts()
  print("Available Hosts:")
  for k,v in pairs(hosts) do
    print("  "..k)
  end
end

function hostAlreadyCreated(hostname)
  return MOAIFileSystem.checkPathExists(hostsFolder..hostname)
end

function canCreateHost(hostname)
  return hosts[hostname] ~= nil 
end

function command_error(message)
  print()
  print("ERROR: "..message)
  print()
  os.exit(1)
end

function confirm(message)
  io.write(message)
  local answer
  while true do
    io.write("\nContinue [y/n]?")
    answer=io.read()
    if answer=="y" then
       return true
    elseif answer=="n" then
       return false
    end
  end
end

function hasConfig()
  return MOAIFileSystem.checkFileExists("hostconfig.lua")
end

function ensureHostConfig()
  if (not hasConfig()) then
    command_error("this command must be run from a project folder containing hostconfig.lua\nYou can create one using moaiutil host init\n")
  end
end


local subcommand = arg[4]
hostname = arg[5]

if (subcommand == "" or subcommand == nil) then
  usage()
  return
end


--all our subcommands are relative to invoke dir
MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)

if MOAIFileSystem.checkFileExists(SCRIPT_DIR..subcommand..'.lua') then
  dofile(SCRIPT_DIR..subcommand..'.lua')
else
  print('host command "'..subcommand..'" not found')
  usage()
end

os.exit(0)




