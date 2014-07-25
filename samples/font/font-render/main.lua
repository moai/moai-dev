----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_GLYPHS, 1, 0, 0, 1, 1 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

onGlyph = function ( cache, code, image, xMin, yMin, xMax, yMax )
	
	print ( 'GLYPH:', code, image, xMin, yMin, xMax, yMax )

	image:generateSDF (xMin, yMin, xMax, yMax)

	
	--for x = xMin, xMax do
		--for y = yMin, yMax do
			--local r, g, b, a = image:getRGBA ( x, y )
			--print ( 'COLOR:', r, g, b, a )
			--image:setRGBA ( x, y, 1 - r, 1 - b, 1 - g, 1 - a )
		--end
	--end
	
end

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()

cache = font:getCache ()
cache:setPadding ( 20, 20 )
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

style = textbox:getStyle ()
style:setPadding ( 10, 10 )

layer:insertProp ( textbox )
