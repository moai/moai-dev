-- Can the tracer survive a pcall with an error down the stack?

luatrace = require("luatrace")

function fail(n)
  if n == 1 then
    error()
  end
  return fail(n - 1)
end


function pcall_fail()
  local status, i = pcall(fail, 5)
end


luatrace.tron()
pcall_fail()
luatrace.troff()