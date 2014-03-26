
-- test case
local TestMOAIActionMgr = {}
_G.TestMOAIActionMgr = TestMOAIActionMgr

---
-- setUp
function TestMOAIActionMgr:setUp()
    self.action = MOAIAction.new()
end

---
-- tearDown
function TestMOAIActionMgr:tearDown()
    MOAIActionMgr.getRoot():clear()
end

---
-- test: getRoot
function TestMOAIActionMgr:test_getRoot()
    local root = MOAIActionMgr.getRoot()
    assertTrue(root ~= nil)

    local newRoot = MOAIAction.new()
    MOAIActionMgr.setRoot(newRoot)

    root = MOAIActionMgr.getRoot()
    assertEquals(root, newRoot)
end

---
-- test: setProfilingEnabled
function TestMOAIActionMgr:test_setProfilingEnabled()
    MOAIActionMgr.setProfilingEnabled(true)
    MOAIActionMgr.setProfilingEnabled(false)
end

---
-- test: setRoot
function TestMOAIActionMgr:test_setRoot()
    local root = MOAIActionMgr.getRoot()
    assertTrue(root ~= nil)

    local newRoot = MOAIAction.new()
    MOAIActionMgr.setRoot(newRoot)

    root = MOAIActionMgr.getRoot()
    assertEquals(root, newRoot)
end

---
-- test: setThreadInfoEnabled
function TestMOAIActionMgr:test_setThreadInfoEnabled()
    MOAIActionMgr.setThreadInfoEnabled(true)
    MOAIActionMgr.setThreadInfoEnabled(false)
end


