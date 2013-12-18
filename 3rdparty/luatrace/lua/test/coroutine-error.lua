-- Can the tracer survive a coroutine with an error down the stack?

luatrace = require("luatrace")

function fail(n)
  if n == 1 then
    error()
  end
  coroutine.yield(1)
  return fail(n - 1)
end


function coroutine_fail()
  local t = coroutine.create(fail)
  coroutine.resume(t, 5)
  while true do
    local status, i = coroutine.resume(t)
    if not status or not i then break end
  end
end


luatrace.tron()
coroutine_fail()
luatrace.troff()