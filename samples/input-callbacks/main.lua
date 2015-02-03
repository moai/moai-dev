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

local keyNames = {}
for name, value in pairs ( MOAIKeyCode ) do
	if type( value ) == "number" then
		keyNames [ value ] = name
	end
end

function onKeyboardKeyEvent ( key, down )
	local keyInfo = keyNames [ key ] or tostring ( key )
	if ( key < 256) then
		keyInfo = keyInfo .. string.format( " ('%s')", string.char ( key ))
	end
	printf ( "keyboard: key %s %s\n", keyInfo, down and "down" or "up" )
end

function onKeyboardCharEvent ( c )
	printf ( "keyboard: character '%s'\n", c )
	if ( string.len ( c ) > 1 ) then
		printf ( "This UTF-8 sequence might not print correctly on a console.\n" )
	end
end

MOAIInputMgr.device.keyboard:setKeyCallback ( onKeyboardKeyEvent )
MOAIInputMgr.device.keyboard:setCharCallback ( onKeyboardCharEvent )

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

