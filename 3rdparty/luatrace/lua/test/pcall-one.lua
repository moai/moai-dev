-- Can the tracer survive a pcall?

luatrace = require("luatrace")

function ok()
  return 1
end


function pcall_ok()
  local status, i = pcall(ok)
end


luatrace.tron()
pcall_ok()
luatrace.troff()