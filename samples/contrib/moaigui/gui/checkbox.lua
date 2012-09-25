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

	FILE: checkbox.lua
	DESCRIPTION: The widget can either be checked or unchecked
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

_M.CheckBox = class(abutton.AButton)

function _M.CheckBox:_createCheckBoxStateChangeEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_CHECK_BOX_STATE_CHANGE, self)

	return t
end

function _M.CheckBox:_setTextPos()
	self._text:setPos(self:screenWidth() + 5, 0)
end

function _M.CheckBox:_setTextAlignment()
	self._text:setAlignment(MOAITextBox.LEFT_JUSTIFY)
end

function _M.CheckBox:_setTextRect()
	if (#self._text:getString() > 0) then
		local width, _ = self._gui:_calcAbsValue(self._width, self._height)

		self._text:setRect(width)
	end
end

function _M.CheckBox:_calcDim(width, height)
	local widgetWidth = height * (self._gui._height / self._gui._width)
	local widgetHeight = height

	return widgetWidth, widgetHeight
end

function _M.CheckBox:_setChecked(event, flag)
	self._checked = flag
	self:_handleEvent(event, self.EVENT_CHECK_BOX_STATE_CHANGE)

	if (true == self._checked) then
		self:_setCurrImages(self._CHECKED_INDEX, self.CHECKED_IMAGES)
	else
		self:_setCurrImages(self._CHECKED_INDEX, nil)
	end

	local e = self:_createCheckBoxStateChangeEvent()

	return self:_handleEvent(self.EVENT_CHECK_BOX_STATE_CHANGE, e)
end

function _M.CheckBox:_inputClick(event)
	if (false == self._visible or false == self._enabled) then
		return
	end
	
	return self:_setChecked(event, not self._checked)
end

function _M.CheckBox:setChecked(flag)
	self:_setChecked({}, flag)
end

function _M.CheckBox:getChecked()
	return self._checked
end

function _M.CheckBox:setCheckedImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._CHECKED_INDEX, self.CHECKED_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)

	if (true == self._checked) then
		self:_setCurrImages(self._CHECKED_INDEX, self.CHECKED_IMAGES)
	end
end

function _M.CheckBox:setImages(normal, hover, pushed, disabled, checked)
	self:setNormalImage(normal)
	self:setHoverImage(hover)
	self:setPushedImage(pushed)
	self:setDisabledImage(disabled)
	self:setCheckedImage(checked)
end

function _M.CheckBox:_CheckBoxEvents()
	self.EVENT_CHECK_BOX_STATE_CHANGE = "EventCheckBoxStateChange"
end

function _M.CheckBox:init(gui)
	abutton.AButton.init(self, gui)

	self:_CheckBoxEvents()

	self._CHECKED_INDEX = self._BUTTON_INDEX + 1
	self.CHECKED_IMAGES = self.DISABLED_IMAGES + 1

	self._type = "CheckBox"
	self._checked = false
end

return _M
