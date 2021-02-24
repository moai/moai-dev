
----------------------------------------------------------------
local deck2DProgram = MOAIShaderProgram.new ()

deck2DProgram:setVertexAttribute ( 1, 'position' )
deck2DProgram:setVertexAttribute ( 2, 'uv' )
deck2DProgram:setVertexAttribute ( 3, 'color' )

deck2DProgram:load ( MOAIFileSystem.loadFile ( 'shaders/deck2D.vsh' ), MOAIFileSystem.loadFile ( 'shaders/deck2D.fsh' ))

deck2DShader = MOAIShader.new ()
deck2DShader:setProgram ( deck2DProgram )

----------------------------------------------------------------
local emissiveProgram = MOAIShaderProgram.new ()

emissiveProgram:setVertexAttribute ( 1, 'position' )
emissiveProgram:setVertexAttribute ( 2, 'uv' )
emissiveProgram:setVertexAttribute ( 3, 'color' )

emissiveProgram:load ( MOAIFileSystem.loadFile ( 'shaders/emissive.vsh' ), MOAIFileSystem.loadFile ( 'shaders/emissive.fsh' ))

emissiveShader = MOAIShader.new ()
emissiveShader:setProgram ( emissiveProgram )

----------------------------------------------------------------
local gaussProgram = MOAIShaderProgram.new ()

gaussProgram:setVertexAttribute ( 1, 'position' )
gaussProgram:setVertexAttribute ( 2, 'uv' )
gaussProgram:setVertexAttribute ( 3, 'color' )

gaussProgram:load ( MOAIFileSystem.loadFile ( 'shaders/gauss.vsh' ), MOAIFileSystem.loadFile ( 'shaders/gauss.fsh' ))

gaussProgram:reserveUniforms ( 1 )
gaussProgram:declareUniform ( 1, 'direction', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_VEC_2 )

gaussShader = MOAIShader.new ()
gaussShader:setProgram ( gaussProgram )

----------------------------------------------------------------
local glowProgram = MOAIShaderProgram.new ()

glowProgram:setVertexAttribute ( 1, 'position' )
glowProgram:setVertexAttribute ( 2, 'uv' )
glowProgram:setVertexAttribute ( 3, 'color' )

glowProgram:load ( MOAIFileSystem.loadFile ( 'shaders/glow.vsh' ), MOAIFileSystem.loadFile ( 'shaders/glow.fsh' ))

glowProgram:reserveUniforms ( 3 )
glowProgram:declareUniform ( 1, 'glow', MOAIShaderProgram.UNIFORM_TYPE_FLOAT )
glowProgram:declareUniform ( 2, 'colorSampler', MOAIShaderProgram.UNIFORM_TYPE_INT )
glowProgram:declareUniform ( 3, 'glowSampler', MOAIShaderProgram.UNIFORM_TYPE_INT )

glowShader = MOAIShader.new ()
glowShader:setProgram ( glowProgram )

glowShader:setUniform ( 1, 0.5 )
glowShader:setUniform ( 2, 0 )
glowShader:setUniform ( 3, 1 )
