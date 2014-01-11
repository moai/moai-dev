-- If a function calls itself (directly or indirectly), then you have to be
-- careful that you only count that function's time once.
-- If you do it wrong, you'll have a record of a call to the "lower" function
-- with the correct self and child time, and the upper function's self and
-- child time will be an accurate reflection of the time elapsed while the upper
-- function was running, but when you accumulate the time for the two calls,
-- you'll count the time in the lower function twice.

luatrace = require("luatrace")

function a(i)
  print("wait a")
  if i > 1 then
    return b(i-1)
  end
end

function b(i)
  print("wait b")
  if i > 1 then
    return a(i-1)
  end
end

luatrace.tron()
a(6)
luatrace.troff()