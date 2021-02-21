
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

gaussShader = MOAIShader.new ()
gaussShader:setProgram ( gaussProgram )
