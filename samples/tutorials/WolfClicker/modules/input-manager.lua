--==============================================================
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
--==============================================================

module ( "inputmgr", package.seeall )

----------------------------------------------------------------
-- local interface
----------------------------------------------------------------
local pointerX, pointerY = nil, nil

if MOAIInputMgr.device.pointer then

	local pointerCallback = function ( x, y )
		
		pointerX, pointerY = x, y
		
		if touchCallbackFunc then
			touchCallbackFunc ( MOAITouchSensor.TOUCH_MOVE, 1, pointerX, pointerY, 1 )
		end
	end

	MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
end

local keys = {}
if MOAIInputMgr.device.keyboard then
	
	local keyCallback = function ( key, down )
	
		if down then table.insert ( keys, key ) end
	end
	
	MOAIInputMgr.device.keyboard:setCallback ( keyCallback )
end

charForCode = {
	[ 97 ] = "a",
	[ 98 ] = "b",
	[ 99 ] = "c",
	[ 100 ] = "d",
	[ 101 ] = "e",
	[ 102 ] = "f",
	[ 103 ] = "g",
	[ 104 ] = "h",
	[ 105 ] = "i",
	[ 106 ] = "j",
	[ 107 ] = "k",
	[ 108 ] = "l",
	[ 109 ] = "m",
	[ 110 ] = "n",
	[ 111 ] = "o",
	[ 112 ] = "p",
	[ 113 ] = "q",
	[ 114 ] = "r",
	[ 115 ] = "s",
	[ 116 ] = "t",
	[ 117 ] = "u",
	[ 118 ] = "v",
	[ 119 ] = "w",
	[ 120 ] = "x",
	[ 121 ] = "y",
	[ 122 ] = "z",
	[ 48 ] = "0",
	[ 49 ] = "1",
	[ 50 ] = "2",
	[ 51 ] = "3",
	[ 52 ] = "4",
	[ 53 ] = "5",
	[ 54 ] = "6",
	[ 55 ] = "7",
	[ 56 ] = "8",
	[ 57 ] = "9",
	[ 32 ] = " "
}

----------------------------------------------------------------
-- public interface
----------------------------------------------------------------
function down ( )
	
	if MOAIInputMgr.device.touch then	
		
		return MOAIInputMgr.device.touch:down ()
		
	elseif MOAIInputMgr.device.pointer then
		
		return (	
			MOAIInputMgr.device.mouseLeft:down ()
		)
	end
end

function getKey ()
	
	if MOAIInputMgr.device.keyboard then
		
		local key = keys [ 1 ]
		table.remove ( keys, 1 )
		return key
	end
end

function getTouch ()
	
	if MOAIInputMgr.device.touch then	

		return MOAIInputMgr.device.touch:getTouch ()
		
	elseif MOAIInputMgr.device.pointer then
	
		return pointerX, pointerY, 1
	end
end

function isDown ( )
	
	if MOAIInputMgr.device.touch then	
		
		return MOAIInputMgr.device.touch:isDown ()
		
	elseif MOAIInputMgr.device.pointer then
		
		return (	
			MOAIInputMgr.device.mouseLeft:isDown ()
		)
	end
end

function up ( )
	
	if MOAIInputMgr.device.touch then	
		
		return MOAIInputMgr.device.touch:up ()
		
	elseif MOAIInputMgr.device.pointer then
		
		return (	
			MOAIInputMgr.device.mouseLeft:up ()
		)
	end
end
