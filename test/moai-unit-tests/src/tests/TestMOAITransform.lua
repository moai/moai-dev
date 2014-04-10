
-- test case
local TestMOAITransform = {}
_G.TestMOAITransform = TestMOAITransform

---
-- setUp
function TestMOAITransform:setUp()
    self.transform = MOAITransform.new()
end

---
-- tearDown
function TestMOAITransform:tearDown()

end

---
-- test: addLoc
function TestMOAITransform:test_addLoc()
    local x, y, z = self.transform:getLoc()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:addLoc(10, 20, 30)
    x, y, z = self.transform:getLoc()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)

    self.transform:addLoc(10, 20, 30)
    x, y, z = self.transform:getLoc()
    assertEquals(x, 20)
    assertEquals(y, 40)
    assertEquals(z, 60)
end

---
-- test: addPiv
function TestMOAITransform:test_addPiv()
    local x, y, z = self.transform:getPiv()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:addPiv(10, 20, 30)
    x, y, z = self.transform:getPiv()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)

    self.transform:addPiv(10, 20, 30)
    x, y, z = self.transform:getPiv()
    assertEquals(x, 20)
    assertEquals(y, 40)
    assertEquals(z, 60)
end

---
-- test: addRot
function TestMOAITransform:test_addRot()
    local x, y, z = self.transform:getRot()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:addRot(10, 20, 30)
    x, y, z = self.transform:getRot()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)

    self.transform:addRot(10, 20, 30)
    x, y, z = self.transform:getRot()
    assertEquals(x, 20)
    assertEquals(y, 40)
    assertEquals(z, 60)

    self.transform:addRot(400, 400, 400)
    x, y, z = self.transform:getRot()
    assertEquals(x, 420)
    assertEquals(y, 440)
    assertEquals(z, 460)
end

---
-- test: addScl
function TestMOAITransform:test_addScl()
    local x, y, z = self.transform:getScl()
    assertEquals(x, 1)
    assertEquals(y, 1)
    assertEquals(z, 1)

    self.transform:addScl(1, 2, 3)
    x, y, z = self.transform:getScl()
    assertEquals(x, 2)
    assertEquals(y, 3)
    assertEquals(z, 4)

    self.transform:addScl(2, 3, 4)
    x, y, z = self.transform:getScl()
    assertEquals(x, 4)
    assertEquals(y, 6)
    assertEquals(z, 8)
end

---
-- test: modelToWorld
function TestMOAITransform:test_modelToWorld()
    local wx, wy, wz = self.transform:modelToWorld()
    assertEquals(wx, 0)
    assertEquals(wy, 0)
    assertEquals(wz, 0)
    
    wx, wy, wz = self.transform:modelToWorld(10, 20, 30)
    assertEquals(wx, 10)
    assertEquals(wy, 20)
    assertEquals(wz, 30)
    
    self.transform:setLoc(100, 200, 300)
    wx, wy, wz = self.transform:modelToWorld(10, 20, 30)
    assertEquals(wx, 10)
    assertEquals(wy, 20)
    assertEquals(wz, 30)
    
    -- Is not reflected only after Update. Troublesome.
    self.transform:forceUpdate()
    wx, wy, wz = self.transform:modelToWorld(10, 20, 30)
    assertEquals(wx, 110)
    assertEquals(wy, 220)
    assertEquals(wz, 330)
end

