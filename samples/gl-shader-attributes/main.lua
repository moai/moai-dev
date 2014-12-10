----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

file = assert ( io.open ( 'shader.vsh', mode ))
vsh = file:read ( '*all' )
file:close ()

file = assert ( io.open ( 'shader.fsh', mode ))
fsh = file:read ( '*all' )
file:close ()

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "numbers.png" )
tileDeck:setSize ( 8, 8 )
tileDeck:setRect ( -0.5, 0.5, 0.5, -0.5 )

grid = MOAIGrid.new ()
grid:setSize ( 8, 8, 32, 32 )
grid:setRepeat ( true ) -- wrap the grid when drawing

grid:setRow ( 1, 	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 )
grid:setRow ( 2, 	0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 )
grid:setRow ( 3, 	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18 )
grid:setRow ( 4, 	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20 )
grid:setRow ( 5, 	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28 )
grid:setRow ( 6, 	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30 )
grid:setRow ( 7, 	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 )
grid:setRow ( 8, 	0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -128, 128 )
prop:setScl ( 1, -1 )
layer:insertProp ( prop )

--prop:moveRot ( 360, 6 )
--prop:moveLoc ( -512, 0, 6 )

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )

program:reserveUniforms ( 2 )
program:declareUniform ( 1, 'xWarp', MOAIShaderProgram.UNIFORM_FLOAT )
program:declareUniform ( 2, 'yWarp', MOAIShaderProgram.UNIFORM_FLOAT )

program:load ( vsh, fsh )

shader = MOAIShader.new ()
shader:setProgram ( program )
tileDeck:setShader ( shader )

shader:setAttr ( 1, 2 )
shader:setAttr ( 2, 0 )

shader:seekAttr ( 1, 0, 6 )
shader:seekAttr ( 2, 2, 6 )
