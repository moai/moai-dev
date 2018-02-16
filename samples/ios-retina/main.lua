----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

if MOAIEnvironment.osBrand ~= MOAIEnvironment.OS_BRAND_IOS then
	print ( 'iOS only sample - please run on an iOS device or simulator' )
	os.exit ()
end

print ( "hello, iPhone!" )

----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 960 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "maneki.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )

