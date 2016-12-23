ensureHostConfig()

if not hostAlreadyCreated(hostname) then
  dofile(SCRIPT_DIR..'create.lua')
end

if not hostAlreadyCreated(hostname) then
  command_error("could not find host :"..hostname)
end

MOAIFileSystem.setWorkingDirectory(hostsFolder..hostname)

local isWindows = MOAIEnvironment.osBrand == 'Windows'

if (isWindows) then
  if not MOAIFileSystem.checkFileExists("build.bat") then
    command_error("Could not build host '"..hostname.."'. Could not find build.bat")
  end
  print("Building host: "..hostname)
  os.execute("build.bat")
else
  if not MOAIFileSystem.checkFileExists("build.sh") then
    command_error("Could not build host '"..hostname.."'. Could not find build.sh")
  end
  print("Building host: "..hostname)
  os.execute("bash build.sh")
end

print("Build complete")

