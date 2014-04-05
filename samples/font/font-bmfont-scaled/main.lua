----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 0, 0, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The quick brown fox jumps over the lazy dog who is so totally lazy that he barely jumps at all.'

bitmapFont = MOAIFont.new ()
bitmapFont:loadFromBMFont ( 'Verdana.fnt' )

staticTextbox = MOAITextBox.new ()
staticTextbox:setString ( text )
staticTextbox:setFont ( bitmapFont )
staticTextbox:setTextSize ( 18 )
staticTextbox:setRect ( -150, 0, 150, 130 )
staticTextbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
staticTextbox:setYFlip ( true )
layer:insertProp ( staticTextbox )

dynamicFont = MOAIFont.new ()
dynamicFont:load ( "Verdana.ttf" )
dynamicFont:preloadGlyphs ( charcodes, 18 )

dynamicTextbox = MOAITextBox.new ()
dynamicTextbox:setString ( text )
dynamicTextbox:setFont ( dynamicFont )
dynamicTextbox:setTextSize ( 18 )
dynamicTextbox:setRect ( -150, -130, 150, 0 )
dynamicTextbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
dynamicTextbox:setYFlip ( true )
layer:insertProp ( dynamicTextbox )
