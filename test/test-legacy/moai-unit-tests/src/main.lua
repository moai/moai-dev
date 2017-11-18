-- import
require "tests"
require "LuaUnit"

MOAISim.openWindow ( "test", 320, 480 )

-- run tests
local luaUnit = LuaUnit:new()
luaUnit:setVerbosity( 1 )
luaUnit:runSuite()
