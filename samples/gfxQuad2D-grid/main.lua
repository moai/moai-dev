----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local fsh = [[  
 
    void main () {
        gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );
    }
]]

local vsh = [[ 

    attribute vec4 position;
 
    void main () { 
        gl_Position = position;          
    }
]]

local program = MOAIShaderProgram.new ()
program:load ( vsh, fsh )

local shader = MOAIShader.new ()
shader:setProgram ( program )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setRow ( 1, 	0x10000001, 0x20000001 )
grid:setRow ( 2,	0x40000001, 0x60000001 )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "../resources/moai.png" )

prop = MOAIGraphicsGridProp.new ()
prop:setDeck ( gfxQuad )
prop:setGrid ( grid )
layer:insertProp ( prop )

prop:moveRot ( 0, 0, 360, 5 )

