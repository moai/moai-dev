--==============================================================
-- setup environment for MOAI and moaiutil
--==============================================================
local envFileName = MOAI_SDK_HOME .. "env.sh"
local envStr = string.format ( 'export MOAI_SDK_HOME=%s\n', MOAI_SDK_HOME ) ..
               string.format ( 'export MOAI_SDK_BIN=$MOAI_SDK_HOME/util/moai\n') ..
               string.format ( 'export PATH=$MOAI_SDK_HOME/util:$PATH\n' ) 

envFile = io.open(envFileName, "w+")
envFile:write(envStr)
envFile:close()

print ( "\nA localized 'env.sh' script has been created for your environment.")
print ( "To set up your environment to use this MOAI_SDK, source the \n" .. envFileName .. " script in your shell.\ne.g.")
print (" \t\tsource " .. envFileName .. "\n")
