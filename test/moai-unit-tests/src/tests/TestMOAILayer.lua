
-- test case
local TestMOAILayer = {}
_G.TestMOAILayer = TestMOAILayer

function TestMOAILayer:setUp()
    self.layer = MOAILayer.new()
end

function TestMOAILayer:tearDown()

end

---
-- test: clear
function TestMOAILayer:test_clear()
    self.layer:clear()
end

