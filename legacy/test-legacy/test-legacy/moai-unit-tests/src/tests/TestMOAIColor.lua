
-- test case
local TestMOAIColor = {}
_G.TestMOAIColor = TestMOAIColor

---
-- setUp
function TestMOAIColor:setUp()
    self.color = MOAIColor.new()
end

---
-- tearDown
function TestMOAIColor:tearDown()

end

---
-- test: getColor
function TestMOAIColor:test_getColor()
    local r, g, b, a = self.color:getColor()
    
    assertEquals(r, 1)
    assertEquals(g, 1)
    assertEquals(b, 1)
    assertEquals(a, 1)
    
    self.color:setColor(1/256, 2/256, 3/256, 4/256)
    r, g, b, a = self.color:getColor()
    assertEquals(r, 1/256)
    assertEquals(g, 2/256)
    assertEquals(b, 3/256)
    assertEquals(a, 4/256)
end

---
-- test: setColor
function TestMOAIColor:test_setColor()
    local r = self.color:getAttr(MOAIColor.ATTR_R_COL)
    local g = self.color:getAttr(MOAIColor.ATTR_G_COL)
    local b = self.color:getAttr(MOAIColor.ATTR_B_COL)
    local a = self.color:getAttr(MOAIColor.ATTR_A_COL)
    
    assertEquals(r, 1)
    assertEquals(g, 1)
    assertEquals(b, 1)
    assertEquals(a, 1)
    
    self.color:setColor(1/256, 2/256, 3/256, 4/256)
    r = self.color:getAttr(MOAIColor.ATTR_R_COL)
    g = self.color:getAttr(MOAIColor.ATTR_G_COL)
    b = self.color:getAttr(MOAIColor.ATTR_B_COL)
    a = self.color:getAttr(MOAIColor.ATTR_A_COL)
    assertEquals(r, 1/256)
    assertEquals(g, 2/256)
    assertEquals(b, 3/256)
    assertEquals(a, 4/256)
end

---
-- test: moveColor
function TestMOAIColor:test_moveColor()
    local action = self.color:moveColor(1/256, 2/256, 3/256, 4/256, 3, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: moveColor
function TestMOAIColor:test_seekColor()
    local action = self.color:moveColor(1/256, 2/256, 3/256, 4/256, 3, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end
