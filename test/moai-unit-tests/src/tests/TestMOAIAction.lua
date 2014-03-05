
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
    MOAIActionMgr.getRoot():clear()
end

---
-- test: addChild
function TestMOAIAction:test_addChild()
    local child = MOAIAction.new()
    local action = self.action:addChild(child)
    
    assertEquals(action, self.action)
end

---
-- test: attach
function TestMOAIAction:test_attach()
    local parent = MOAIAction.new()
    local action = self.action:attach(parent)
    
    assertEquals(action, self.action)
end

---
-- test: clear
function TestMOAIAction:test_clear()
    local child = MOAIAction.new()
    local action = self.action:clear()
    assertEquals(action, self.action)

    self.action:addChild(child)
    action = self.action:clear()
    assertEquals(action, self.action)
end

---
-- test: detach
function TestMOAIAction:test_detach()
    local parent = MOAIAction.new()
    local action = self.action:attach(parent)
    assertEquals(action, self.action)
    
    local action = self.action:detach()
    assertEquals(action, self.action)
end

---
-- test: isActive
function TestMOAIAction:test_isActive()
    assertEquals(self.action:isActive(), false)
    
    self.action:start()
    assertEquals(self.action:isActive(), true)
    
    self.action:stop()
    assertEquals(self.action:isActive(), false)
end

---
-- test: isBusy
function TestMOAIAction:test_isBusy()
    local color = MOAIColor.new()
    local child = color:seekColor(0, 0, 0, 0, 4)
    assertEquals(self.action:isBusy(), false)
    
    self.action:addChild(child)
    self.action:start()
    assertEquals(self.action:isBusy(), true)
    
    self.action:pause()
    assertEquals(self.action:isBusy(), true)
    
    self.action:stop()
    assertEquals(self.action:isBusy(), false)
end

---
-- test: isDone
function TestMOAIAction:test_isDone()
    local color = MOAIColor.new()
    local child = color:seekColor(0, 0, 0, 0, 4)
    assertEquals(self.action:isDone(), true)
    
    self.action:addChild(child)
    self.action:start()
    assertEquals(self.action:isDone(), false)
    
    self.action:pause()
    assertEquals(self.action:isDone(), false)
    
    self.action:stop()
    assertEquals(self.action:isDone(), true) -- TODO:Failed.Bug?
end

---
-- test: start,pause,stop
function TestMOAIAction:test_start_stop_pause()
    assertEquals(self.action:isActive(), false)
    assertEquals(self.action:isBusy(), false)
    assertEquals(self.action:isDone(), true)
    
    local color = MOAIColor.new()
    local child = color:seekColor(0, 0, 0, 0, 4)
    local action = nil
    local stoped = false
    self.action:addChild(child)
    self.action:setListener(MOAIAction.EVENT_STOP, function()
        stoped = true
    end)
    
    assertEquals(self.action:isActive(), false)
    assertEquals(self.action:isBusy(), false)
    assertEquals(self.action:isDone(), false)
    
    action = self.action:start()
    assertEquals(action, self.action)
    assertEquals(self.action:isActive(), true)
    assertEquals(self.action:isBusy(), true)
    assertEquals(self.action:isDone(), false)
    
    action = self.action:pause()
    assertEquals(action, nil)
    assertEquals(self.action:isActive(), true)
    assertEquals(self.action:isBusy(), true)
    assertEquals(self.action:isDone(), false)
    
    action = self.action:pause(true)
    assertEquals(action, nil)
    assertEquals(self.action:isActive(), true)
    assertEquals(self.action:isBusy(), true)
    assertEquals(self.action:isDone(), false)
    
    action = self.action:stop()
    assertEquals(stoped, true)
    assertEquals(action, self.action)
    assertEquals(self.action:isActive(), false)
    assertEquals(self.action:isBusy(), false)
    assertEquals(self.action:isDone(), true)  -- TODO:Bug?
    
    action = self.action:clear()
    assertEquals(action, self.action)
    assertEquals(self.action:isActive(), false)
    assertEquals(self.action:isBusy(), false)
    assertEquals(self.action:isDone(), true)
    
    action = self.action:start()
    assertEquals(action, self.action)
    assertEquals(self.action:isActive(), true)
    assertEquals(self.action:isBusy(), true) -- TODO:Bug?
    assertEquals(self.action:isDone(), false)
    
end

---
-- test: throttle
function TestMOAIAction:test_throttle()
    self.action:throttle()
    self.action:throttle(2)
    self.action:throttle(0.5)
end

