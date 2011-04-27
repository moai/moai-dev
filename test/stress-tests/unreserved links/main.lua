----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 960, 640 )
viewport:setScale ( 960, 640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

tiles = MOAITileDeck2D.new ()
tiles:setTexture ( "../test.png" )
tiles:setSize ( 2, 2 )
tiles:setRect ( -32, -32, 32, 32 )

prop = MOAIProp2D.new ()
prop:setDeck ( tiles )
layer:insertProp ( prop )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 20 )
for i = 1, 5 do
	curve:setKey ( 1+(4*i-1), (i-1)+0, 5, MOAIEaseType.FLAT )
	curve:setKey ( 2+(4*i-1), (i-1)+0.25, 1, MOAIEaseType.FLAT )
	curve:setKey ( 3+(4*i-1), (i-1)+0.5, 2, MOAIEaseType.FLAT )
	curve:setKey ( 4+(4*i-1), (i-1)+0.75, 3, MOAIEaseType.FLAT )
end

anim = MOAIAnim.new ()
-- anim:reserveLinks ( 3 )
anim:setLink ( 1, curve, prop, MOAIProp2D.ATTR_INDEX )
anim:setLink ( 2, curve, prop, MOAIProp2D.ATTR_X_SCL )
anim:setLink ( 3, curve, prop, MOAIProp2D.ATTR_Y_SCL )
anim:setMode ( MOAITimer.LOOP )
anim:start ()

MOAISim.openWindow ( "stress test", 960, 640 )
