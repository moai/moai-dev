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

renderGlyph = function ( font, reader, image, code, x, y, xMin, yMin, xMax, yMax )
	print ( 'GLYPH:', font, reader, image, code, x, y, xMin, yMin, xMax, yMax )
	reader:renderGlyph ( image, x, y )
	image:generateSDF ( xMin, yMin, xMax, yMax )
end

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()
font:setListener ( MOAIFont.EVENT_RENDER_GLYPH, renderGlyph )

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

style = textbox:getStyle ()
style:setPadding ( 10, 10 )

layer:insertProp ( textbox )
