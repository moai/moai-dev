----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function evaluate ( pass, str )
	if not pass then
		MOAITestMgr.comment ( "FAILED\t" .. str )
		success = false
	end
end

function afterGrab ()
	img:writePNG ( fileName )
	continue = true
end

function afterGrabTest ()
	continue = true
end

function takeScreenshot ( fName, time )
	continue = false
	if time then
		timer:setSpan ( time )
		timer:start ()
		timer:setListener ( MOAIAction.EVENT_STOP,
			function ()
				MOAIRenderMgr.grabNextFrame ( img, afterGrab )
			end )
	elseif action:isBusy () then
		action:setListener ( MOAIAction.EVENT_STOP,
			function ()
				MOAIRenderMgr.grabNextFrame ( img, afterGrab )
			end )
	else
		MOAIRenderMgr.grabNextFrame ( img, afterGrab )
	end
	fileName = fName
end

function takeScreenshotTest ( time )
	continue = false
	if time then
		timer:setSpan ( time )
		timer:start ()
		timer:setListener ( MOAIAction.EVENT_STOP,
			function ()
				MOAIRenderMgr.grabNextFrame ( img, afterGrabTest )
			end )
	elseif action:isBusy () then
		action:setListener ( MOAIAction.EVENT_STOP,
			function ()
				MOAIRenderMgr.grabNextFrame ( img, afterGrabTest )
			end )
	else
		MOAIRenderMgr.grabNextFrame ( img, afterGrabTest )
	end
end

function getImages ()
	action = MOAIAction.new ()
	timer = MOAITimer.new ()
	
	MOAITestMgr.comment ( 'test' )
	
	-- curves
	text = 'This text is curved. This text is curved. This text is curved. This text is curved. This text is curved. This text is curved.'
	textbox:setString ( text )
	
	curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 3 )
	curve:setKey ( 1, 0, 0 )
	curve:setKey ( 2, .5, 16 )
	curve:setKey ( 3, 1, 0 )
	
	curve2 = MOAIAnimCurve.new ()
	curve2:reserveKeys ( 2 )
	curve2:setKey ( 1, 0, 0 )
	curve2:setKey ( 2, 1, 24 )
	
	textbox:reserveCurves ( 1 )
	textbox:setCurve ( 1, curve )
	
	takeScreenshot ( 'output/curve1.png' )
	repeat coroutine.yield () until continue
	
	text = 'This text has two curves. This text has two curves. This text has two curves. This text has two curves. This text has two curves. This text has two curves.'
	textbox:setString ( text )
	
	textbox:reserveCurves ( 2 )
	textbox:setCurve ( 1, curve )
	textbox:setCurve ( 2, curve2 )
	
	takeScreenshot ( 'output/curve2.png' )
	repeat coroutine.yield () until continue
	
	textbox:setCurve ()
	
	-- pages
	text = '1This is long text that will take multiple pages. This is long text that will take multiple 2pages. This is long text that will take multiple pages. This is long text that will take 3multiple pages. This is long text that will take multiple pages.'
	textbox:setString ( text )
	
	textbox:setRect ( -150, 100, 150, 230 )
	
	takeScreenshot ( 'output/page1.png' )
	repeat coroutine.yield () until continue
	textbox:nextPage ()
	
	takeScreenshot ( 'output/page2.png' )
	repeat coroutine.yield () until continue
	textbox:nextPage ()
	
	takeScreenshot ( 'output/page3.png' )
	repeat coroutine.yield () until continue
	textbox:nextPage ()
	
	textbox:setRect ( -150, -230, 150, 230 )
	
	-- alignment
	textbox:setVisible ( false )
	textbox1:setVisible ( true )
	textbox2:setVisible ( true )
	textbox3:setVisible ( true )
	
	text = 'This text is left justified. This text is left justified.'
	textbox1:setString ( text )
	
	text = 'This text is center justified. This text is center justified.'
	textbox2:setString ( text )
	
	text = 'This text is right justified. This text is right justified.'
	textbox3:setString ( text )
	
	takeScreenshot ( 'output/alignment1.png' )
	repeat coroutine.yield () until continue
	
	text = 'This text is\n     left justified.'
	textbox1:setString ( text )
	text = 'This text is\n     center justified.'
	textbox2:setString ( text )
	text = 'This text is\n     right justified.'
	textbox3:setString ( text )
	
	takeScreenshot ( 'output/alignment2.png' )
	repeat coroutine.yield () until continue
	
	text = 'This text is     \nleft justified.'
	textbox1:setString ( text )
	text = 'This text is     \ncenter justified.'
	textbox2:setString ( text )
	text = 'This text is     \nright justified.'
	textbox3:setString ( text )
	
	takeScreenshot ( 'output/alignment3.png' )
	repeat coroutine.yield () until continue
	
	textbox:setVisible ( true )
	textbox1:setVisible ( false )
	textbox2:setVisible ( false )
	textbox3:setVisible ( false )
	
	-- reveal
	text = '1234567890 1234567890 1234567890 1234567890 1234567890 1234567890'
	textbox:setString ( text )
	
	textbox:setRect ( -150, 70, -50, 230 )
	
	takeScreenshot ( 'output/reveal1.png' )
	repeat coroutine.yield () until continue
	
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setReveal ( 25 )
	
	takeScreenshot ( 'output/reveal2.png' )
	repeat coroutine.yield () until continue
	
	textbox:setReveal ( -5 )
	
	takeScreenshot ( 'output/reveal3.png' )
	repeat coroutine.yield () until continue
	
	textbox:revealAll ()
	
	takeScreenshot ( 'output/reveal4.png' )
	repeat coroutine.yield () until continue
	
	-- highlight
	text = '123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789'
	textbox:setString ( text )
	
	textbox:setRect ( -150, 70, 130, 230 )
	
	textbox:setHighlight ( 1, 5, 1, 0, 0 )
	textbox:setHighlight ( 16, 5, 1, 0, 0 )
	textbox:setHighlight ( 8, 5, 1, 0, 0 )
	
	takeScreenshot ( 'output/highlight1.png' )
	repeat coroutine.yield () until continue
	
	textbox:setHighlight ( 3, 14, 1, 0, 0 )
	
	takeScreenshot ( 'output/highlight2.png' )
	repeat coroutine.yield () until continue
	
	textbox:setHighlight ( 45, 21, 0, 0, 1 )
	
	takeScreenshot ( 'output/highlight3.png' )
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	
	takeScreenshot ( 'output/highlight4.png' )
	repeat coroutine.yield () until continue
	
	textbox:nextPage ()
	textbox:clearHighlights ()
	
	takeScreenshot ( 'output/highlight5.png' )
	repeat coroutine.yield () until continue
	
	-- line spacing
	textbox:setLineSpacing ( -10 )
	
	takeScreenshot ( 'output/spacing1.png' )
	repeat coroutine.yield () until continue
	
	textbox:setLineSpacing ( 10 )
	
	takeScreenshot ( 'output/spacing2.png' )
	repeat coroutine.yield () until continue
	
	textbox:setLineSpacing ( 0 )
	
	-- spool speed
	text = 'Spooling at regular speed. Spooling at regular speed. Spooling at regular speed.'
	textbox:setString ( text )
	
	textbox:setSpeed ( 24 )
	textbox:spool ()
	
	takeScreenshot ( 'output/spoolspeed1.png', 1 )
	repeat coroutine.yield () until continue
	
	textbox:setSpeed ( 48 )
	textbox:spool ()
	
	takeScreenshot ( 'output/spoolspeed2.png', 1 )
	repeat coroutine.yield () until continue
	
	textbox:setSpeed ( -10 )
	textbox:spool ()
	
	takeScreenshot ( 'output/spoolspeed3.png', 1 )
	repeat coroutine.yield () until continue
	
	-- styles
	text = 'This is <gre/en>some test</> text <<to <yellow><blue>print out while</> testing different styles.</> This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</> This is <green>some test</> text to <yellow><blue>print <<>out while</> testing different styles.</>'
	textbox:setString ( text )
	
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
	
	takeScreenshot ( 'output/style1.png' )
	repeat coroutine.yield () until continue
	
	textbox:reserveCurves ( 1 )
	textbox:setCurve ( 1, curve )
	
	takeScreenshot ( 'output/style2.png' )
	repeat coroutine.yield () until continue
	textbox:setCurve ()
	
	textbox:nextPage ()
	
	takeScreenshot ( 'output/style3.png' )
	repeat coroutine.yield () until continue
	textbox:nextPage ()
	textbox:setVisible ( false )
	textbox1:setVisible ( true )
	textbox2:setVisible ( true )
	textbox3:setVisible ( true )
	
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	textbox1:setString ( text )
	textbox1:setStyle ( style1 )
	textbox1:setStyle ( 'red', style1 )
	textbox1:setStyle ( 'green', style2 )
	textbox1:setStyle ( 'blue', style3 )
	textbox1:setStyle ( 'yellow', style4 )
	
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	textbox2:setString ( text )
	textbox2:setStyle ( style1 )
	textbox2:setGlyphScale ( 2 )
	textbox2:setStyle ( 'red', style1 )
	textbox2:setStyle ( 'green', style2 )
	textbox2:setStyle ( 'blue', style3 )
	textbox2:setStyle ( 'yellow', style4 )
	
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	textbox3:setString ( text )
	textbox3:setStyle ( style1 )
	textbox3:setWordBreak ( MOAITextBox.WORD_BREAK_CHAR )
	textbox3:setStyle ( 'red', style1 )
	textbox3:setStyle ( 'green', style2 )
	textbox3:setStyle ( 'blue', style3 )
	textbox3:setStyle ( 'yellow', style4 )
	
	takeScreenshot ( 'output/style4.png' )
	repeat coroutine.yield () until continue
	textbox:setVisible ( true )
	textbox1:setVisible ( false )
	textbox2:setVisible ( false )
	textbox3:setVisible ( false )
	
	textbox:setReveal ( 25 )
	
	takeScreenshot ( 'output/style5.png' )
	repeat coroutine.yield () until continue
	textbox:revealAll ()
	
	textbox:setHighlight ( 25, 20, 1, 1, 1, 1 )
	
	takeScreenshot ( 'output/style6.png' )
	repeat coroutine.yield () until continue
	textbox:clearHighlights ()
	
	textbox:setLineSpacing ( 10 )
	
	takeScreenshot ( 'output/style7.png' )
	repeat coroutine.yield () until continue
	textbox:setLineSpacing ( 0 )
	
	text4 = 'Hello world. How are you today. I am doing well. How are you today. It seems peculiar that we are even having a conversation'
	textbox4:setString ( text4 )
	xmin, ymin, xmax, ymax = textbox4:getStringBounds ( 10, 50 )
	
	gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( "box.png" )
	gfxQuad:setRect ( xmin, ymin, xmax, ymax )
	gfxQuad:setUVRect ( 0, 0, 1, 1 )

	prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )
	
	takeScreenshot ( 'output/getStringBounds.png' )
	repeat coroutine.yield () until continue
	prop:setVisible ( false )
	
	-- staging is done
	print ( "Staging done. Please close the Moai window now." )
end

function getImagesTest ()
	print ( 'hello2' )
	-- font1 = MOAIFont.new ()
	-- font1:load ( 'arial-rounded.TTF' )
	-- font1:setDefaultSize ( 12, 163 )
	
	style1 = MOAITextStyle.new ()
	style1:setColor ( 1, 0, 0, 1 )
	style1:setFont ( font1 )
	style1:setSize ( 12 )
		
	-- textbox = MOAITextBox.new ()
	-- textbox:setFont ( font1 )
	-- textbox:setRect ( -150, -230, 150, 230 )
	-- textbox:setLineSpacing ( 10 )
	-- textbox:setGlyphScale ( 2 )
	textbox:setStyle ( style1 )
	textbox:setStyle ( 'red', style1 )	
	-- textbox:setYFlip ( true )
	-- layer:insertProp ( textbox )
			
	-- x, y, z, k = textbox:getRect ()
	-- evaluate ( x == -150 and y == -230 and z == 150 and k == 230, 'textbox getRect' )
	
	-- testStyle = textbox:getStyle ( 'red' )
	-- evaluate ( testStyle:getColor () == style1:getColor (), 'getStyle getColor' )
	-- evaluate ( testStyle:getFont () == style1:getFont (), 'getStyle getFont' )
	-- evaluate ( testStyle:getScale () == style1:getScale (), 'getStyle getScale' )
	-- evaluate ( testStyle:getSize () == style1:getSize (), 'getStyle getSize' )
	
	-- test affirmStyle with a style that's already created
	-- testAffirmStyle = textbox:affirmStyle ( )
	-- evaluate ( testAffirmStyle:getColor () == style1:getColor (), 'affirmStyle getColor' )
	-- evaluate ( testAffirmStyle:getFont () == style1:getFont (), 'getStyle getFont' )
	-- evaluate ( testAffirmStyle:getScale () == style1:getScale (), 'getStyle getScale' )
	-- evaluate ( testAffirmStyle:getSize () == style1:getSize (), 'getStyle getSize' )
	
	-- textbox2 = MOAITextBox.new ()
	
	-- test affirmStyle with a style that's set by affirmStyle
	-- testAffirmStyle2 = textbox2:affirmStyle ()
	-- styleDefault = textbox2:getStyle ()
	-- evaluate ( testAffirmStyle2:getColor () == styleDefault:getColor (), 'affirmStyle2 getColor' )
	-- evaluate ( testAffirmStyle2:getFont () == styleDefault:getFont (), 'affirmStyle2 getFont' )
	-- evaluate ( testAffirmStyle2:getScale () == styleDefault:getScale (), 'affirmStyle2 getScale' )
	-- evaluate ( testAffirmStyle2:getSize () == styleDefault:getSize (), 'affirmStyle2 getSize' )
	
	text = '1This is long text that will take multiple pages. This is long text that will take multiple 2pages. This is long text that will take multiple pages. This is long text that will take 3multiple pages. This is long text that will take multiple pages.'
	textbox:setString ( text )
	
	textbox:setRect ( -150, 100, 150, 230 )
	
	-- evaluate ( textbox:more () == true, 'textbox more' ) 
	-- evaluate ( textbox:getLineSpacing () == 10, 'texbox getLineSpacing' )
	-- evaluate ( textbox:getGlyphScale () == 2, 'textbox getGlyphScale' )

	action = MOAIAction.new ()
	timer = MOAITimer.new ()
	
	print ( 'hello3' )
	
	-- curves
	-- text = 'This text is curved. This text is curved. This text is curved. This text is curved. This text is curved. This text is curved.'
	-- textbox:setString ( text )
	
	-- curve = MOAIAnimCurve.new ()
	-- curve:reserveKeys ( 3 )
	-- curve:setKey ( 1, 0, 0 )
	-- curve:setKey ( 2, .5, 16 )
	-- curve:setKey ( 3, 1, 0 )
	
	-- curve2 = MOAIAnimCurve.new ()
	-- curve2:reserveKeys ( 2 )
	-- curve2:setKey ( 1, 0, 0 )
	-- curve2:setKey ( 2, 1, 24 )
	
	-- textbox:reserveCurves ( 1 )
	-- textbox:setCurve ( 1, curve )
	
	-- print ( 'hello3.5' )
	
	-- takeScreenshotTest () -- error -- crashes program
	
	if true then
		return
	end
	
	repeat coroutine.yield () until continue
	print ( 'hello 3.75' )
	img1 = MOAIImage.new ()
	
	img1:load ( "output/curve1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/curve1.png' ) 
		evaluate ( false, 'compare curve1' )
	end
	
	
	
	text = 'This text has two curves. This text has two curves. This text has two curves. This text has two curves. This text has two curves. This text has two curves.'
	textbox:setString ( text )
	
	textbox:reserveCurves ( 2 )
	textbox:setCurve ( 1, curve )
	textbox:setCurve ( 2, curve2 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/curve2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/curve2.png' ) 
		evaluate ( false, 'compare curve2' )
	end
	
	textbox:setCurve ()
	
	-- pages
	text = '1This is long text that will take multiple pages. This is long text that will take multiple 2pages. This is long text that will take multiple pages. This is long text that will take 3multiple pages. This is long text that will take multiple pages.'
	textbox:setString ( text )
	
	textbox:setRect ( -150, 100, 150, 230 )
	
	print ( 'hello4' )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/page1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/page1.png' ) 
		evaluate ( false, 'compare page1' )
	end
	
	textbox:nextPage ()
	
	takeScreenshot ( 'output/page2.png' )
	repeat coroutine.yield () until continue
	
	img1:load ( "output/page2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/page2.png' ) 
		evaluate ( false, 'compare page2' )
	end
	
	textbox:nextPage ()
	
	takeScreenshot ( 'output/page3.png' )
	repeat coroutine.yield () until continue
	
	img1:load ( "output/page3.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/page3.png' ) 
		evaluate ( false, 'compare page3' )
	end
	
	textbox:nextPage ()
	
	textbox:setRect ( -150, -230, 150, 230 )
	
	-- alignment
	textbox:setVisible ( false )
	textbox1:setVisible ( true )
	textbox2:setVisible ( true )
	textbox3:setVisible ( true )
	
	text = 'This text is left justified. This text is left justified.'
	textbox1:setString ( text )
	
	text = 'This text is center justified. This text is center justified.'
	textbox2:setString ( text )
	
	text = 'This text is right justified. This text is right justified.'
	textbox3:setString ( text )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/alignment1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/alignment1.png' ) 
		evaluate ( false, 'compare alignment1' )
	end
	
	text = 'This text is\n     left justified.'
	textbox1:setString ( text )
	text = 'This text is\n     center justified.'
	textbox2:setString ( text )
	text = 'This text is\n     right justified.'
	textbox3:setString ( text )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/alignment2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/alignment2.png' ) 
		evaluate ( false, 'compare alignment2' )
	end
	
	text = 'This text is     \nleft justified.'
	textbox1:setString ( text )
	text = 'This text is     \ncenter justified.'
	textbox2:setString ( text )
	text = 'This text is     \nright justified.'
	textbox3:setString ( text )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/alignment3.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/alignment3.png' ) 
		evaluate ( false, 'compare alignment3' )
	end
	
	textbox:setVisible ( true )
	textbox1:setVisible ( false )
	textbox2:setVisible ( false )
	textbox3:setVisible ( false )
	
	-- reveal
	text = '1234567890 1234567890 1234567890 1234567890 1234567890 1234567890'
	textbox:setString ( text )
	
	textbox:setRect ( -150, 70, -50, 230 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/reveal1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/reveal1.png' ) 
		evaluate ( false, 'compare reveal1' )
	end
	
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setReveal ( 25 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/reveal2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/reveal2.png' ) 
		evaluate ( false, 'compare reveal2' )
	end
	
	textbox:setReveal ( -5 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/reveal3.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/reveal3.png' ) 
		evaluate ( false, 'compare reveal3' )
	end
	
	textbox:revealAll ()
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/reveal4.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/reveal4.png' ) 
		evaluate ( false, 'compare reveal4' )
	end
	
	-- highlight
	text = '123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789  123456789'
	textbox:setString ( text )
	
	textbox:setRect ( -150, 70, 130, 230 )
	
	textbox:setHighlight ( 1, 5, 1, 0, 0 )
	textbox:setHighlight ( 16, 5, 1, 0, 0 )
	textbox:setHighlight ( 8, 5, 1, 0, 0 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/highlight1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/highlight1.png' ) 
		evaluate ( false, 'compare highlight1' )
	end
	
	textbox:setHighlight ( 3, 14, 1, 0, 0 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/highlight2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/highlight2.png' ) 
		evaluate ( false, 'compare highlight2' )
	end
	
	textbox:setHighlight ( 45, 21, 0, 0, 1 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/highlight3.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/highlight3.png' ) 
		evaluate ( false, 'compare highlight3' )
	end
	
	textbox:nextPage ()
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/highlight4.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/highlight4.png' ) 
		evaluate ( false, 'compare highlight4' )
	end
	
	textbox:nextPage ()
	textbox:clearHighlights ()
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/highlight5.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/highlight5.png' ) 
		evaluate ( false, 'compare highlight5' )
	end
	
	-- line spacing
	textbox:setLineSpacing ( -10 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/spacing1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/spacing1.png' ) 
		evaluate ( false, 'compare spacing1' )
	end
	
	textbox:setLineSpacing ( 10 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/spacing2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/spacing2.png' ) 
		evaluate ( false, 'compare spacing2' )
	end
	
	textbox:setLineSpacing ( 0 )
	
	-- spool speed
	text = 'Spooling at regular speed. Spooling at regular speed. Spooling at regular speed.'
	textbox:setString ( text )
	
	textbox:setSpeed ( 24 )
	textbox:spool ()
	
	takeScreenshotTest ( 1 )
	repeat coroutine.yield () until continue
	
	img1:load ( "output/spoolspeed1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/spoolspeed1.png' ) 
		evaluate ( false, 'compare spoolspeed1' )
	end
	
	textbox:setSpeed ( 48 )
	textbox:spool ()
	
	takeScreenshotTest ( 1 )
	repeat coroutine.yield () until continue
	
	img1:load ( "output/spoolspeed2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/spoolspeed2.png' ) 
		evaluate ( false, 'compare spoolspeed2' )
	end
	
	textbox:setSpeed ( -10 )
	textbox:spool ()
	
	takeScreenshotTest ( 1 )
	repeat coroutine.yield () until continue
	
	img1:load ( "output/spoolspeed3.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/spoolspeed3.png' ) 
		evaluate ( false, 'compare spoolspeed3' )
	end
	
	-- styles
	text = 'This is <gre/en>some test</> text <<to <yellow><blue>print out while</> testing different styles.</> This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</> This is <green>some test</> text to <yellow><blue>print <<>out while</> testing different styles.</>'
	textbox:setString ( text )
	
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
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/style1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/style1.png' ) 
		evaluate ( false, 'compare style1' )
	end
	
	textbox:reserveCurves ( 1 )
	textbox:setCurve ( 1, curve )
	
	takeScreenshot ( 'output/style2.png' )
	repeat coroutine.yield () until continue
	
	img1:load ( "output/style2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/style2.png' ) 
		evaluate ( false, 'compare style2' )
	end
	
	textbox:setCurve ()
	
	textbox:nextPage ()
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/style3.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/style3.png' ) 
		evaluate ( false, 'compare style3' )
	end
	
	textbox:nextPage ()
	textbox:setVisible ( false )
	textbox1:setVisible ( true )
	textbox2:setVisible ( true )
	textbox3:setVisible ( true )
	
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	textbox1:setString ( text )
	textbox1:setStyle ( style1 )
	textbox1:setStyle ( 'red', style1 )
	textbox1:setStyle ( 'green', style2 )
	textbox1:setStyle ( 'blue', style3 )
	textbox1:setStyle ( 'yellow', style4 )
	
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	textbox2:setString ( text )
	textbox2:setStyle ( style1 )
	textbox2:setGlyphScale ( 2 )
	textbox2:setStyle ( 'red', style1 )
	textbox2:setStyle ( 'green', style2 )
	textbox2:setStyle ( 'blue', style3 )
	textbox2:setStyle ( 'yellow', style4 )
	
	text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
	textbox3:setString ( text )
	textbox3:setStyle ( style1 )
	textbox3:setWordBreak ( MOAITextBox.WORD_BREAK_CHAR )
	textbox3:setStyle ( 'red', style1 )
	textbox3:setStyle ( 'green', style2 )
	textbox3:setStyle ( 'blue', style3 )
	textbox3:setStyle ( 'yellow', style4 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/style4.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/style4.png' ) 
		evaluate ( false, 'compare style4' )
	end
	
	textbox:setVisible ( true )
	textbox1:setVisible ( false )
	textbox2:setVisible ( false )
	textbox3:setVisible ( false )
	
	textbox:setReveal ( 25 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/style5.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/style5.png' ) 
		evaluate ( false, 'compare style5' )
	end
	
	textbox:revealAll ()
	
	textbox:setHighlight ( 25, 20, 1, 1, 1, 1 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/style6.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/style6.png' ) 
		evaluate ( false, 'compare style6' )
	end
	
	textbox:clearHighlights ()
	
	textbox:setLineSpacing ( 10 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/style7.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/style7.png' ) 
		evaluate ( false, 'compare style7' )
	end
	
	textbox:setLineSpacing ( 0 )
	
	text4 = 'Hello world. How are you today. I am doing well. How are you today. It seems peculiar that we are even having a conversation'
	textbox4:setString ( text4 )
	xmin, ymin, xmax, ymax = textbox4:getStringBounds ( 10, 50 )
	
	gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( "box.png" )
	gfxQuad:setRect ( xmin, ymin, xmax, ymax )
	gfxQuad:setUVRect ( 0, 0, 1, 1 )

	prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )
	
	takeScreenshot()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/getStringBounds.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/getStringBounds.png' ) 
		evaluate ( false, 'compare getStringBounds' )
	end
	
	prop:setVisible ( false )
	
	MOAITestMgr.endTest ( success )
end

function stage ()
	MOAISim.openWindow ( "MOAITextBox", 320, 480 )
	
	viewport = MOAIViewport.new ()
	viewport:setSize ( 320, 480 )
	viewport:setScale ( 320, 480 )

	layer = MOAILayer.new ()
	layer:setViewport ( viewport )
	MOAISim.pushRenderPass ( layer )
	
	charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

	font = MOAIFont.new ()
	font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 12, 163 )

	font1 = MOAIFont.new ()
	font1:load ( 'arial-rounded.TTF' )
	font1:setDefaultSize ( 12, 163 )

	font2 = MOAIFont.new ()
	font2:load ( 'Dwarves.TTF' )
	font2:preloadGlyphs ( charcodes, 24 )
	font2:setDefaultSize ( 12, 163 )

	font3 = MOAIFont.new ()
	font3:load ( 'EBOLA-KI.TTF' )
	font3:setDefaultSize ( 12, 163 )

	font4 = MOAIFont.new ()
	font4:load ( 'nanum_gothic_coding.ttf' )
	font4:setDefaultSize ( 12, 163 )
	
	textbox = MOAITextBox.new ()
	textbox:setFont ( font )
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	textbox1 = MOAITextBox.new ()
	textbox1:setFont ( font )
	textbox1:setTextSize ( 12, 163 )
	textbox1:setRect ( -150, 70, 150, 230 )
	textbox1:setAlignment ( MOAITextBox.LEFT_JUSTIFY )
	textbox1:setYFlip ( true )
	layer:insertProp ( textbox1 )
	
	textbox2 = MOAITextBox.new ()
	textbox2:setFont ( font )
	textbox2:setTextSize ( 12, 163 )
	textbox2:setRect ( -150, -90, 150, 70 )
	textbox2:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	textbox2:setYFlip ( true )
	layer:insertProp ( textbox2 )
	
	textbox3 = MOAITextBox.new ()
	textbox3:setFont ( font )
	textbox3:setTextSize ( 12, 163 )
	textbox3:setRect ( -150, -250, 150, -90 )
	textbox3:setAlignment ( MOAITextBox.RIGHT_JUSTIFY )
	textbox3:setYFlip ( true )
	layer:insertProp ( textbox3 )
	
	textbox4 = MOAITextBox.new ()
	textbox4:setFont ( font )
	textbox4:setTextSize ( 12, 163 )
	textbox4:setRect ( -150, -250, 150, -90 )
	textbox4:setYFlip ( true )
	layer:insertProp ( textbox4 )
	
	img = MOAIImage.new ()
	
	-- getImages ()
	thread = MOAIThread.new ()
	thread:run ( getImages )
end

function test ()
	MOAITestMgr.beginTest ( 'MOAITextBox' )
	success = true
	
	MOAISim.openWindow ( "MOAITextBox", 320, 480 )
	
	viewport = MOAIViewport.new ()
	viewport:setSize ( 320, 480 )
	viewport:setScale ( 320, 480 )

	layer = MOAILayer.new ()
	layer:setViewport ( viewport )
	MOAISim.pushRenderPass ( layer )
	
	print ( 'hello' )
	
	charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

	font = MOAIFont.new ()
	font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 12, 163 )

	font1 = MOAIFont.new ()
	font1:load ( 'arial-rounded.TTF' )
	font1:setDefaultSize ( 12, 163 )

	font2 = MOAIFont.new ()
	font2:load ( 'Dwarves.TTF' )
	font2:preloadGlyphs ( charcodes, 24 )
	font2:setDefaultSize ( 12, 163 )

	font3 = MOAIFont.new ()
	font3:load ( 'EBOLA-KI.TTF' )
	font3:setDefaultSize ( 12, 163 )

	font4 = MOAIFont.new ()
	font4:load ( 'nanum_gothic_coding.ttf' )
	font4:setDefaultSize ( 12, 163 )
	
	textbox = MOAITextBox.new ()
	textbox:setFont ( font )
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	textbox1 = MOAITextBox.new ()
	textbox1:setFont ( font )
	textbox1:setTextSize ( 12, 163 )
	textbox1:setRect ( -150, 70, 150, 230 )
	textbox1:setAlignment ( MOAITextBox.LEFT_JUSTIFY )
	textbox1:setYFlip ( true )
	layer:insertProp ( textbox1 )
	
	textbox2 = MOAITextBox.new ()
	textbox2:setFont ( font )
	textbox2:setTextSize ( 12, 163 )
	textbox2:setRect ( -150, -90, 150, 70 )
	textbox2:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
	textbox2:setYFlip ( true )
	layer:insertProp ( textbox2 )
	
	textbox3 = MOAITextBox.new ()
	textbox3:setFont ( font )
	textbox3:setTextSize ( 12, 163 )
	textbox3:setRect ( -150, -250, 150, -90 )
	textbox3:setAlignment ( MOAITextBox.RIGHT_JUSTIFY )
	textbox3:setYFlip ( true )
	layer:insertProp ( textbox3 )
	
	textbox4 = MOAITextBox.new ()
	textbox4:setFont ( font )
	textbox4:setTextSize ( 12, 163 )
	textbox4:setRect ( -150, -250, 150, -90 )
	textbox4:setYFlip ( true )
	layer:insertProp ( textbox4 )
	
	img = MOAIImage.new ()
	
	threadTest = MOAIThread.new ()
	threadTest:run ( getImagesTest )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
