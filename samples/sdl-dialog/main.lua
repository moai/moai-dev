----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( 'test', 320, 480 )

buttons = {

	{
		text = 'foo',
		func = function () print ( 'FOO' ) end,
	},

	{
		text = 'bar',
		default = true,	
	},

	'baz',
}

function onButton ( buttonid )
	local button = buttons [ buttonid ]
	local text = ( type ( button ) == 'table' ) and button.text or button
	print ( 'ON BUTTON', buttonid, text )
end

function onMouseLeftEvent ( down )
	if down then
		MOAIDialogSDL.show ( 'Dialog Title', 'This is the dialog message.', buttons, onButton )
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )