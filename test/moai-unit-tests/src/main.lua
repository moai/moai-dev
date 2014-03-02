arg = nil

-- import
tests = require "tests"
require "LuaUnit"

MOAISim.openWindow ( "test", 320, 480 )

-- run tests
LuaUnit:setVerbosity( 1 )
LuaUnit:run(unpack(tests))
--os.exit(LuaUnit:run(unpack(tests)))
