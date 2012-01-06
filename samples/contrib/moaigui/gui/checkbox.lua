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
]]

module(..., package.seeall)

require "gui\\support\\class"

local abutton = require "gui\\abutton"
local constants = require "gui\\guiconstants"
local awidgetevent = require "gui\\awidgetevent"

CheckBox = class(abutton.AButton)

function CheckBox:_createCheckBoxStateChangeEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_CHECK_BOX_STATE_CHANGE, self)

	return t
end

function CheckBox:_setTextPos()
	local x = self:screenX() + self:screenWidth() + 5
	local y = -self:screenY() + (self._text:height() - self:screenHeight()) * 0.5

	self._text:setPos(x, y)
end

function CheckBox:_setTextAlignment()
	self._text:setAlignment(MOAITextBox.LEFT_JUSTIFY)
end

function CheckBox:_setTextRect()
	if (#self._text:getString() > 0) then
		local width = self:screenWidth()

		self._text:setRect(width)
	end
end

function CheckBox:setDim(width, height)
	self._width = width
	self._height = height

	local widgetWidth = height * (self._gui._height / self._gui._width)
	local widgetHeight = height

	self._screenWidth, self._screenHeight = self._gui:_calcAbsValue(widgetWidth, widgetHeight)

	for i, v in ipairs(self._quads) do
		for i2, v2 in ipairs(v) do
			v2:setRect(0, -self._screenHeight, self._screenWidth, 0)
		end
	end

	self:_setTextPos()
	self:_setTextRect()
	self:_setTextAlignment()

	self:_onSetDim()
end

function CheckBox:_setChecked(event, flag)
	self._checked = flag
	self:_handleEvent(event, self.EVENT_CHECK_BOX_STATE_CHANGE)

	if (true == self._checked) then
		self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX][1]:setTexture(self._checkedImage)
	else
		self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX][1]:setTexture(nil)
	end

	local e = self:_createCheckBoxStateChangeEvent()

	return self:_handleEvent(self.EVENT_CHECK_BOX_STATE_CHANGE, e)
end

function CheckBox:_inputClick(event)
	if (false == self._visible or false == self._enabled) then
		return
	end
	
	return self:_setChecked(event, not self._checked)
end

function CheckBox:setChecked(flag)
	self:_setChecked({}, flag)
end

function CheckBox:getChecked()
	return self._checked
end

function CheckBox:setCheckedImage(image)
	self._checkedImage = image

	if (true == self._checked) then
		self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX][1]:setTexture(self._checkedImage)
	end
end

function CheckBox:setImages(normal, hover, pushed, disabled, checked)
	self:setNormalImage(normal)
	self:setHoverImage(hover)
	self:setPushedImage(pushed)
	self:setDisabledImage(disabled)
	self:setCheckedImage(checked)
end

function CheckBox:_CheckBoxEvents()
	self.EVENT_CHECK_BOX_STATE_CHANGE = "EventCheckBoxStateChange"
end

function CheckBox:init(gui)
	abutton.AButton.init(self, gui)

	self:_CheckBoxEvents()

	self._type = "CheckBox"
	self._checked = false
	checkQuad, checkProp = self._gui:_createRenderObject(self._priority + 1)
	self._gui:_registerHitObject(self, checkProp)
	table.insert(self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX], checkQuad)
	table.insert(self._props[self._WIDGET_SPECIFIC_OBJECTS_INDEX], checkProp)
end
