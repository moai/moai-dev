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
]]

module(..., package.seeall)

require "gui\\support\\class"

local abutton = require "gui\\abutton"
local awidgetevent = require "gui\\awidgetevent"

local groups = {}
local radioButtonsByGroup = {}

RadioButton = class(abutton.AButton)

function RadioButton:_createRadioButtonStateChangeEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_RADIO_BUTTON_STATE_CHANGE, self)

	return t
end

function RadioButton:_setTextPos()
	local x = self:screenX() + self:screenWidth() + 5
	local y = -self:screenY() + (self._text:height() - self:screenHeight()) * 0.5

	self._text:setPos(x, y)
end

function RadioButton:_setTextAlignment()
	self._text:setAlignment(MOAITextBox.LEFT_JUSTIFY)
end

function RadioButton:_setTextRect()
	if (#self._text:getString() > 0) then
		local width = self:screenWidth()

		self._text:setRect(width)
	end
end

function RadioButton:setDim(width, height)
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

function RadioButton:_setSelected(event, flag)
	local prevValue = self._selected
	self._selected = flag
	self:_handleEvent(event, self.EVENT_RADIO_BUTTON_STATE_CHANGE)

	if (true == self._selected) then
		self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX][1]:setTexture(self._selectedImage)
	else
		self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX][1]:setTexture(nil)
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

function RadioButton:_inputClick(event)
	if (false == self._visible or false == self._enabled) then
		return
	end
	
	return self:_setSelected(event, true)
end

function RadioButton:setSelected(flag)
	self:_setSelected({}, flag)
end

function RadioButton:getSelected()
	return self._selected
end

function RadioButton:setSelectedImage(image)
	self._selectedImage = image

	if (true == self._selected) then
		self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX][1]:setTexture(self._selectedImage)
	end
end

function RadioButton:setImages(normal, hover, pushed, disabled, checked)
	self:setNormalImage(normal)
	self:setHoverImage(hover)
	self:setPushedImage(pushed)
	self:setDisabledImage(disabled)
	self:setSelectedImage(checked)
end

function RadioButton:setGroup(group)
	local buttons = groups[group]

	if (nil == buttons) then
		buttons = {}
		groups[group] = buttons
	end

	table.insert(buttons, self)
	
	self._group = group
end

function RadioButton:getGroup()
	return self._group
end

function RadioButton:_RadioButtonEvents()
	self.EVENT_RADIO_BUTTON_STATE_CHANGE = "EventRadioButtonStateChange"
end

function RadioButton:init(gui)
	abutton.AButton.init(self, gui)

	self:_RadioButtonEvents()

	self._type = "RadioButton"
	self._selected = false
	local selectedQuad, selectedProp = self._gui:_createRenderObject(self._priority + 1)
	self._gui:_registerHitObject(self, selectedProp)
	table.insert(self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX], selectedQuad)
	table.insert(self._props[self._WIDGET_SPECIFIC_OBJECTS_INDEX], selectedProp)

	self._RADIO_BUTTON_IMAGE_INDEX = 2

	self._group = nil

end
