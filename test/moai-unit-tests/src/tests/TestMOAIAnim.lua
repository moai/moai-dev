
-- test case
local TestMOAIAnim = {}
_G.TestMOAIAnim = TestMOAIAnim

---
-- setUp
function TestMOAIAnim:setUp()
    self.prop = MOAIProp.new()
    self.anim = MOAIAnim.new()
    self.animCurve1 = MOAIAnimCurve.new()
    self.animCurve2 = MOAIAnimCurve.new()
    self.animCurve3 = MOAIAnimCurve.new()
end

---
-- tearDown
function TestMOAIAnim:tearDown()
end

---
-- test: apply
function TestMOAIAnim:test_apply()
    local root = MOAIActionMgr.getRoot()
    assertTrue(root ~= nil)

    local newRoot = MOAIAction.new()
    MOAIActionMgr.setRoot(newRoot)

    root = MOAIActionMgr.getRoot()
    assertEquals(root, newRoot)
end

---
-- test: getLength
function TestMOAIAnim:test_getLength()
end

---
-- test: reserveLinks
function TestMOAIAnim:test_reserveLinks()
end

---
-- test: setLink
function TestMOAIAnim:test_setLink()
end


