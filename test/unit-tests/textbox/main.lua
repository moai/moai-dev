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
-- MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
-- MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 0, 0, 1, 1 )
-- MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 12, 163 )

font1 = MOAIFont.new ()
font1:load ( 'arial-rounded.TTF' )

font2 = MOAIFont.new ()
font2:load ( 'Dwarves.TTF' )
font2:preloadGlyphs ( charcodes, 24 )

font3 = MOAIFont.new ()
font3:load ( 'EBOLA-KI.TTF' )

font4 = MOAIFont.new ()
font4:load ( 'nanum_gothic_coding.ttf' )

running = false
continue = false

-- clearCurves
-- reserveCurves
-- setCurve
-- spool
function curveTests ()
	running = true
	
	text = 'This text is curved. This text is curved. This text is curved. This text is curved. This text is curved. This text is curved.'
	
	textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	font1:setDefaultSize ( 12, 163 )
	textbox:setRect ( -150, 70, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 3 )
	curve:setKey ( 1, 0, 0 )
	curve:setKey ( 2, .5, 16 )
	curve:setKey ( 3, 1, 0 )

	textbox:reserveCurves ( 1 )
	textbox:setCurve ( 1, curve )
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 150 and ymax == 230,
		"Get rect" )
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	
	text = 'This text has two curves. This text has two curves. This text has two curves. This text has two curves. This text has two curves. This text has two curves.'
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
	
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue

	text = 'The curve has been cleared. The curve has been cleared. The curve has been cleared.'
	textbox:setString ( text )
	textbox:setCurve ()
	
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
	
	text = '1This is long text that will take multiple pages. This is long text that will take multiple 2pages. This is long text that will take multiple pages. This is long text that will take 3multiple pages. This is long text that will take multiple pages.'
	
	textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 100, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	textbox:forceUpdate ()
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 100 and xmax == 150 and ymax == 230,
		"Get rect" )
	
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
	
	xmin, ymin, xmax, ymax = textbox1:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 150 and ymax == 230,
		"Get rect" )
	
	text = 'This text is center justified. This text is center justified.'
	local textbox2 = MOAITextBox.new ()
	textbox2:setString ( text )
	textbox2:setFont ( font )
	textbox2:setTextSize ( 12, 163 )
	textbox2:setRect ( -150, -90, 150, 70 )
	textbox2:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	textbox2:setYFlip ( true )
	layer:insertProp ( textbox2 )
	
	xmin, ymin, xmax, ymax = textbox2:getRect ()
	test.evaluate ( xmin == -150 and ymin == -90 and xmax == 150 and ymax == 70,
		"Get rect" )
	
	text = 'This text is right justified. This text is right justified.'
	local textbox3 = MOAITextBox.new ()
	textbox3:setString ( text )
	textbox3:setFont ( font )
	textbox3:setTextSize ( 12, 163 )
	textbox3:setRect ( -150, -250, 150, -90 )
	textbox3:setAlignment ( MOAITextBox.RIGHT_JUSTIFY )
	textbox3:setYFlip ( true )
	layer:insertProp ( textbox3 )
	
	xmin, ymin, xmax, ymax = textbox3:getRect ()
	test.evaluate ( xmin == -150 and ymin == -250 and xmax == 150 and ymax == -90,
		"Get rect" )
	
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
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == -50 and ymax == 230,
		"Get rect" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:stop ()
	textbox:revealAll ()
	textbox:setRect ( -150, 70, 150, 230 )
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 150 and ymax == 230,
		"Get rect" )
	
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

	text = '123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 70, 130, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 130 and ymax == 230,
		"Get rect" )
	
	textbox:setHighlight ( 1, 5, 1, 0, 0 )
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:setHighlight ( 16, 5, 1, 0, 0 )
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:setHighlight ( 8, 5, 1, 0, 0 )
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:setHighlight ( 3, 14, 1, 0, 0 )
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:setHighlight ( 45, 21, 0, 0, 1 )
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:clearHighlights ()
	print ( "cleared" )
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
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
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	textbox:setLineSpacing ( 0 )
	test.evaluate ( textbox:getLineSpacing () == 0,
		"getLineSpacing with 0" )
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == -230 and xmax == 150 and ymax == 230,
		"Get rect" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( -10 )
	test.evaluate ( textbox:getLineSpacing () == 0,
		"getLineSpacing with -10" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 2 )
	test.evaluate ( textbox:getLineSpacing () == 2,
		"getLineSpacing with 2" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 3 )
	test.evaluate ( textbox:getLineSpacing () == 3,
		"getLineSpacing with 3" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 4 )
	test.evaluate ( textbox:getLineSpacing () == 4,
		"getLineSpacing with 4" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 5 )
	test.evaluate ( textbox:getLineSpacing () == 5,
		"getLineSpacing with 5" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 6 )
	test.evaluate ( textbox:getLineSpacing () == 6,
		"getLineSpacing with 6" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 7 )
	test.evaluate ( textbox:getLineSpacing () == 7,
		"getLineSpacing with 7" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 8 )
	test.evaluate ( textbox:getLineSpacing () == 8,
		"getLineSpacing with 8" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 9 )
	test.evaluate ( textbox:getLineSpacing () == 9,
		"getLineSpacing with 9" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 10 )
	test.evaluate ( textbox:getLineSpacing () == 10,
		"getLineSpacing with 10" )
	
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
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 150 and ymax == 230,
		"Get rect" )
	
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
	
	text = 'Spooling with negative speed. Spooling with negative speed. Spooling with negative speed.'
	textbox:setString ( text )
	textbox:setSpeed ( -12 )
	textbox:spool ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:stop ()
	textbox:setReveal ( 0 )
	running = false
