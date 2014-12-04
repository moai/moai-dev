----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

xScale = 2
yScale = 2

image = MOAIImage.new ()
image:load ( "moai.png" )
image:simpleThreshold ( 0.5, 1, 1, 0 )

w0, h0 = image:getSize ()

w1 = w0 * xScale
h1 = h0 * yScale

local sW, sH = w1 * 1.5, h1 * 1.5

MOAISim.openWindow ( "test", sW, sH )

viewport = MOAIViewport.new ()
viewport:setSize ( sW, sH )
viewport:setScale ( sW, -sH )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

hw1 = w1 * 0.5
hh1 = h1 * 0.5

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( image )
gfxQuad:setRect ( -hw1, -hh1, hw1, hh1 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

