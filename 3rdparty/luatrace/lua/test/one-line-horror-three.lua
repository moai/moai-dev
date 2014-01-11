-- Is the time for recursive functions that are defined on one line and
-- contain some "self work" accumulated correctly?

luatrace = require("luatrace")

sum = 0

function a(n)
  if n > 0 then
    local f = function() for i = 1, 100 do sum = sum + i end; a(n-1) end
    f()
  end
end


luatrace.tron()
a(2)
luatrace.troff()

