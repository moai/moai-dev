
-- test case
local TestMOAIAnim = {}
_G.TestMOAIAnim = TestMOAIAnim

---
-- setUp
function TestMOAIAnim:setUp()
    self.prop = MOAIProp.new()
    
    self.animCurve1 = MOAIAnimCurve.new()
    self.animCurve1:reserveKeys ( 3 )
    self.animCurve1:setKey ( 1, 0.00, 1, MOAIEaseType.FLAT )
    self.animCurve1:setKey ( 2, 0.25, 2, MOAIEaseType.FLAT )
    self.animCurve1:setKey ( 3, 0.50, 3, MOAIEaseType.FLAT )

    self.anim = MOAIAnim.new()
    self.anim:reserveLinks ( 1 )
    self.anim:setLink ( 1, self.animCurve1, self.prop, MOAIProp2D.ATTR_INDEX )
    self.anim:setMode ( MOAITimer.LOOP )
end

---
-- tearDown
function TestMOAIAnim:tearDown()
end

---
-- test: apply
function TestMOAIAnim:test_apply()
    self.anim:apply(0.25)
    assertEquals(self.prop:getIndex(), 2)
end

---
-- test: getLength
function TestMOAIAnim:test_getLength()
    assertEquals(self.anim:getLength(), 0.5)
end

---
-- test: reserveLinks
function TestMOAIAnim:test_reserveLinks()
    self.anim:reserveLinks(2)
    self.anim:reserveLinks(0)
    self.anim:reserveLinks(1)
end

---
-- test: setLink
function TestMOAIAnim:test_setLink()
    self.anim:reserveLinks(2)
    self.anim:setLink ( 1, self.animCurve1, self.prop, MOAIProp2D.ATTR_INDEX )
    self.anim:setLink ( 2, self.animCurve1, self.prop, MOAIProp2D.ATTR_INDEX )
end


