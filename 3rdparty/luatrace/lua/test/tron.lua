local luatrace = require("luatrace")

function b()
  return 1
end
function c()
  return 2
end

luatrace.tron()
local a = b() + c()
print(a + b())
luatrace.troff()