end

-- setStyle
function setStyleTests ()
	running = true
	
	-- multiple styles
	text = 'This is <gre/en>some test</> text <<to <yellow><blue>print out while</> testing different styles.</> This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</> This is <green>some test</> text to <yellow><blue>print <<>out while</> testing different styles.</>'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setRect ( -150, 70, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 150 and ymax == 230,
		"Get rect" )
	
	local style1 = MOAITextStyle.new ()
	style1:setColor ( 1, 0, 0, 1 )
	style1:setFont ( font1 )
	style1:setSize ( 12 )
	textbox:setStyle ( style1 )
	textbox:setStyle ( 'red', style1 )
	
	local style2 = MOAITextStyle.new ()
	style2:setColor ( 0, 1, 0, 1 )
	style2:setFont ( font2 )
	style2:setSize ( 24 )
	textbox:setStyle ( 'green', style2 )
	textbox:setStyle ( 'gre/en', style2 )
	
	local style3 = MOAITextStyle.new ()
	style3:setColor ( 0, 0, 1, .5 )
	style3:setFont ( font3 )
	style3:setSize ( 36 )
	textbox:setStyle ( 'blue', style3 )
	
	local style4 = MOAITextStyle.new ()
	style4:setColor ( 1, 1, 0, 1 )
	style4:setFont ( font1 )
	style4:setSize ( 12 )
	textbox:setStyle ( 'yellow', style4 )
	
	continue = false
	repeat coroutine.yield () until continue
	
	-- styles with curves
	curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 3 )
	curve:setKey ( 1, 0, 0 )
	curve:setKey ( 2, .5, 16 )
	curve:setKey ( 3, 1, 0 )

	textbox:reserveCurves ( 1 )
	textbox:setCurve ( 1, curve )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setCurve ()
	
	-- styles with pages
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:nextPage ()
	textbox:setReveal ( 0 )
	
	-- styles with alignment
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	local textbox1 = MOAITextBox.new ()
	textbox1:setString ( text )
	textbox1:setStyle ( style1 )
	textbox1:setStyle ( 'red', style1 )
	textbox1:setStyle ( 'green', style2 )
	textbox1:setStyle ( 'blue', style3 )
	textbox1:setStyle ( 'yellow', style4 )
	textbox1:setRect ( -150, 70, 150, 230 )
	textbox1:setAlignment ( MOAITextBox.LEFT_JUSTIFY )
	textbox1:setYFlip ( true )
	layer:insertProp ( textbox1 )
	
	xmin, ymin, xmax, ymax = textbox1:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 150 and ymax == 230,
		"Get rect" )
	
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	local textbox2 = MOAITextBox.new ()
	textbox2:setString ( text )
	textbox2:setStyle ( style1 )
	textbox2:setStyle ( 'red', style1 )
	textbox2:setStyle ( 'green', style2 )
	textbox2:setStyle ( 'blue', style3 )
	textbox2:setStyle ( 'yellow', style4 )
	textbox2:setRect ( -150, -90, 150, 70 )
	textbox2:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	textbox2:setYFlip ( true )
	layer:insertProp ( textbox2 )
	
	xmin, ymin, xmax, ymax = textbox2:getRect ()
	test.evaluate ( xmin == -150 and ymin == -90 and xmax == 150 and ymax == 70,
		"Get rect" )
	
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	local textbox3 = MOAITextBox.new ()
	textbox3:setString ( text )
	textbox3:setStyle ( style1 )
	textbox3:setStyle ( 'red', style1 )
	textbox3:setStyle ( 'green', style2 )
	textbox3:setStyle ( 'blue', style3 )
	textbox3:setStyle ( 'yellow', style4 )
	textbox3:setRect ( -150, -250, 150, -90 )
	textbox3:setAlignment ( MOAITextBox.RIGHT_JUSTIFY )
	textbox3:setYFlip ( true )
	layer:insertProp ( textbox3 )
	
	xmin, ymin, xmax, ymax = textbox3:getRect ()
	test.evaluate ( xmin == -150 and ymin == -250 and xmax == 150 and ymax == -90,
		"Get rect" )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox1:setReveal ( 0 )
	textbox2:setReveal ( 0 )
	textbox3:setReveal ( 0 )
	
	-- styles with setReveal
	textbox:setReveal ( 25 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:revealAll ()
	
	-- styles with a highlight
	textbox:setHighlight ( 25, 20, 1, 1, 1, 1 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setHighlight ( 25, 20 )
	
	-- styles with line spacing
	textbox:setLineSpacing ( 10 )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 0 )
	textbox:setReveal ( 0 )
	textbox1:setAlignment ( MOAITextBox.LEFT_JUSTIFY )
	textbox2:setAlignment ( MOAITextBox.LEFT_JUSTIFY )
	textbox3:setAlignment ( MOAITextBox.LEFT_JUSTIFY )
	
	textbox1:setSpeed ( 12 )
	textbox2:setSpeed ( 24 )
	textbox3:setSpeed ( 48 )
	textbox1:spool ()
	textbox2:spool ()
	textbox3:spool ()
	
	continue = false
	repeat coroutine.yield () until continue
	textbox1:stop ()
	textbox2:stop ()
	textbox3:stop ()
	textbox1:setReveal ( 0 )
	textbox2:setReveal ( 0 )
	textbox3:setReveal ( 0 )
	textbox:revealAll ()
	
	-- styles with yflip
	textbox:setYFlip ( false )
	
	continue = false
	repeat coroutine.yield () until continue
	textbox:setReveal ( 0 )
	running = false
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
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 150 and ymax == 230,
		"Get rect" )
	
	continue = false
	repeat coroutine.yield () until continue
	text = '1Text is not YFLipped. 2Text is not YFLipped. 3Text is not YFLipped.'
	textbox:setString ( text )
	textbox:setYFlip ( false )
	
	continue = false
	repeat coroutine.yield () until continue
	text = '1Text is not YFLipped. 2Text is not YFLipped. 3Text is not YFLipped.'
	viewport:setScale ( 320, -480 )
	textbox:setString ( text )
	textbox:setYFlip ( false )
	
	continue = false
	repeat coroutine.yield () until continue
	viewport:setScale ( 320, 480 )
	textbox:setReveal ( 0 )
	running = false
