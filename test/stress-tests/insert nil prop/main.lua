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

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "../test.png" )
texture:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( texture )

nilprop = MOAIProp2D.new ()
nilprop:setDeck ( texture )
nilprop:setLoc ( 128, 128 )
nilprop = nil

layer:insertProp ( prop )
layer:insertProp ( nilprop )

MOAISim.openWindow ( "stress test", 960, 640 )
