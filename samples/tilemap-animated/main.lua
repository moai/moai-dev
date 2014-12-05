----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 256, 256 )

layer = MOAILayer2D.new ()
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 256, 256 )
viewport:setScale ( 256, 256 )
layer:setViewport ( viewport )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "numbers.png" )
tileDeck:setSize ( 8, 8 )
tileDeck:setRect ( -0.5, 0.5, 0.5, -0.5 )

grid = MOAIGrid.new ()
grid:setSize ( 8, 8, 32, 32 )
grid:setRepeat ( true ) -- wrap the grid when drawing

grid:setRow ( 1, 	0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x20000001 )
grid:setRow ( 2, 	0x00000009, 0x00000001, 0x0000000b, 0x0000000c, 0x0000000d, 0x0000000e, 0x20000001, 0x00000010 )
grid:setRow ( 3, 	0x00000011, 0x00000012, 0x00000001, 0x00000014, 0x00000015, 0x20000001, 0x00000017, 0x00000018 )
grid:setRow ( 4, 	0x00000019, 0x0000001a, 0x0000001b, 0x00000001, 0x20000001, 0x0000001e, 0x0000001f, 0x00000020 )
grid:setRow ( 5, 	0x00000021, 0x00000022, 0x00000023, 0x40000001, 0x60000001, 0x00000026, 0x00000027, 0x00000028 )
grid:setRow ( 6, 	0x00000029, 0x0000002a, 0x40000001, 0x0000002c, 0x0000002d, 0x60000001, 0x0000002f, 0x00000030 )
grid:setRow ( 7, 	0x00000031, 0x40000001, 0x00000033, 0x00000034, 0x00000035, 0x00000036, 0x60000001, 0x00000038 )
grid:setRow ( 8, 	0x40000001, 0x0000003a, 0x0000003b, 0x0000003c, 0x0000003d, 0x0000003e, 0x0000003f, 0x60000001 )

remapper = MOAIDeckRemapper.new ()
remapper:reserve ( 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setRemapper ( remapper )
prop:setLoc ( -128, 128 )
prop:setScl ( 1, -1 )
layer:insertProp ( prop )

curve = MOAIAnimCurve.new ()

curve:reserveKeys ( 5 )
curve:setKey ( 1, 0.00, 1, MOAIEaseType.FLAT )
curve:setKey ( 2, 0.25, 2, MOAIEaseType.FLAT )
curve:setKey ( 3, 0.50, 3, MOAIEaseType.FLAT )
curve:setKey ( 4, 0.75, 4, MOAIEaseType.FLAT )
curve:setKey ( 5, 1.00, 1, MOAIEaseType.FLAT )

anim = MOAIAnim:new ()
anim:reserveLinks ( 1 )
anim:setLink ( 1, curve,remapper, 1 )
anim:setMode ( MOAITimer.LOOP )
anim:start ()
