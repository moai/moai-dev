--[[
* The MIT License
* Copyright (C) 2011 Derick Dong (derickdong@hotmail.com).  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	FILE: thumb.lua
	DESCRIPTION: A widget that moves along a track (sliders and scroll bars)
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
]]

local _M = {}

require "gui\\support\\class"

local abutton = require "gui\\abutton"
local awidgetevent = require "gui\\awidgetevent"

_M.Thumb = class(abutton.AButton)

function _M.Thumb:_createThumbPosChangedEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_THUMB_POS_CHANGED, self)

	return t
end

function _M.Thumb:_createThumbMoveStartEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_THUMB_MOVE_START, self)

	return t
end

function _M.Thumb:_createThumbMoveEndEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_THUMB_MOVE_END, self)

	return t
end

function _M.Thumb:_createThumbIncreaseEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_THUMB_INCREASE, self)

	return t
end

function _M.Thumb:_createThumbDecreaseEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_THUMB_DECREASE, self)

	return t
end

function _M.Thumb:_onHandleDragStart(event)
	local e = self:_createThumbMoveStartEvent()
	return self:_handleEvent(self.THUMB_MOVE_START, e)
end

function _M.Thumb:_onHandleDragEnd(event)
	local e = self:_createThumbMoveEndEvent()
	return self:_handleEvent(self.THUMB_MOVE_END, e)
end

function _M.Thumb:_onHandleMouseMove(event)
	if (false == self._dragging) then
		return false
	end

	local x = 0
	local y = 0
	local prevValue = self._currValue

	local mouseX, mouseY = self._gui:_calcRelValue(event.x, event.y)

	if (self.ORIENTATION_HORZ == self._orientation) then
		x = self:x()
		y = 0

		if (x > self._maxValue - self:width()) then
			x = self._maxValue - self:width()
		end
		
		if (x < self._minValue) then
			x = self._minValue
		end

		if (mouseX < self._parent:fullX()) then
			x = 0
		end

		if (mouseX > x + self._parent:fullX() + self:width()) then
			x = self._maxValue - self:width()
		end
		
		self._currValue = x

	else
		x = 0
		y = self:y()

		if (y > self._maxValue - self:height()) then
			y = self._maxValue - self:height()
		end
		
		if (y < self._minValue) then
			y = self._minValue
		end

		if (mouseY < self._parent:fullY()) then
			y = 0
		end

		if (mouseY > y + self._parent:fullY() + self:height()) then
			y = self._maxValue - self:height()
		end
		
		self._currValue = y
	end

	self:setPos(x, y)

	local e = self:_createThumbPosChangedEvent()
	self:_handleEvent(self.EVENT_THUMB_POS_CHANGED, e)

	if (prevValue < self._currValue) then
		local e = self:_createThumbIncreaseEvent()
		self:_handleEvent(self.EVENT_THUMB_INCREASE, e)

	elseif (prevValue > self._currValue) then
		local e = self:_createThumbDecreaseEvent()
		self:_handleEvent(self.EVENT_THUMB_DECREASE, e)
	end

	return false
end

function _M.Thumb:setOrientation(orientation)
	self._orientation = orientation
end

function _M.Thumb:setRange(minValue, maxValue)
	self._minValue = minValue
	self._maxValue = maxValue
end

function _M.Thumb:setCurrValue(value)

end

function _M.Thumb:getCurrValue()
	return self._currValue
end

function _M.Thumb:_ThumbEvents()
	self.EVENT_THUMB_POS_CHANGED = "EventThumbPosChanged"
	self.EVENT_THUMB_MOVE_START = "EventThumbMoveStart"
	self.EVENT_THUMB_MOVE_END = "EventThumbMoveEnd"
	self.EVENT_THUMB_INCREASE = "EventThumbIncrease"
	self.EVENT_THUMB_DECREASE = "EventThumbDecrease"
end

function _M.Thumb:init(gui)
	abutton.AButton.init(self, gui)

	self:_ThumbEvents()

	self._type = "Thumb"

	self.ORIENTATION_HORZ = 0
	self.ORIENTATION_VERT = 1

	self._orientation = self.ORIENTATION_VERT
	self._minValue = 0
	self._maxValue = 0
	self._currValue = 0

	self:setDraggable(true)
end

return _M
