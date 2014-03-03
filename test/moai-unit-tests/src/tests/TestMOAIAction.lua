
-- test case
local TestMOAIAction = {}
_G.TestMOAIAction = TestMOAIAction

---
-- setUp
function TestMOAIAction:setUp()
    self.action = MOAIAction.new()
end

---
-- tearDown
function TestMOAIAction:tearDown()

end

---
-- test: addChild
function TestMOAIAction:test_addChild()
    local child = MOAIAction.new()
    local action = self.action:addChild(child)
    
    assertTrue(action ~= nil)
end
