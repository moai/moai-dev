----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

onGlyph = function ( cache, code, image, xMin, yMin, xMax, yMax )
	
	--if code == 65 then
	
		print ( 'GLYPH:', code, image, xMin, yMin, xMax, yMax )
	
		for x = xMin, xMax do
			for y = yMin, yMax do
				local r, g, b, a = image:getRGBA ( x, y )
				image:setRGBA ( x, y, 1 - r, 1 - b, 1 - g, 1 - a )
			end
		end
	--end
end

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()

cache = font:getCache ()
cache:setListener ( MOAIDynamicGlyphCache.EVENT_RENDER_GLYPH, onGlyph )

font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 50 )

image = font:getImage ()
image:writePNG ( 'font.png' )

-- set the font image
--font:setCache ()
--font:setReader ()
--font:setImage ( image )

-- font now ready for use
textbox = MOAITextBox.new ()
textbox:setText ( charcodes )
textbox:setFont ( font )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )
