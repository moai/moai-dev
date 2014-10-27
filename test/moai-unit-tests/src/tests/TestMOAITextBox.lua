
-- test case
local TestMOAITextBox = {}
_G.TestMOAITextBox = TestMOAITextBox

---
-- setUp
function TestMOAITextBox:setUp()
    self.font = MOAIFont.new()
    self.font:load( 'assets/arial-rounded.TTF')

    self.textbox = MOAITextBox.new()
    self.textbox:setString("Test Text String")
    self.textbox:setFont(self.font)
    self.textbox:setTextSize(7.5, 163)
    self.textbox:setRect(0, 0, 100, 200)
    self.textbox:setAlignment(MOAITextBox.LEFT_JUSTIFY)
    self.textbox:setYFlip(true)
    
    self.layer = MOAILayer.new()
    self.layer:insertProp(self.textbox)
end

---
-- tearDown
function TestMOAITextBox:tearDown()

end

---
-- test: getString
function TestMOAITextBox:test_getString()
    local text = self.textbox:getString()
    assertEquals(text, "Test Text String")
end

---
-- test: getBounds
function TestMOAITextBox:test_getBounds()
    local xMin, yMin, zMin, xMax, yMax, zMax = self.textbox:getBounds()
    assertEquals(xMin, 0)
    assertEquals(yMin, 0)
    assertEquals(zMin, 0)
    assertEquals(xMax, 100)
    assertEquals(yMax, 200)
    assertEquals(zMax, 0)

    self.textbox:setRect(10, 20, 30, 40)
    xMin, yMin, zMin, xMax, yMax, zMax = self.textbox:getBounds()
    assertEquals(xMin, 10)
    assertEquals(yMin, 20)
    assertEquals(zMin, 0)
    assertEquals(xMax, 30)
    assertEquals(yMax, 40)
    assertEquals(zMax, 0)
end

---
-- test: getBounds
function TestMOAITextBox:test_getRect()
    local xMin, yMin, xMax, yMax = self.textbox:getRect()
    assertEquals(xMin, 0)
    assertEquals(yMin, 0)
    assertEquals(xMax, 100)
    assertEquals(yMax, 200)

    self.textbox:setRect(10, 20, 30, 40)
    xMin, yMin, xMax, yMax = self.textbox:getRect()
    assertEquals(xMin, 10)
    assertEquals(yMin, 20)
    assertEquals(xMax, 30)
    assertEquals(yMax, 40)
end

---
-- test: getDims
function TestMOAITextBox:test_getDims()
    local width, height, depth = self.textbox:getDims()
    assertEquals(width, 100)
    assertEquals(height, 200)
    assertEquals(depth, 0)
    
    self.textbox:setRect(10, 20, 30, 40)
    width, height, depth = self.textbox:getDims()
    assertEquals(width, 20)
    assertEquals(height, 20)
    assertEquals(depth, 0)
end

