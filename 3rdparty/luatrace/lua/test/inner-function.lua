-- Does time in inner functions get accumulated properly?

luatrace = require("luatrace")

function c(f)
  for i = 1, 5 do
    f()
  end
end

function a()
  c(function() print("wait") end)
end

luatrace.tron()
a(5)
luatrace.troff()