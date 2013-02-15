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
	print ( MOAIKeyboardAndroid.getText ())
end

function onReturn ()
	print ( 'on return' )
	print ( MOAIKeyboardAndroid.getText ())
end

MOAIKeyboardAndroid.setListener ( MOAIKeyboardAndroid.EVENT_INPUT, onInput )
MOAIKeyboardAndroid.setListener ( MOAIKeyboardAndroid.EVENT_RETURN, onReturn )
MOAIKeyboardAndroid.showKeyboard ()