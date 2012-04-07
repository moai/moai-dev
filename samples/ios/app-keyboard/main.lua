----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'KEYBOARD' )
MOAIGfxDevice.setClearColor ( 1, 1, 1 )

function onInput ( start, length, text )
	print ( 'on input' )
	print ( start, length, text )
	print ( MOAIKeyboardIOS.getText ())
end

function onReturn ()
	print ( 'on return' )
	print ( MOAIKeyboardIOS.getText ())
end

MOAIKeyboardIOS.setListener ( MOAIKeyboardIOS.EVENT_INPUT, onInput )
MOAIKeyboardIOS.setListener ( MOAIKeyboardIOS.EVENT_RETURN, onReturn )
MOAIKeyboardIOS.showKeyboard ()