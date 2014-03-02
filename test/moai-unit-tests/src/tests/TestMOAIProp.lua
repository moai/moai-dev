
-- test case
local TestMOAIProp = {}
_G.TestMOAIProp = TestMOAIProp

---
-- setUp
function TestMOAIProp:setUp()
    self.deck = MOAIGfxQuad2D.new()
    self.deck:setUVRect(0, 0, 1, 1)
    self.deck:setRect(0, 0, 100, 120)
    self.deck:setTexture("assets/cathead.png")

    self.prop = MOAIProp.new()
    self.prop:setDeck(self.deck)
    
    self.layer = MOAILayer.new()
    self.layer:insertProp(self.prop)
end

---
-- tearDown
function TestMOAIProp:tearDown()

end

---
-- test: const
function TestMOAIProp:test_const()
    
    for key, value in pairs(MOAIProp) do
        print("key=" .. key, "value=" .. tostring(value))
    end

    assertTrue(MOAIProp.ATTR_INDEX ~= nil)
    assertTrue(MOAIProp.ATTR_PARTITION ~= nil)
    assertTrue(MOAIProp.ATTR_SHADER ~= nil)
    assertTrue(MOAIProp.ATTR_BLEND_MODE ~= nil)
    assertTrue(MOAIProp.ATTR_VISIBLE ~= nil)
    assertTrue(MOAIProp.ATTR_LOCAL_VISIBLE ~= nil)
    assertTrue(MOAIProp.ATTR_VISIBLE ~= nil)
    assertTrue(MOAIProp.INHERIT_VISIBLE ~= nil)
    assertTrue(MOAIProp.INHERIT_FRAME ~= nil)
    assertTrue(MOAIProp.FRAME_TRAIT ~= nil)
    assertTrue(MOAIProp.BLEND_ADD ~= nil)
    assertTrue(MOAIProp.BLEND_MULTIPLY ~= nil)
    assertTrue(MOAIProp.BLEND_NORMAL ~= nil)
    assertTrue(MOAIProp.GL_FUNC_ADD ~= nil)
    assertTrue(MOAIProp.GL_FUNC_SUBTRACT ~= nil)
    assertTrue(MOAIProp.GL_FUNC_REVERSE_SUBTRACT ~= nil)
    assertTrue(MOAIProp.GL_ONE ~= nil)
    assertTrue(MOAIProp.GL_ZERO ~= nil)
    assertTrue(MOAIProp.GL_DST_ALPHA ~= nil)
    assertTrue(MOAIProp.GL_DST_COLOR ~= nil)
    assertTrue(MOAIProp.GL_SRC_COLOR ~= nil)
    assertTrue(MOAIProp.GL_ONE_MINUS_DST_ALPHA ~= nil)
    assertTrue(MOAIProp.GL_ONE_MINUS_DST_COLOR ~= nil)
    assertTrue(MOAIProp.GL_ONE_MINUS_SRC_ALPHA ~= nil)
    assertTrue(MOAIProp.GL_ONE_MINUS_SRC_COLOR ~= nil)
    assertTrue(MOAIProp.GL_SRC_ALPHA ~= nil)
    assertTrue(MOAIProp.GL_SRC_ALPHA_SATURATE ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_DISABLE ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_NEVER ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_LESS ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_EQUAL ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_LESS_EQUAL ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_GREATER ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_NOTEQUAL ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_GREATER_EQUAL ~= nil)
    assertTrue(MOAIProp.DEPTH_TEST_ALWAYS ~= nil)
    assertTrue(MOAIProp.CULL_NONE ~= nil)
    assertTrue(MOAIProp.CULL_ALL ~= nil)
    assertTrue(MOAIProp.CULL_BACK ~= nil)
    assertTrue(MOAIProp.CULL_FRONT ~= nil)
    
end

---
-- test: getBounds
function TestMOAIProp:test_getBounds()
    local xMin, yMin, zMin, xMax, yMax, zMax = self.prop:getBounds()
    assertEquals(xMin, 0)
    assertEquals(yMin, 0)
    assertEquals(zMin, 0)
    assertEquals(xMax, 100)
    assertEquals(yMax, 120)
    assertEquals(zMax, 0)

    self.prop:setBounds(10, 20, 30, 40, 50, 60)
    xMin, yMin, zMin, xMax, yMax, zMax = self.prop:getBounds()
    assertEquals(xMin, 10)
    assertEquals(yMin, 20)
    assertEquals(zMin, 30)
    assertEquals(xMax, 40)
    assertEquals(yMax, 50)
    assertEquals(zMax, 60)
end

---
-- test: getDims
function TestMOAIProp:test_getDims()
    local width, height, depth = self.prop:getDims()
    assertEquals(width, 100)
    assertEquals(height, 120)
    assertEquals(depth, 0)
    
    self.prop:setBounds(10, 20, 30, 40, 50, 60)
    width, height, depth = self.prop:getDims()
    assertEquals(width, 30)
    assertEquals(height, 30)
    assertEquals(depth, 30)
end

---
-- test: getGrid
function TestMOAIProp:test_getGrid()
    local grid = self.prop:getGrid()
    assertIsNil(grid)
    
    grid = MOAIGrid.new()
    self.prop:setGrid(grid)
    grid = self.prop:getGrid()
    assertTrue(grid ~= nil)
end

---
-- test: getIndex
function TestMOAIProp:test_getIndex()
    local index = self.prop:getIndex()
    assertEquals(index, 1)

    self.prop:setIndex(10)
    index = self.prop:getIndex()
    assertEquals(index, 10) 
end

