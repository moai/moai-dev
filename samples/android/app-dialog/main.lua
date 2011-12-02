----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, Android!" )

function onDialogDismissed ( code )
	print ( "onDialogDismissed: " )
	
	if ( code == MOAIApp.DIALOG_RESULT_POSITIVE ) then
		print ( "dialog result: YES" )
	elseif ( code == MOAIApp.DIALOG_RESULT_NEUTRAL ) then
		print ( "dialog result: MAYBE" )
	elseif ( code == MOAIApp.DIALOG_RESULT_NEGATIVE ) then
		print ( "dialog result: NO" )
	else
		print ( "dialog result: CANCELED" )
	end
end

MOAIApp.setListener ( MOAIApp.DIALOG_DISMISSED, onDialogDismissed )

MOAIApp.showDialog ( "A title", "A meaningful message", "OK", "Maybe", "No", false )

----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 540, 2.0 )
