----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 256, 256 )

viewport = MOAIViewport.new ()
viewport:setSize ( 256, 256 )
viewport:setScale ( 256, 256 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

image = MOAIImage.new ()
--image:init ( 256, 256, MOAIImage.COLOR_FMT_A_8 )
image:init ( 256, 256 )

reader = MOAIFreeTypeFontReader.new ()
reader:open ( 'arial-rounded.TTF' )
reader:selectFace ( 50 )
reader:selectGlyph ( 65 )
reader:setPenColor ( 1, 0, 0, 1 )
reader:enableAntiAliasing ( false )
reader:renderGlyph ( image, 128, 128 )

image:writePNG ( 'font.png' )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( image )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
--prop:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.FONT_SHADER ))
prop:setBlendMode ( MOAIGraphicsProp.GL_SRC_ALPHA, MOAIGraphicsProp.GL_ONE_MINUS_SRC_ALPHA );
layer:insertProp ( prop )
