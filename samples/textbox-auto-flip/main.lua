----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 1024, 1024 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_GLYPHS, 1, 0.5, 0.5, 0.5, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

camera = MOAICamera2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 1024, 1024 )
viewport:setScale ( 1024, 1024 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
MOAISim.pushRenderPass ( layer )

text1 = 'This quick brown fox jumped over the something something.'
--text2 = 'Two lines\nof text...\nor NOT!'
text2 = 'One line of text'
text3 = '\nOne line of text'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF' )

local labels = {}

local addLabel = function ( text, x, y, hJustify, vJustify )

	local label = MOAITextLabel.new ()
	label:setString ( text )
	label:setFont ( font )
	label:setTextSize ( 32 )
	label:setAlignment ( hJustify, vJustify )
	label:setYFlip ( true )
	label:setAutoFlip ( true )
	label:setLoc ( x, y )
	layer:insertProp ( label )
	
	table.insert ( labels, label )
end

local addTextBox = function ( text, x, y, hJustify, vJustify )

	local label = MOAITextLabel.new ()
	label:setString ( text )
	label:setFont ( font )
	label:setTextSize ( 32 )
	label:setAlignment ( hJustify, vJustify )
	label:setYFlip ( true )
	label:setAutoFlip ( true )
	label:setRect ( 0, -256, 256, 0 )
	label:setLoc ( x, y )
	layer:insertProp ( label )
	
	table.insert ( labels, label )
end

addTextBox ( text1,		-512, 256,		MOAITextBox.LEFT_JUSTIFY, MOAITextBox.TOP_JUSTIFY )
addTextBox ( text1,		-256, 256,		MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
addTextBox ( text1,		0, 256,			MOAITextBox.RIGHT_JUSTIFY, MOAITextBox.BOTTOM_JUSTIFY )
addTextBox ( text1,		256, 256,		MOAITextBox.CENTER_JUSTIFY, MOAITextBox.BASELINE_JUSTIFY )

addLabel ( text2,		-384, -128,		MOAITextBox.LEFT_JUSTIFY, MOAITextBox.TOP_JUSTIFY )
addLabel ( text2,		-128, -128,		MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
addLabel ( text2,		128, -128,		MOAITextBox.RIGHT_JUSTIFY, MOAITextBox.BOTTOM_JUSTIFY )
addLabel ( text2,		384, -128,		MOAITextBox.CENTER_JUSTIFY, MOAITextBox.BASELINE_JUSTIFY )

camera:moveRot ( 360, 10 )
