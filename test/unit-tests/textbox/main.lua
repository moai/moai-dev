----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

workingDir = MOAIFileSystem.getWorkingDirectory ()
MOAIFileSystem.setWorkingDirectory ( ".." )
require "testhelpers"
MOAIFileSystem.setWorkingDirectory ( workingDir )
MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 12, 163 )

running = false
continue = false

-- clearCurves
-- reserveCurves
-- setCurve
-- spool
function curveTests ()
	running = true
	
	text = 'This text is curved. This text is curved. This text is curved.'
	
	textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 3 )
	curve:setKey ( 1, 0, 0 )
	curve:setKey ( 2, .5, 16 )
	curve:setKey ( 3, 1, 0 )

	textbox:reserveCurves ( 1 )
	textbox:setCurve ( 1, curve )
	
	continue = false
	repeat coroutine.yield () until continue
	
	text = 'This text has two curves. This text has two curves. This text has two curves.'
	textbox:setString ( text )
	curve2 = MOAIAnimCurve.new ()
	curve2:reserveKeys ( 2 )
	curve2:setKey ( 1, 0, 0 )
	curve2:setKey ( 2, 1, 24 )

	textbox:reserveCurves ( 2 )
	textbox:setCurve ( 1, curve )
	textbox:setCurve ( 2, curve2 )
	
	continue = false
	repeat coroutine.yield () until continue

	text = 'The curve has been cleared. The curve has been cleared. The curve has been cleared.'
	textbox:setString ( text )
	textbox:clearCurves ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setReveal ( 0 )
	running = false
end

-- more
-- nextPage
-- spool
function pageTests ()
	running = true
	
	text = '1This is long text that will take multiple pages. This is long text that will take 2multiple pages. This is long text that will take multiple pages. This is long text that 3will take multiple pages. This is long text that will take multiple pages.'
	
	textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 100, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	textbox:forceUpdate ()
	
	continue = false
	
	while textbox:more () do
		repeat coroutine.yield () until continue
		if textbox:more () then
			textbox:nextPage ()
		end
		continue = false
	end
	
	textbox:setReveal ( 0 )
	running = false
end

-- setAlignment
function alignmentTests ()
	running = true

	text = 'This text is left justified. This text is left justified.'
	local textbox1 = MOAITextBox.new ()
	textbox1:setString ( text )
	textbox1:setFont ( font )
	textbox1:setTextSize ( 12, 163 )
	textbox1:setRect ( -150, 70, 150, 230 )
	textbox1:setAlignment ( MOAITextBox.LEFT_JUSTIFY )
	textbox1:setYFlip ( true )
	layer:insertProp ( textbox1 )
	
	text = 'This text is center justified. This text is center justified.'
	local textbox2 = MOAITextBox.new ()
	textbox2:setString ( text )
	textbox2:setFont ( font )
	textbox2:setTextSize ( 12, 163 )
	textbox2:setRect ( -150, -90, 150, 70 )
	textbox2:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	textbox2:setYFlip ( true )
	layer:insertProp ( textbox2 )
	
	text = 'This text is right justified. This text is right justified.'
	local textbox3 = MOAITextBox.new ()
	textbox3:setString ( text )
	textbox3:setFont ( font )
	textbox3:setTextSize ( 12, 163 )
	textbox3:setRect ( -150, -250, 150, -90 )
	textbox3:setAlignment ( MOAITextBox.RIGHT_JUSTIFY )
	textbox3:setYFlip ( true )
	layer:insertProp ( textbox3 )
	
	continue = false
	repeat coroutine.yield () until continue
	text = 'This text is\n     left justified.'
	textbox1:setString ( text )
	text = 'This text is\n     center justified.'
	textbox2:setString ( text )
	text = 'This text is\n     right justified.'
	textbox3:setString ( text )
	
	continue = false
	repeat coroutine.yield () until continue
	text = 'This text is     \nleft justified.'
	textbox1:setString ( text )
	text = 'This text is     \ncenter justified.'
	textbox2:setString ( text )
	text = 'This text is     \nright justified.'
	textbox3:setString ( text )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox1:setReveal ( 0 )
	textbox2:setReveal ( 0 )
	textbox3:setReveal ( 0 )
	running = false
end

-- revealAll
-- setReveal
function revealTests ()
	running = true
	
	text = '1234567890 1234567890 1234567890 1234567890 1234567890 1234567890'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 70, -50, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	textbox:spool ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:stop ()
	textbox:revealAll ()
	textbox:setRect ( -150, 70, 150, 230 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setReveal ( 25 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setReveal ( -5 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setReveal ( 0 )
	running = false
end

-- setHighlight
function highlightTests ()
	running = true

	text = '1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 70, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	textbox:setHighlight ( 12, 10, 1, 1, 0 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setHighlight ( 12, 10, 1, 1, 0, .5 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setHighlight ( 12, 10 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setHighlight ( 50, 10, 0, 0, 1 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setReveal ( 0 )
	running = false
end

-- setLineSpacing
function lineSpacingTests ()
	running = true
	
	text = '1234567890 1234567890 1234567890 1234567890 1234567890'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 70, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	textbox:setLineSpacing ( 2 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 1 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setReveal ( 0 )
	running = false
end

-- setSpeed
function spoolSpeedTests ()
	running = true
	
	text = 'Spooling at regular speed. Spooling at regular speed. Spooling at regular speed.'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 70, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	textbox:setSpeed ( 24 )
	textbox:spool ()
	
	continue = false
	repeat coroutine.yield () until continue
	
	text = 'Spooling at double speed. Spooling at double speed. Spooling at double speed.'
	textbox:setString ( text )
	textbox:setSpeed ( 48 )
	textbox:spool ()
	
	continue = false
	repeat coroutine.yield () until continue
	
	text = 'Spooling at half speed. Spooling at half speed. Spooling at half speed.'
	textbox:setString ( text )
	textbox:setSpeed ( 12 )
	textbox:spool ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:stop ()
	textbox:setReveal ( 0 )
	running = false
end

-- setStyle
function setStyleTests ()

end

-- setYFlip
function setYFlipTests ()
	running = true
	
	text = '1Text is YFlipped. 2Text is YFlipped. 3Text is YFlipped.'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 70, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	continue = false
	repeat coroutine.yield () until continue
	text = '1Text is not YFLipped. 2Text is not YFLipped. 3Text is not YFLipped.'
	textbox:setString ( text )
	textbox:setYFlip ( false )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setReveal ( 0 )
	running = false
end

-- tests
function onKeyboardEvent ( key, down )
	if down == true then
		-- print ( key )
		if key == 32 and running and not continue then -- continue
			continue = true
		elseif key == 49 and not running then -- curveTests
			thread = MOAIThread.new ()
			thread:run ( curveTests )
		elseif key == 50 and not running then -- pageTests
			thread = MOAIThread.new ()
			thread:run ( pageTests )
		elseif key == 51 and not running then -- alignmentTests
			thread = MOAIThread.new ()
			thread:run ( alignmentTests )
		elseif key == 52 and not running then -- revealTests
			thread = MOAIThread.new ()
			thread:run ( revealTests )
		elseif key == 53 and not running then -- highlightTests
			thread = MOAIThread.new ()
			thread:run ( highlightTests )
		elseif key == 54 and not running then -- lineSpacingTests
			thread = MOAIThread.new ()
			thread:run ( lineSpacingTests )
		elseif key == 55 and not running then -- spoolSpeedTests
			thread = MOAIThread.new ()
			thread:run ( spoolSpeedTests )
		elseif key == 56 and not running then -- setStyleTests
			thread = MOAIThread.new ()
			thread:run ( setStyleTests )
		elseif key == 57 and not running then -- setYFlipTests
			thread = MOAIThread.new ()
			thread:run ( setYFlipTests )
		else
			return
		end
	end
end

function homeScreen ()
	text = 'Home\n\n1 Curve Tests\n2 Page Tests\n3 Alignment Tests\n4 Reveal Tests\n5 Highlight Tests\n6 Line Spacing Tests\n7 Spool Speed Tests\n8 Set Style Tests\n9 Set YFlip Tests'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	while 1 do
		if running then
			textbox:setReveal ( 0 )
		else
			textbox:revealAll ()
		end
		coroutine.yield ()
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )
thread = MOAIThread.new ()
thread:run ( homeScreen )
