local function a()
  local x=0
  for i=1,1e3 do
    x=x+i
  end
end

local function b()
  local x=0
  for i=1,1e3 do
    x=x+1
  end
end

local function c()
  local x=0
  for i=1,1e3 do
    x = x + 1
    ("%d"):format(x)
  end
end


local annotate = require("jit.annotate")

annotate.on()
a()
b()
a()
c()
annotate.off()
annotate.report()