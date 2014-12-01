----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

xScale = 2
yScale = 0.5

srcImage = MOAIImage.new ()
srcImage:load ( "moai.png" )

w0, h0 = srcImage:getSize ()

w1 = w0 * xScale
h1 = h0 * yScale

image = MOAIImage.new ()
image:init ( w1, h1, srcImage:getFormat ())

image:copyRect ( srcImage,
	0, 0, w0, h0,
	0, 0, w1, h1,
	MOAIImage.FILTER_LINEAR
)

MOAISim.openWindow ( "test", w1, h1 )

viewport = MOAIViewport.new ()
viewport:setSize ( w1, h1 )
viewport:setScale ( w1, -h1 )

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
