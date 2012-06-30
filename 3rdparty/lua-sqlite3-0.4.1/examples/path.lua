
local path = "?;?.lua;../?;../?.lua"

if package == nil then
  LUA_PATH = path         -- Lua 5.0
else
  package.path = path     -- Lua 5.1
end
