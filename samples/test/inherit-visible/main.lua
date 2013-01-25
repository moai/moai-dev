----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setLoc(-10,10)
layer:insertProp ( prop )


prop1 = MOAIProp2D.new ()
prop1:setDeck ( gfxQuad )
prop1:setLoc(10,-10)

layer:insertProp ( prop1 )

print("(child) prop1.visible  <- prop.visible (parent)")
prop1:setAttrLink(MOAIProp.INHERIT_VISIBLE, prop, MOAIProp.ACTUAL_VISIBLE)
print(prop1:getAttr(MOAIProp.ACTUAL_VISIBLE), prop:getAttr(MOAIProp.ATTR_VISIBLE))

print('prop.visible=false')
prop:setVisible(false)
prop:forceUpdate()
prop1:forceUpdate()
print(prop1:getAttr(MOAIProp.ACTUAL_VISIBLE), prop:getAttr(MOAIProp.ATTR_VISIBLE))

print('prop.visible=true')
prop:setVisible(true)
prop:forceUpdate()
prop1:forceUpdate()
print(prop1:getAttr(MOAIProp.ACTUAL_VISIBLE), prop:getAttr(MOAIProp.ATTR_VISIBLE))

print('prop.visible=true prop1.visible=false')
prop:setVisible(true)
prop1:setVisible(false)
prop:forceUpdate()
prop1:forceUpdate()
print(prop1:getAttr(MOAIProp.ACTUAL_VISIBLE), prop:getAttr(MOAIProp.ATTR_VISIBLE))


print('prop.visible=false prop1.visible=false')
prop:setVisible(false)
prop1:setVisible(false)
prop:forceUpdate()
prop1:forceUpdate()
print(prop1:getAttr(MOAIProp.ACTUAL_VISIBLE), prop:getAttr(MOAIProp.ATTR_VISIBLE))
