-- Can the tracer survive a pcall with an error?

luatrace = require("luatrace")

function fail()
  error()
end


function pcall_fail()
  local status, i = pcall(fail)
end


luatrace.tron()
pcall_fail()
luatrace.troff()