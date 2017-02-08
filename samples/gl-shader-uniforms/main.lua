MOAISim.openWindow ( "test", 320, 320 )

local fsh = [[  

    uniform vec4   colorVec;
    uniform mat4    worldMatrix;
 
    void main () {
        gl_FragColor = worldMatrix * colorVec;
    }
]]

local vsh = [[ 

    attribute vec4 position;
 
    void main () { 
        gl_Position = position;          
    }
]]

local program = MOAIShaderProgram.new ()
program:reserveUniforms ( 2 )
program:declareUniform ( 1, 'colorVec', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_VEC_4 )
program:declareUniform ( 2, 'worldMatrix', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_MATRIX_4X4 )
program:load ( vsh, fsh )

program:reserveGlobals ( 1 )
program:setGlobal ( 1, MOAIShaderProgram.GLOBAL_WORLD, 2, 1 )

local shader = MOAIShader.new ()
shader:setProgram ( program )
shader:setUniform ( 1, 1, 0, 0, 1 )

----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 320 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 320 )
viewport:setScale ( 320, 320 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )

layer:pushRenderPass ()

----------------------------------------------------------------
-- test shader on prop
spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( '../resources/moai.png' )
spriteDeck:setRect ( -128, -128, 128, 128 )

prop = MOAIProp.new ()
prop:setDeck ( spriteDeck )
prop:setShader ( shader )
layer:insertProp ( prop )

prop:moveRot ( 0, 0, -360, 5 )

----------------------------------------------------------------
-- test shader on deck
spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( '../resources/moai.png' )
spriteDeck:setRect ( -64, -64, 64, 64 )
spriteDeck:setShader ( shader )

prop = MOAIProp.new ()
prop:setDeck ( spriteDeck )
layer:insertProp ( prop )   

prop:moveRot ( 0, 0, 360, 5 )
