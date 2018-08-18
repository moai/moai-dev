
-- test case
local TestMOAIPartition = {}
_G.TestMOAIPartition = TestMOAIPartition

function TestMOAIPartition:setUp()
    self.prop1 = MOAIProp.new()
    self.prop1:setLoc(0, 0, 0)
    self.prop1:setBounds(0, 0, 0, 30, 40, 60)
    self.prop1:setPriority(20)
    self.prop1:forceUpdate()

    self.prop2 = MOAIProp.new()
    self.prop2:setLoc(60, 80, 100)
    self.prop2:setBounds(0, 0, 0, 30, 40, 60)
    self.prop2:setPriority(1)
    self.prop2:forceUpdate()

    self.prop3 = MOAIProp.new()
    self.prop3:setLoc(30, 40, 50)
    self.prop3:setBounds(0, 0, 0, 30, 40, 60)
    self.prop3:setPriority(12)
    self.prop3:forceUpdate()

    self.partition = MOAIPartition.new()
    self.partition:insertProp(self.prop1)
    self.partition:insertProp(self.prop2)
    self.partition:insertProp(self.prop3)

    self.layer = MOAILayer.new()
    self.layer:setPartition(self.partition)
end

function TestMOAIPartition:tearDown()

end

---
-- test: propList
function TestMOAIPartition:test_propList()
    local props = {self.partition:propList()}
    assertEquals(props[1], self.prop1)
    assertEquals(props[2], self.prop2)
    assertEquals(props[3], self.prop3)

    props = {self.partition:propList(MOAILayer.SORT_PRIORITY_ASCENDING)}
    assertEquals(props[1], self.prop2)
    assertEquals(props[2], self.prop3)
    assertEquals(props[3], self.prop1)

    props = {self.partition:propList(MOAILayer.SORT_X_ASCENDING)}
    assertEquals(props[1], self.prop1)
    assertEquals(props[2], self.prop3)
    assertEquals(props[3], self.prop2)
end

