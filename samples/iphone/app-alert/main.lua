----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, iPhone!" )

function onDialogDismissed ( index )
	print ( "onDialogDismissed: " )	

	-- NOTE: Indexes are 1 based in the order that the buttons are specified in 
	-- the call to alert, INCLUDING the cancel button. If nil is passed for the
	-- cancel button, then the index of the first button title provided is one;
	-- otherwise, it is two.
	print ( "button at index " .. index .. " clicked" )
end

MOAIApp.alert ( "A title", "A meaningful message", onDialogDismissed, nil, "Yes", "Later", "Never" )

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
