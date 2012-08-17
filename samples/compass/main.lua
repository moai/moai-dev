----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--[[
IMPORTANT NOTE

To make this run on Android, you need to enable accelerometer events in

	host-source/project/src/[YOUR/NAMESPACE]/MoaiActivity.java

by modifying enableAccelerometerEvents ( false ) to

	enableAccelerometerEvents ( true );
]]--

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "rocket.png" )
gfxQuad:setRect ( -32, -32, 32, 32 )
gfxQuad:setUVRect ( 1, 1, 0, 0 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 16, 163 )

textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( 16, 163 )
textbox:setRect ( -150, -230, 150, 230 )
layer:insertProp ( textbox )

function dump( heading )
	textbox:setString ( "heading: "..heading )
	print ( "heading: "..heading )
	prop:setRot ( -heading )
end

local heading = MOAIInputMgr.device.compass:getHeading()
dump( heading )

MOAIInputMgr.device.compass:setCallback(
	function( heading )
		dump( heading )
	end )
