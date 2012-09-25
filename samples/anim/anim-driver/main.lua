----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 640, 320 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 320 )
viewport:setScale ( 640, 320 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop1 = MOAIProp2D.new ()
prop1:setDeck ( gfxQuad )
layer:insertProp ( prop1 )

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad )
layer:insertProp ( prop2 )

curve1 = MOAIAnimCurve.new ()
curve1:reserveKeys ( 2 )
curve1:setKey ( 1, 0, 0 )
curve1:setKey ( 2, 1.5, 64 )
curve1:setWrapMode ( MOAIAnimCurve.APPEND )

curve2 = MOAIAnimCurve.new ()
curve2:reserveKeys ( 3 )
curve2:setKey ( 1, 0, 0 )
curve2:setKey ( 2, 0.75, 64 )
curve2:setKey ( 3, 1.5, 0 )
curve2:setWrapMode ( MOAIAnimCurve.WRAP )

anim = MOAIAnim.new ()
anim:reserveLinks ( 3 )
anim:setLink ( 1, curve1, prop1, MOAIProp2D.ATTR_X_LOC )
anim:setLink ( 2, curve2, prop1, MOAIProp2D.ATTR_Y_LOC )
anim:setLink ( 3, curve1, prop2, MOAIProp2D.ATTR_X_LOC, true )
anim:setMode ( MOAITimer.CONTINUE )
anim:start ()
