-- Tracing tail returns is tricky, and if you're not careful you'll end up
-- attributing time to the wrong stack frame.
-- LuaJIT reports tailcalls differently - it doesn't work yet.

luatrace = require("luatrace")

function factorial(n, p)
  print(n, p)
  if n == 1 then
    return p or 1
  end
  return factorial(n - 1, (p or 1) * n)
end

luatrace.tron()
print(factorial(10))
luatrace.troff()

