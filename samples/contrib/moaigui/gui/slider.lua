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

	FILE: slider.lua
	DESCRIPTION: Allows the user to move a thumb value to set a numeric value
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11
]]

module(..., package.seeall)

require "gui\\support\\class"

local awindow = require "gui\\awindow"
local thumb = require "gui\\thumb"
local awidgetevent = require "gui\\awidgetevent"

Slider = class(awindow.AWindow)

function Slider:_createSliderValueChangedEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_SLIDER_VALUE_CHANGED, self)

	return t
end

function Slider:_setTextRect()
	local width = 1

	self._text:setRect(width)
end

function Slider:_setTextPos()
	local x, y

	if (self.ORIENTATION_HORZ == self._orientation) then
		if (self._valueDisplayLoc == self.VALUE_DISPLAY_TOP) then
			x = self:screenX()
			y = self:screenY() - self._text:height()

		elseif (self._valueDisplayLoc == self.VALUE_DISPLAY_BOTTOM) then
			x = self:screenX() + self:screenWidth() * 0.5
			y = self:screenY() + self:screenHeight()

		elseif (self._valueDisplayLoc == self.VALUE_DISPLAY_LEFT) then
			x = self:screenX() - 30
			y = self:screenY() - self._text:height() * 0.25

		elseif (self._valueDisplayLoc == self.VALUE_DISPLAY_RIGHT) then
			x = self:screenX() + self:screenWidth() + 5
			y = self:screenY() - self._text:height() * 0.25
		end
	else
		if (self._valueDisplayLoc == self.VALUE_DISPLAY_TOP) then
			x = self:screenX()
			y = self:screenY() - self._text:height()

		elseif (self._valueDisplayLoc == self.VALUE_DISPLAY_BOTTOM) then
			x = self:screenX()
			y = self:screenY() + self:screenHeight()

		elseif (self._valueDisplayLoc == self.VALUE_DISPLAY_LEFT) then
			x = self:screenX() - 30
			y = self:screenY() + self:screenHeight() * 0.5 - self._text:height() * 0.5

		elseif (self._valueDisplayLoc == self.VALUE_DISPLAY_RIGHT) then
			x = self:screenX() + self:screenWidth() + 5
			y = self:screenY() + self:screenHeight() * 0.5 - self._text:height() * 0.5
		end
	end

	self._text:setPos(x, -y)
end

function Slider:_onSetDim()
	if (self.ORIENTATION_HORZ == self._orientation) then
		self._thumb:setDim(5, self:height())
		self._thumb:setRange(0, self:width())
	else
		self._thumb:setDim(self:width(), 5)
		self._thumb:setRange(0, self:height())
	end

	self:setCurrValue(self._currValue)
end

function Slider:_handleThumbPosChanged(event)
	local thumbValue = self._thumb:getCurrValue()

	if (self.ORIENTATION_HORZ == self._orientation) then
		thumbValue = thumbValue / (self._thumb._maxValue - self._thumb:width() - self._thumb._minValue)
	else
		thumbValue = 1.0 - thumbValue / (self._thumb._maxValue - self._thumb:height() - self._thumb._minValue)
	end

	self._currValue = (self._maxValue - self._minValue) * thumbValue + self._minValue
	self._currValue = math.floor(self._currValue + 0.5)
	self:setText(tostring(self._currValue))

	local e = self:_createSliderValueChangedEvent()
	self:_handleEvent(self.EVENT_SLIDER_VALUE_CHANGED, e)

	return true
end

function Slider:setMaxValue(value)
	self._maxValue = value

	self:setCurrValue(self._currValue)
end

function Slider:getMaxValue()
	return self._maxValue
end

function Slider:setMinValue(value)
	self._minValue = value

	self:setCurrValue(self._currValue)
end

function Slider:getMinValue()
	return self._minValue
end

function Slider:setRange(minValue, maxValue)
	self:setMinValue(minValue)
	self:setMaxValue(maxValue)
end

function Slider:setOrientation(ori)
	if (self.ORIENTATION_HORZ == ori) then
		self._orientation = self.ORIENTATION_HORZ
		self._thumb:setOrientation(self._thumb.ORIENTATION_HORZ)
	else
		self._orientation = self.ORIENTATION_VERT
		self._thumb:setOrientation(self._thumb.ORIENTATION_VERT)
	end

	self:_setTextPos()
end

function Slider:getOrientation()
	return self._orientation
end

function Slider:setValueDisplayLoc(loc)
	self._valueDisplayLoc = loc

	self:_setTextPos()
end

function Slider:getValueDisplayLoc()
	return self._valueDisplayLoc
end

function Slider:setCurrValue(value)
	if (value < self._minValue) then
		value = self._minValue
	end

	if (value > self._maxValue) then
		value = self._maxValue
	end

	if (self.ORIENTATION_HORZ == self._orientation) then
		self._thumb:setPos(((self:width() - self._thumb:width()) / (self._maxValue - self._minValue)) * (value - self._minValue), 0)
	else
		self._thumb:setPos(0, self:height() - self._thumb:height() - ((self:height() - self._thumb:height()) / (self._maxValue - self._minValue)) * (value - self._minValue))
	end

	self._currValue = value
	self:setText(tostring(value))
end

function Slider:getCurrValue()
	return self._currValue
end

-- function Slider:setInterval(self, value)
	-- self._interval = value
-- end

-- function Slider:getInterval(self)
	-- return self._interval
-- end

function Slider:setBackgroundImage(image)
	self._quads[self._BASE_OBJECTS_INDEX][1]:setTexture(image)

	self._backgroundImage = image
end

function Slider:getBackgroundImage()
	return self._backgroundImage
end

function Slider:setThumbImages(normal, hover)
	self._thumb:setImages(normal, hover, normal, normal)
end

function Slider:getThumbNormalImage()
	return self._thumb:getNormalImage()
end

function Slider:getThumbHoverImage()
	return self._thumb:getHoverImage()
end

function Slider:_SliderEvents()
	self.EVENT_SLIDER_VALUE_CHANGED = "EventSliderValueChanged"
end

function Slider:init(gui)
	awindow.AWindow.init(self, gui)

	self:_SliderEvents()

	self._type = "Slider"

	self.ORIENTATION_HORZ = 0
	self.ORIENTATION_VERT = 1

	self.VALUE_DISPLAY_TOP = 0
	self.VALUE_DISPLAY_BOTTOM = 1
	self.VALUE_DISPLAY_LEFT = 2
	self.VALUE_DISPLAY_RIGHT = 3

	self._maxValue = 100
	self._minValue = 0
	self._currValue = 0
	self._interval = 5
	self._backgroundImage = nil
	self._orientation = self.ORIENTATION_VERT
	self._valueDisplayLoc = self.VALUE_DISPLAY_BOTTOM

	self._thumb = thumb.Thumb(gui)
	self:_addWidgetChild(self._thumb)
	self._thumb:setOrientation(self._thumb.ORIENTATION_VERT)

	self._thumb:registerEventHandler(self._thumb.EVENT_THUMB_POS_CHANGED, self, "_handleThumbPosChanged")

	self:setCurrValue(0)
	self:setTextAlignment(self.TEXT_ALIGN_LEFT)
end
