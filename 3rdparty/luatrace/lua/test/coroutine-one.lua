-- Detecting coroutine resumes and traces is tricky, particularly as
-- + thanks to coroutine.wrap, you might resume from any function
-- + if the thread is finished, resume doesn't cause a thread change
-- + threads finish by returning, without a call to yield
-- Here we only test yielding from the top of the thread stack

luatrace = require("luatrace")

function co()
  for i = 1, 3 do
    coroutine.yield(i)
  end
end


function test_resume(t)
  while true do
    local status, i = coroutine.resume(t)
    if not status then print("ERROR: ", i) break end
    if not i then break end
  end
end


function test_wrap(f)
  while true do
    local i = f()
    if not i then break end
  end
end


luatrace.tron()
test_resume(coroutine.create(co))
test_wrap(coroutine.wrap(co))
luatrace.troff()