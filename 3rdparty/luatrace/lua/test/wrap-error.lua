-- Can the tracer survive a wrapped coroutine with an error down the stack?

luatrace = require("luatrace")

function fail(n)
  if n == 1 then
    error()
  end
  coroutine.yield(1)
  return fail(n - 1)
end


function coroutine_fail(f)
  f(5)
  while f() do end
end


function pcall_fail()
  local f = coroutine.wrap(fail)
  pcall(coroutine_fail, f)
end


luatrace.tron()
pcall_fail()
luatrace.troff()