end

function longTextTests ()
	running = true
	text = "test \
	"
	
	text = '1<red>Lorem ipsum dolor sit amet,</> consectetur adipiscing elit. <green>Pellentesque id<red> magna adipiscing</> augue mollis fringilla.</> Fusce mauris <yellow>justo, scelerisque sit</> amet condimentum at, <blue>accumsan in velit. Proin <yellow>suscipit, mauris <green>nec volutpat <blue>congue, orci sapien</> posuere arcu,</></> vitae</> tempor 2massa neque <green>nec metus. Lorem ipsum</> dolor sit<blue></><green> amet, consectetur</> adipi<green>scing elit.<blue> Etiam</> tristique,</> odio <red>vel adipiscing facilisis, magna neque scelerisque metus, et volutpat turpis erat in orci. <blue>Duis suscipit eleifend <red>posuere. Vivamus auctor cursus mi, eget dapibus risus fringilla eget. Donec bibendum urna 3vitae orci</></></> adipiscing facilisis. Nam ullamcorper ultrices tempor. Vivamus a tortor lectus, in euismod metus. Etiam cursus molestie arcu nec fermentum. Nulla porttitor dui eget neque dignissim tristique. Donec elit quam, lobortis vitae fringilla ut, placerat et leo. Phasellus iaculis commodo ante quis condimentum. Vestibulum at neque a 4nibh fringilla semper. Sed diam dolor, convallis a tempus in, fringilla id lacus. Donec bibendum, erat semper consequat cursus, nibh neque tempor quam, nec pellentesque justo ligula eget nibh. Vestibulum adipiscing, enim vitae porttitor semper, nisi est vestibulum elit, non varius tortor dui at quam. Suspendisse tincidunt, velit at mollis vehicula, nunc est vulputate enim, 5condimentum vestibulum tortor tortor sed nulla. Nunc ac lectus urna. Vivamus condimentum nisi volutpat nulla vestibulum at condimentum ligula viverra. Nunc nec adipiscing purus.'

	-- gfxQuad = MOAIGfxQuad2D.new ()
	-- gfxQuad:setTexture ( "box.png" )
	-- gfxQuad:setRect ( -64, -64, 64, 64 )

	-- prop = MOAIProp2D.new ()
	-- prop:setDeck ( gfxQuad )
	-- layer:insertProp ( prop )
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == -230 and xmax == 150 and ymax == 230,
		"Get rect" )
	
	local style = MOAITextStyle.new ()
	style:setColor ( 1, 1, 1, 1 )
	style:setFont ( font1 )
	style:setSize ( 24 )
	textbox:setStyle ( style )
	
	local style1 = MOAITextStyle.new ()
	style1:setColor ( 1, 0, 0, 1 )
	style1:setFont ( font1 )
	style1:setSize ( 24 )
	textbox:setStyle ( 'red', style1 )
	
	local style2 = MOAITextStyle.new ()
	style2:setColor ( 0, 1, 0, 1 )
	style2:setFont ( font2 )
	style2:setSize ( 24 )
	textbox:setStyle ( 'green', style2 )
	
	local style3 = MOAITextStyle.new ()
	style3:setColor ( 0, 0, 1, .5 )
	style3:setFont ( font3 )
	style3:setSize ( 36 )
	textbox:setStyle ( 'blue', style3 )
	
	local style4 = MOAITextStyle.new ()
	style4:setColor ( 1, 1, 0, 1 )
	style4:setFont ( font1 )
	style4:setSize ( 12 )
	textbox:setStyle ( 'yellow', style4 )
	
	print ( textbox:getStringBounds ( 1, 10 ) )
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	
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