---
-- test: move
function TestMOAITransform:test_move()
    local action = self.transform:move(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    assertTrue(action ~= nil)
    
    local action = self.transform:move(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: moveLoc
function TestMOAITransform:test_moveLoc()
    local action = self.transform:moveLoc(1, 2, 3, 4)
    assertTrue(action ~= nil)
    
    local action = self.transform:moveLoc(1, 2, 3, 4, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: movePiv
function TestMOAITransform:test_movePiv()
    local action = self.transform:movePiv(1, 2, 3, 4)
    assertTrue(action ~= nil)
    
    local action = self.transform:movePiv(1, 2, 3, 4, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: moveRot
function TestMOAITransform:test_moveRot()
    local action = self.transform:moveRot(1, 2, 3, 4)
    assertTrue(action ~= nil)
    
    local action = self.transform:moveRot(1, 2, 3, 4, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: moveScl
function TestMOAITransform:test_moveScl()
    local action = self.transform:moveScl(1, 2, 3, 4)
    assertTrue(action ~= nil)
    
    local action = self.transform:moveScl(1, 2, 3, 4, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: seek
function TestMOAITransform:test_seek()
    local action = self.transform:seek(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    assertTrue(action ~= nil)
    
    local action = self.transform:seek(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: seekLoc
function TestMOAITransform:test_seekLoc()
    local action = self.transform:seekLoc(1, 2, 3, 4)
    assertTrue(action ~= nil)
    
    local action = self.transform:seekLoc(1, 2, 3, 4, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: seekPiv
function TestMOAITransform:test_seekPiv()
    local action = self.transform:seekPiv(1, 2, 3, 4)
    assertTrue(action ~= nil)
    
    local action = self.transform:seekPiv(1, 2, 3, 4, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: seekRot
function TestMOAITransform:test_seekRot()
    local action = self.transform:seekRot(1, 2, 3, 4)
    assertTrue(action ~= nil)
    
    local action = self.transform:seekRot(1, 2, 3, 4, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: seekScl
function TestMOAITransform:test_seekScl()
    local action = self.transform:seekScl(1, 2, 3, 4)
    assertTrue(action ~= nil)
    
    local action = self.transform:seekScl(1, 2, 3, 4, MOAIEaseType.EASE_IN)
    assertTrue(action ~= nil)
end

---
-- test: getLoc
function TestMOAITransform:test_getLoc()
    local x, y, z = self.transform:getLoc()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:setLoc(10, 20, 30)
    x, y, z = self.transform:getLoc()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)
end

---
-- test: getPiv
function TestMOAITransform:test_getPiv()
    local x, y, z = self.transform:getPiv()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:setPiv(10, 20, 30)
    x, y, z = self.transform:getPiv()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)
end

---
-- test: getRot
function TestMOAITransform:test_getRot()
    local x, y, z = self.transform:getRot()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:setRot(10, 20, 30)
    x, y, z = self.transform:getRot()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)
end

---
-- test: getScl
function TestMOAITransform:test_getScl()
    local x, y, z = self.transform:getScl()
    assertEquals(x, 1)
    assertEquals(y, 1)
    assertEquals(z, 1)

    self.transform:setScl(10, 20, 30)
    x, y, z = self.transform:getScl()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)
end


---
-- test: setLoc
function TestMOAITransform:test_setLoc()
    local x, y, z = self.transform:getLoc()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:setLoc(10, 20, 30)
    x, y, z = self.transform:getLoc()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)
end

---
-- test: setPiv
function TestMOAITransform:test_setPiv()
    local x, y, z = self.transform:getPiv()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:setPiv(10, 20, 30)
    x, y, z = self.transform:getPiv()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)
end

---
-- test: setRot
function TestMOAITransform:test_setRot()
    local x, y, z = self.transform:getRot()
    assertEquals(x, 0)
    assertEquals(y, 0)
    assertEquals(z, 0)

    self.transform:setRot(10, 20, 30)
    x, y, z = self.transform:getRot()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)
end

---
-- test: setScl
function TestMOAITransform:test_setScl()
    local x, y, z = self.transform:getScl()
    assertEquals(x, 1)
    assertEquals(y, 1)
    assertEquals(z, 1)

    self.transform:setScl(10, 20, 30)
    x, y, z = self.transform:getScl()
    assertEquals(x, 10)
    assertEquals(y, 20)
    assertEquals(z, 30)
end

---
-- test: modelToWorld
function TestMOAITransform:test_worldToModel()
    local wx, wy, wz = self.transform:worldToModel()
    assertEquals(wx, 0)
    assertEquals(wy, 0)
    assertEquals(wz, 0)
    
    wx, wy, wz = self.transform:worldToModel(10, 20, 30)
    assertEquals(wx, 10)
    assertEquals(wy, 20)
    assertEquals(wz, 30)
    
    self.transform:setLoc(100, 200, 300)
    wx, wy, wz = self.transform:worldToModel(10, 20, 30)
    assertEquals(wx, 10)
    assertEquals(wy, 20)
    assertEquals(wz, 30)
    
    -- Is not reflected only after Update. Troublesome.
    self.transform:forceUpdate()
    wx, wy, wz = self.transform:worldToModel(10, 20, 30)
    assertEquals(wx, -90)
    assertEquals(wy, -180)
    assertEquals(wz, -270)
end

