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

onGlyph = function ( cache, code, image, xMin, yMin, xMax, yMax )
	
	print ( 'GLYPH:', code, image, xMin, yMin, xMax, yMax )

	image:generateSDF (xMin - 5, yMin - 5, xMax + 5, yMax + 5)

	
	--for x = xMin, xMax do
		--for y = yMin, yMax do
			--local r, g, b, a = image:getRGBA ( x, y )
			--print ( 'COLOR:', r, g, b, a )
			--image:setRGBA ( x, y, 1 - r, 1 - b, 1 - g, 1 - a )
		--end
	--end
	
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

program:reserveUniforms ( 2 )
program:declareUniform ( 1, 'outlineMin', MOAIShaderProgram.UNIFORM_FLOAT )
program:declareUniform ( 2, 'outlineMax', MOAIShaderProgram.UNIFORM_FLOAT )

program:load ( vsh, fsh )

--color = MOAIColor.new ()
--color:setColor ( 1, 0, 0, 1 )
--color:moveColor ( 1, 0, 0, 0, 1.5 )

shader = MOAIShader.new ()
shader:setProgram ( program )
shader:setAttr ( 1, 0.51 )
shader:setAttr ( 2, 0.55 )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()

cache = font:getCache ()
cache:setPadding ( 10, 10 )
cache:setListener ( MOAIDynamicGlyphCache.EVENT_RENDER_GLYPH, onGlyph )

font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 50 )

image = font:getImage ()
image:writePNG ( 'font.png' )

-- font now ready for use
textbox = MOAITextBox.new ()
textbox:setText ( 'A B C D E' )
textbox:setFont ( font )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
textbox:setColor ( 0, 0, 1, 1 )
textbox:setShader ( shader )

style = textbox:getStyle ()
style:setPadding ( 10, 10 )

layer:insertProp ( textbox )
