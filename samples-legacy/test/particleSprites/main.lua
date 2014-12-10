----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "cathead", 640, 480 )

----------------------------------------------------------------
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead_small.png" )
texture:setRect ( -8, -8, 8, 8 )

system = MOAIParticleSystem.new ()
system:reserveSprites ( 1000 )
system:setDeck ( texture )

for i=1, 1000 do

	x = math.random ( -320, 320 )
	y = math.random ( -240, 240 )
	system:pushSprite ( x, y )
end

layer:insertProp ( system )
