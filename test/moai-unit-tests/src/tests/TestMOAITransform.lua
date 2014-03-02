
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

