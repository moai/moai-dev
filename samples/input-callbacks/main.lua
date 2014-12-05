----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

print ( MOAIInputMgr.configuration )

----------------------------------------------------------------
-- keyboard events

function onKeyboardEvent ( key, down )

	if down == true then
		printf ( "keyboard: %d down\n", key )
	else
		printf ( "keyboard: %d up\n", key )
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )

----------------------------------------------------------------
-- pointer events

function onPointerEvent ( x, y )
	printf ( "pointer: %d %d\n", x, y )
end

MOAIInputMgr.device.pointer:setCallback ( onPointerEvent )

----------------------------------------------------------------
-- mouse left button events

function onMouseLeftEvent ( down )

	if down == true then
		printf ( "mouse left down\n" )
	else
		printf ( "mouse left up\n" )
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )

----------------------------------------------------------------
-- mouse middle button events

function onMouseMiddleEvent ( down )

	if down == true then
		printf ( "mouse middle down\n" )
	else
		printf ( "mouse middle up\n" )
	end
end

MOAIInputMgr.device.mouseMiddle:setCallback ( onMouseMiddleEvent )

----------------------------------------------------------------
-- mouse right button events

function onMouseRightEvent ( down )

	if down == true then
		printf ( "mouse right down\n" )
	else
		printf ( "mouse right up\n" )
	end
end

MOAIInputMgr.device.mouseRight:setCallback ( onMouseRightEvent )

