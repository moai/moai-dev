----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

image = MOAIImage.new ()
image:load ( '../resources/moai-alpha.png' )

width, height = image:getSize ()
hWidth, hHeight = width * 0.5, height * 0.5

image:generateSDFAA ( 0, 0, width, height, 16 )

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )

program:reserveUniforms ( 4 )
program:declareUniformSampler ( 1, 'sdfSampler', 1 )
program:declareUniformSampler ( 2, 'rampSampler', 2 )
program:declareUniform ( 3, 'outlineColor', MOAIShaderProgram.UNIFORM_VECTOR_F4 )
program:declareUniform ( 4, 'glowColor', MOAIShaderProgram.UNIFORM_VECTOR_F4 )

program:load ( MOAIFileSystem.loadFile ( 'shader.vsh' ), MOAIFileSystem.loadFile ( 'shader.fsh' ))

shader = MOAIShader.new ()
shader:setProgram ( program )

outlineColor = MOAIColor.new ()
outlineColor:setColor ( 0.0, 1.0, 0.75, 1.0 )
shader:setAttrLink ( 3, outlineColor, MOAIColor.COLOR_TRAIT )

glowColor = MOAIColor.new ()
glowColor:setColor ( 0.0, 0.25, 1.0, 1.0 )
shader:setAttrLink ( 4, glowColor, MOAIColor.COLOR_TRAIT )

MOAISim.openWindow ( "test", width, height )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale ( width, -height )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

sdfTexture = MOAITexture.new ()
sdfTexture:load ( image )
sdfTexture:setFilter ( MOAITexture.GL_LINEAR )

rampTexture = MOAITexture.new ()
rampTexture:load ( 'ramp.png' )
rampTexture:setFilter ( MOAITexture.GL_LINEAR )
rampTexture:setWrap ( false )

texture = MOAIMultiTexture.new ()
texture:reserve ( 2 )
texture:setTexture ( 1, sdfTexture )
texture:setTexture ( 2, rampTexture )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setShader ( shader )
gfxQuad:setRect ( -hWidth, -hHeight, hWidth, hHeight )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )
