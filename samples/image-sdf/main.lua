----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIGfxDevice.getFrameBuffer ():setClearColor ( 1, 1, 1, 1 )

image = MOAIImage.new ()
image:load ( "../resources/moai-alpha.png" )

width, height = image:getSize ()
hWidth, hHeight = width * 0.5, height * 0.5

image:generateSDFAA ( 0, 0, width, height, 16 )

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )

program:load ( MOAIFileSystem.loadFile ( 'shader.vsh' ), MOAIFileSystem.loadFile ( 'shader.fsh' ))

shader = MOAIShader.new ()
shader:setProgram ( program )

MOAISim.openWindow ( "test", width, height )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale ( width, -height )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

texture = MOAITexture.new ()
texture:load ( image )
texture:setFilter ( MOAITexture.GL_LINEAR )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setShader ( shader )
gfxQuad:setRect ( -hWidth, -hHeight, hWidth, hHeight )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )


