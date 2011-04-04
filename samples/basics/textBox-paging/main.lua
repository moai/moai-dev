----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text1 = "a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
text2 = "1 2 3 4 5 6 7 8 9 0 a i e o u sa shi se su so ma mi me mu mo ta chi te tsu to ka ki ke ku ko ra ri re ru ro"

font = MOAIFont.new ()
font:loadFromTTF ( 'EBOLA-KI.TTF', charcodes, 16, 163 )

textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( -150, 100, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )

function page ( text )

	-- start spooling the text
	textbox:setString ( text )
	textbox:spool ()

	-- more will be true as long as there is any remaining text
	while textbox:more () do
		
		-- wait until we're done spooling the page
		while textbox:isBusy () do coroutine.yield () end
		
		-- if there's more to spool, show the next page
		if textbox:more () then
		
			-- this shows the next page and restarts the spool action
			textbox:nextPage ()
			textbox:spool ()
		end
	end
end

function main ()

	page ( text1 )
	page ( text2 )
end

thread = MOAIThread.new ()
thread:run ( main )

MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( 'test', 320, 480 )
