
-- test case
local TestMOAIScissorRect = {}
_G.TestMOAIScissorRect = TestMOAIScissorRect

---
-- setUp
function TestMOAIScissorRect:setUp()
    self.scissorRect = MOAIScissorRect.new()
end

---
-- tearDown
function TestMOAIScissorRect:tearDown()

end

---
-- test: setRect
function TestMOAIScissorRect:test_setRect()
    local xMin, yMin, xMax, yMax = self.scissorRect:getRect()
    assertEquals(xMin, 0)
    assertEquals(yMin, 0)
    assertEquals(xMax, 0)
    assertEquals(yMax, 0)

    self.scissorRect:setRect(-1, -2, 3, 4)
    xMin, yMin, xMax, yMax = self.scissorRect:getRect()
    assertEquals(xMin, -1)
    assertEquals(yMin, -2)
    assertEquals(xMax, 3)
    assertEquals(yMax, 4)

    -- test invalid rect (xMin > xMax)
    -- scissorRect can be invalid
    self.scissorRect:setRect(10, 0, -3, 5)
    xMin, yMin, xMax, yMax = self.scissorRect:getRect()
    assertEquals(xMin, 10)
    assertEquals(yMin, 0)
    assertEquals(xMax, -3)
    assertEquals(yMax, 5)
end

---
-- test: setScissorRect
function TestMOAIScissorRect:test_setScissorRect()
    local parent = MOAIScissorRect.new()

    self.scissorRect:setScissorRect(parent)
    self.scissorRect:setScissorRect(nil)
end