function koreanTests ()
	running = true
	
	--text = '상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다.'
	text = '<green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다. 상대 종족 장군을 <blue>3명 처치해야 하는 고난이도 퀘스트를 완료하면 공격 속도와 비행 속도가 붙어</> 있는 고성능 타이틀을 입수할 수 있다. 비행 속도의 경우 <green>16%만 맞추면 상급 광풍의 주문서를 먹고서 최대 속도가 <blue>나오므로 앞서 소개했던 타이틀에 비하면</> 성능은 다소 낮은 편이라고 할 수 있다. 하지만,</> 이동 속도 감소에 걸리거나 했을 때에는 확실히 도움이 되며, 만약 비행 속도가 모자른 장비를 사용하고 있다면 더 유용하게 쓸 수 있다.'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	xmin, ymin, xmax, ymax = textbox:getRect ()
	test.evaluate ( xmin == -150 and ymin == 70 and xmax == 150 and ymax == 230,
		"Get rect" )
	
	local style1 = MOAITextStyle.new ()
	style1:setColor ( 1, 1, 1, 1 )
	style1:setFont ( font4 )
	style1:setSize ( 12 )
	textbox:setStyle ( style1 )
	
	local style2 = MOAITextStyle.new ()
	style2:setColor ( 0, 1, 1, 1 )
	style2:setFont ( font4 )
	style2:setSize ( 20 )
	textbox:setStyle ( 'blue', style2 )
	
	local style3 = MOAITextStyle.new ()
	style3:setColor ( 0, 1, 0, 1 )
	style3:setFont ( font4 )
	style3:setSize ( 28 )
	textbox:setStyle ( 'green', style3 )
	
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	
	continue = false
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	
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
		elseif key == 48 and not running then -- longTextTests
			thread = MOAIThread.new ()
			thread:run ( koreanTests )
		else
			return
		end
	end
end

function homeScreen ()
	text = 'Home\n\n1 Curve Tests\n2 Page Tests\n3 Alignment Tests\n4 Reveal Tests\n5 Highlight Tests\n6 Line Spacing Tests\n7 Spool Speed Tests\n8 Set Style Tests\n9 Set YFlip Tests\n0 Korean Text'
	
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
