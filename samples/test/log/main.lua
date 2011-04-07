----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

-- type mismatch
gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setRect ( 1, 2, 'something' )

-- missing file
texture = MOAITexture.new ()
texture:load ( 'foo.png' )

-- create an object to test the out of range message
brushLib = MOAIGfxQuadDeck2D.new ()
brushLib:reserve ( 2 )
brushLib:setRect ( 0, 0, 0, 0, 0 ) -- error
brushLib:setRect ( 1, 0, 0, 0, 0 )
brushLib:setRect ( 2, 0, 0, 0, 0 )
brushLib:setRect ( 3, 0, 0, 0, 0 ) -- error

-- attributes
transform = MOAITransform.new ()
transform:getAttr ( 76 )

