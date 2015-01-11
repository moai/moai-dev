ensureHostConfig()

if (not canCreateHost(hostname)) then
  command_error("I do not know how to create host: "..hostname)
end

if (hostAlreadyCreated(hostname)) then
  if not confirm("This will remove the existing host named '"..hostname.."' and replace with a new one based on hostconfig.lua\n"
        .."The old host and all modifications will be deleted!") then
    print("Ok. Aborting host create")
    os.exit(0)
  else
    print("\nRemoving old host")
    MOAIFileSystem.deleteDirectory(hostsFolder..hostname, true)
  end
end

--we get here if we don't exist and we are a valid host
print("creating host: "..hostname)
os.execute("moaiutil "..hosts[hostname].." -c hostconfig.lua -o "..hostsFolder..hostname)
print("\n Host created ")

  