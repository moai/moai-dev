-- Is the time for recursive functions that are defined on the same line as
-- some other work on that line accumulated correctly?
-- This may seem unlikely, but it happens - the work can be in figuring out
-- arguments or something like a call to sort (this is a made up example,
-- so it might not be quite right):
-- function sort_tree(t)
--   table.sort(t, function(a, b) return compare subtree(sort_tree(a), sort_tree(b)) end)
-- end

luatrace = require("luatrace")

sum = 0

function a(n)
  if n > 0 then
    for i = 1, 100 do sum = sum + i end; local f = function() a(n-1) end
    f()
  end
end


luatrace.tron()
a(2)
luatrace.troff()

