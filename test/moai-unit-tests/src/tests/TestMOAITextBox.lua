
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

