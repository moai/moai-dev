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

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setLoc ( 0, 80 )
layer:insertProp ( prop )

font = MOAIFont.new ()
font:load("arialbd.ttf")
font:preloadGlyphs (" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.?!", 12 )

	local style = MOAITextStyle.new ()
	style:setFont ( font )
	style:setSize ( 12 )



textbox = MOAITextBox.new ()
textbox:setStyle (style)
textbox:setFont ( font )
textbox:setRect ( -160, -80, 160, 80 )
textbox:setLoc ( 0, -100 )
textbox:setYFlip ( true )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
layer:insertProp ( textbox )



textbox:setString ( "Moai<c:0F0>Meow.<c>" )
textbox:spool ()

function twirlingTowardsFreedom ()
	while true do
		MOAIThread.blockOnAction ( prop:moveRot ( 360, 1.5 ))
		MOAIThread.blockOnAction ( prop:moveRot ( -360, 1.5 ))
	end
end

thread = MOAIThread.new ()
thread:run ( twirlingTowardsFreedom )
