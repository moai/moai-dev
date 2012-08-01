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

	FILE: radiobutton.lua
	DESCRIPTION: The widget can either be selected or unselected; only one radio button in
		each group may be selected at any given time.
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

local groups = {}
local radioButtonsByGroup = {}

_M.RadioButton = class(abutton.AButton)

function _M.RadioButton:_createRadioButtonStateChangeEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_RADIO_BUTTON_STATE_CHANGE, self)

	return t
end

function _M.RadioButton:_setTextPos()
	self._text:setPos(self:screenWidth() + 5, 0)
end

function _M.RadioButton:_setTextAlignment()
	self._text:setAlignment(MOAITextBox.LEFT_JUSTIFY)
end

function _M.RadioButton:_setTextRect()
	if (#self._text:getString() > 0) then
		local width, _ = self._gui:_calcAbsValue(self._width, self._height)

		self._text:setRect(width)
	end
end

function _M.RadioButton:_calcDim(width, height)
	local widgetWidth = height * (self._gui._height / self._gui._width)
	local widgetHeight = height

	return widgetWidth, widgetHeight
end

function _M.RadioButton:_setSelected(event, flag)
	local prevValue = self._selected
	self._selected = flag
	self:_handleEvent(event, self.EVENT_RADIO_BUTTON_STATE_CHANGE)

	if (true == self._selected) then
		self:_setCurrImages(self._SELECTED_INDEX, self.SELECTED_IMAGES)
	else
		self:_setCurrImages(self._SELECTED_INDEX, nil)
	end

	if (true == self._selected) then
		group = self:getGroup()
		buttons = groups[group]
		if (nil == buttons) then
			return
		end

		for i, v in ipairs(buttons) do
			if (v ~= self) then
				v:setSelected(false)
			end
		end
	end

	local e = self:_createRadioButtonStateChangeEvent()
	e.currValue = self._selected
	e.prevValue = prevValue

	return self:_handleEvent(self.EVENT_RADIO_BUTTON_STATE_CHANGE, e)
end

function _M.RadioButton:_inputClick(event)
	if (false == self._visible or false == self._enabled) then
		return
	end
	
	return self:_setSelected(event, true)
end

function _M.RadioButton:setSelected(flag)
	self:_setSelected({}, flag)
end

function _M.RadioButton:getSelected()
	return self._selected
end

function _M.RadioButton:setSelectedImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._SELECTED_INDEX, self.SELECTED_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)

	if (true == self._selected) then
		self:_setCurrImages(self._SELECTED_INDEX, self.SELECTED_IMAGES)
	end
end

function _M.RadioButton:setImages(normal, hover, pushed, disabled, selected)
	self:setNormalImage(normal)
	self:setHoverImage(hover)
	self:setPushedImage(pushed)
	self:setDisabledImage(disabled)
	self:setSelectedImage(selected)
end

function _M.RadioButton:setGroup(group)
	local buttons = groups[group]

	if (nil == buttons) then
		buttons = {}
		groups[group] = buttons
	end

	table.insert(buttons, self)
	
	self._group = group
end

function _M.RadioButton:getGroup()
	return self._group
end

function _M.RadioButton:_RadioButtonEvents()
	self.EVENT_RADIO_BUTTON_STATE_CHANGE = "EventRadioButtonStateChange"
end

function _M.RadioButton:init(gui)
	abutton.AButton.init(self, gui)

	self:_RadioButtonEvents()

	self._SELECTED_INDEX = self._BUTTON_INDEX + 1
	self.SELECTED_IMAGES = self.DISABLED_IMAGES + 1

	self._type = "RadioButton"
	self._selected = false

	self._group = nil
end

return _M
