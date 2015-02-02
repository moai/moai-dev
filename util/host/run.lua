--build first
dofile(SCRIPT_DIR..'build.lua')

MOAIFileSystem.setWorkingDirectory(hostsFolder..hostname)
print("Running host")

local isWindows = MOAIEnvironment.osBrand == 'Windows'

if (isWindows) then
  os.execute("run.bat")
else
  os.execute("bash run.sh")
end


