----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_GLYPHS, 1, 0, 0, 1, 1 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

onGlyph = function ( font, reader, image, code, x, y, xMin, yMin, xMax, yMax )

	print ( 'GLYPH:', font, reader, image, code, x, y, xMin, yMin, xMax, yMax )
	reader:renderGlyph ( image, x, y )
	image:generateSDFDeadReckoning (xMin - 10, yMin - 10, xMax + 10, yMax + 10)
	
end

file = assert ( io.open ( 'MOAIFontEffectsShader.vsh', mode ))
vsh = file:read ( '*all' )
file:close ()

file = assert ( io.open ( 'MOAIFontEffectsShader.fsh', mode ))
fsh = file:read ( '*all' )
file:close ()

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )

program:reserveUniforms ( 3 )
program:declareUniform ( 1, 'outlineMin', MOAIShaderProgram.UNIFORM_FLOAT )
program:declareUniform ( 2, 'outlineMax', MOAIShaderProgram.UNIFORM_FLOAT )
program:declareUniform ( 3, 'outlineColor', MOAIShaderProgram.UNIFORM_VECTOR_F4 )

program:load ( vsh, fsh )

color = MOAIColor.new ()
color:setColor ( 1, 1, 1, 1 )

shader = MOAIShader.new ()
shader:setProgram ( program )
shader:setAttr ( 1, 0.46 )
shader:setAttr ( 2, 0.499 )
shader:setAttrLink ( 3, color, MOAIColor.COLOR_TRAIT )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()
font:setListener ( MOAIFont.EVENT_RENDER_GLYPH, onGlyph )

cache = font:getCache ()
cache:setPadding ( 20, 20 )

font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 50 )

image = font:getImage ()
image:writePNG ( 'font.png' )

-- font now ready for use
textbox = MOAITextBox.new ()
textbox:setText ( 'A B C D E' )
textbox:setFont ( font )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
textbox:setColor ( .5, .5, .5, 1 )
textbox:setShader ( shader )

style = textbox:getStyle ()
style:setPadding ( 20, 20 )

layer:insertProp ( textbox )
