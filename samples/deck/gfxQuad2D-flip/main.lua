----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc.
-- All Rights Reserved.
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 640, 960 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 960 )
viewport:setScale ( 640, -960 )
viewport:setOffset(-1, 1)

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

deck1 = MOAIGfxQuad2D.new ()
deck1:setTexture ( "moai.png" )
deck1:setRect ( 0, 0, 128, 128 )
deck1:setUVRect ( 0, 0, 1, 1 )

deck2 = MOAIGfxQuad2D.new ()
deck2:setTexture ( "moai.png" )
deck2:setRect ( 0, 0, 128, 128 )
deck2:setUVRect ( 1, 0, 0, 1 )

deck3 = MOAIGfxQuad2D.new ()
deck3:setTexture ( "moai.png" )
deck3:setRect ( 0, 0, 128, 128 )
deck3:setUVRect ( 1, 1, 0, 0 )

deck4 = MOAIGfxQuad2D.new ()
deck4:setTexture ( "moai.png" )
deck4:setRect ( 0, 0, 128, 128 )
deck4:setUVRect ( 0, 1, 1, 0 )

prop1 = MOAIProp2D.new ()
prop1:setLoc(0, 0)
prop1:setPiv(64, 64)
prop1:setColor(1, 1, 1, 1)
prop1:setDeck ( deck1 )
prop1:moveLoc(100, 100, 3)

prop2 = MOAIProp2D.new ()
prop2:setLoc(150, 0)
prop2:setPiv(64, 64)
prop2:setColor(1, 0, 0, 1)
prop2:setDeck ( deck2 )
prop2:moveLoc(100, 100, 3)

prop3 = MOAIProp2D.new ()
prop3:setLoc(0, 150)
prop3:setPiv(64, 64)
prop3:setColor(0, 1, 0, 1)
prop3:setDeck ( deck3 )
prop3:moveLoc(100, 100, 3)

prop4 = MOAIProp2D.new ()
prop4:setLoc(150, 150)
prop4:setPiv(64, 64)
prop4:setColor(0, 0, 1, 1)
prop4:setDeck ( deck4 )
prop4:moveLoc(100, 100, 3)



layer:insertProp ( prop1 )
layer:insertProp ( prop2 )
layer:insertProp ( prop3 )
layer:insertProp ( prop4 )