---
-- test: getPriority
function TestMOAIProp:test_getPriority()
    local priority = self.prop:getPriority()
    assertEquals(priority, 0)

    self.prop:setPriority(10)
    priority = self.prop:getPriority()
    assertEquals(priority, 10) 
end

---
-- test: getPriority
function TestMOAIProp:test_getWorldBounds()
    local xMin, yMin, zMin, xMax, yMax, zMax = self.prop:getWorldBounds()
    assertEquals(xMin, 0)
    assertEquals(yMin, 0)
    assertEquals(zMin, 0)
    assertEquals(xMax, 100)
    assertEquals(yMax, 120)
    assertEquals(zMax, 0)

    self.prop:setBounds(10, 20, 30, 40, 50, 60)
    xMin, yMin, zMin, xMax, yMax, zMax = self.prop:getWorldBounds()
    assertEquals(xMin, 10)
    assertEquals(yMin, 20)
    assertEquals(zMin, 30)
    assertEquals(xMax, 40)
    assertEquals(yMax, 50)
    assertEquals(zMax, 60)
end

---
-- test: isVisible
function TestMOAIProp:test_isVisible()
    local visible = self.prop:isVisible()
    assertEquals(visible, true)
    
    self.prop:setVisible(false)
    visible = self.prop:isVisible()
    assertEquals(visible, false)
end

---
-- test: isVisible
function TestMOAIProp:test_inside()
    local visible = self.prop:isVisible()
    assertEquals(self.prop:inside(0, 0, 0), true)
    assertEquals(self.prop:inside(100, 0, 0), true)
    assertEquals(self.prop:inside(100, 120, 0), true)
    assertEquals(self.prop:inside(100, 121, 0), false)
    assertEquals(self.prop:inside(-1, -1, -1, 1), true)
    assertEquals(self.prop:inside(0, 0, 0, -1), false)
    assertEquals(self.prop:inside(1, 1, 0, -1), true)
    assertEquals(self.prop:inside(110, 130, 0, 10), true)
    assertEquals(self.prop:inside(101, 121, 0, 1), true)
    assertEquals(self.prop:inside(102, 122, 0, 1), false)
    
end

---
-- test: setBillboard
function TestMOAIProp:test_setBillboard()
    self.prop:setBillboard(true)
    self.prop:setBillboard(false)
end

---
-- test: setBlendEquation
function TestMOAIProp:test_setBlendEquation()
    self.prop:setBlendEquation(MOAIProp.GL_FUNC_ADD)
    self.prop:setBlendEquation(MOAIProp.GL_FUNC_SUBTRACT)
    self.prop:setBlendEquation(MOAIProp.GL_FUNC_REVERSE_SUBTRACT)
    self.prop:setBlendEquation(nil)
end

---
-- test: setBlendMode
function TestMOAIProp:test_setBlendMode()
    self.prop:setBlendMode(MOAIProp.BLEND_NORMAL)
    self.prop:setBlendMode(MOAIProp.BLEND_ADD)
    self.prop:setBlendMode(MOAIProp.BLEND_MULTIPLY)
    self.prop:setBlendMode(nil)
end

---
-- test: setBounds
function TestMOAIProp:test_setBounds()
    local xMin, yMin, zMin, xMax, yMax, zMax = self.prop:getBounds()
    assertEquals(xMin, 0)
    assertEquals(yMin, 0)
    assertEquals(zMin, 0)
    assertEquals(xMax, 100)
    assertEquals(yMax, 120)
    assertEquals(zMax, 0)

    self.prop:setBounds(10, 20, 30, 40, 50, 60)
    xMin, yMin, zMin, xMax, yMax, zMax = self.prop:getBounds()
    assertEquals(xMin, 10)
    assertEquals(yMin, 20)
    assertEquals(zMin, 30)
    assertEquals(xMax, 40)
    assertEquals(yMax, 50)
    assertEquals(zMax, 60)
    
    self.prop:setBounds()
    xMin, yMin, zMin, xMax, yMax, zMax = self.prop:getBounds()
    assertEquals(xMin, 0)
    assertEquals(yMin, 0)
    assertEquals(zMin, 0)
    assertEquals(xMax, 100)
    assertEquals(yMax, 120)
    assertEquals(zMax, 0)
end

---
-- test: setCullMode
function TestMOAIProp:test_setCullMode()
    self.prop:setCullMode(MOAIProp.CULL_NONE)
    self.prop:setCullMode(MOAIProp.CULL_ALL)
    self.prop:setCullMode(MOAIProp.CULL_BACK)
    self.prop:setCullMode(MOAIProp.CULL_FRONT)
end

---
-- test: setColor
function TestMOAIProp:test_setColor()
    local r = self.prop:getAttr(MOAIColor.ATTR_R_COL)
    local g = self.prop:getAttr(MOAIColor.ATTR_G_COL)
    local b = self.prop:getAttr(MOAIColor.ATTR_B_COL)
    local a = self.prop:getAttr(MOAIColor.ATTR_A_COL)
    
    assertEquals(r, 1)
    assertEquals(g, 1)
    assertEquals(b, 1)
    assertEquals(a, 1)
    
    self.prop:setColor(1/256, 2/256, 3/256, 4/256)
    r = self.prop:getAttr(MOAIColor.ATTR_R_COL)
    g = self.prop:getAttr(MOAIColor.ATTR_G_COL)
    b = self.prop:getAttr(MOAIColor.ATTR_B_COL)
    a = self.prop:getAttr(MOAIColor.ATTR_A_COL)
    assertEquals(r, 1/256)
    assertEquals(g, 2/256)
    assertEquals(b, 3/256)
    assertEquals(a, 4/256)
end

