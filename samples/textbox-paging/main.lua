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
text1 = "a b c d e f g h i j k l m n o p q r s t u v w x y z <c:fc0>A B C D E F G H I J K L M N O P Q R <c:0cf>S T U V W X Y Z"
text2 = "<c:0f0>1 2 3 4 5 6 7 8 9 0 <c:f00>a i e o u <c:00f>sa shi se su so ma mi me mu mo <c:0f0>ta chi te tsu to ka ki ke ku ko <c:fc0>ra ri re ru ro"

font = MOAIFont.new ()
font:loadFromTTF ( 'EBOLA-KI.TTF', charcodes, 16, 163 )

textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( 16, 163 )
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

