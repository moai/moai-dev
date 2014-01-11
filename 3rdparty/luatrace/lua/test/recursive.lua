local luatrace = require("luatrace")

function a(n)
  print("wait")
  if n > 0 then
    return a(n-1)
  end
end

luatrace.tron()
a(1)
luatrace.troff()