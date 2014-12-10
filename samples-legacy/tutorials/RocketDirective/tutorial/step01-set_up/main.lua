--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

--==============================================================
-- set up
--==============================================================
print ( "hello, moai!" )

MOAISim.openWindow ( "Rocket Lobster", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setScale ( 320, 480 )
viewport:setSize ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )