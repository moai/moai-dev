----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, Android!" )

function onDialogDismissed ( code )
	print ( "onDialogDismissed: " )
	
	if ( code == MOAIDialog.DIALOG_RESULT_POSITIVE ) then
		print ( "dialog result: YES" )
	elseif ( code == MOAIDialog.DIALOG_RESULT_NEUTRAL ) then
		print ( "dialog result: MAYBE" )
	elseif ( code == MOAIDialog.DIALOG_RESULT_NEGATIVE ) then
		print ( "dialog result: NO" )
	elseif ( code == MOAIDialog.DIALOG_RESULT_CANCEL ) then
		print ( "dialog result: CANCELED" )
	else
		print ( "dialog result: UNKNOWN" )
	end
end

MOAIDialog.showDialog ( "A title", "A meaningful message", "Yes", "Maybe", "No", true, onDialogDismissed )

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
layer:insertProp ( prop )

prop:moveRot ( 540, 2.0 )
