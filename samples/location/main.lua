----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--[[
IMPORTANT NOTE

To make this run on Android, you need to enable location events in

	host-source/project/src/[YOUR/NAMESPACE]/MoaiActivity.java

by modifying enableLocationEvents ( false ) to

	enableLocationEvents ( true );

Further, you need to add those permissions

    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
	<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
	<uses-permission android:name="android.permission.ACCESS_LOCATION_EXTRA_COMMANDS" />
	<uses-permission android:name="android.permission.ACCESS_MOCK_LOCATION" />
	<uses-permission android:name="android.permission.CONTROL_LOCATION_UPDATES" />

to the manifest file in

	host-source/project/AndroidManifest.xml
]]--

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 16, 163 )

textbox = MOAITextBox.new ()
textbox:setFont ( font )
textbox:setTextSize ( 16, 163 )
textbox:setRect ( -150, -230, 150, 230 )
layer:insertProp ( textbox )

function dump( lng, lat )
	textbox:setString ( "lng: "..lng.."\nlat: "..lat )
	print ( "lng: "..lng..", lat: "..lat )
end

local lng, lat, ha, alt, va, speed = MOAIInputMgr.device.location:getLocation()
dump( lng, lat )

MOAIInputMgr.device.location:setCallback(
	function( lng, lat, ha, alt, va, speed )
		dump( lng, lat )
	end )
