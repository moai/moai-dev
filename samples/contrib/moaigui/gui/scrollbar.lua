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

	FILE: gui.lua
	DESCRIPTION: The managing class for the gui system
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	BUGS
	- setTopItem - setting the thumb position
]]

module(..., package.seeall)

require "gui\\support\\class"

local awindow = require "gui\\awindow"
local thumb = require "gui\\thumb"
local awidgetevent = require "gui\\awidgetevent"

ScrollBar = class(awindow.AWindow)

function ScrollBar:_createScrollBarPosChangedEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_SCROLL_BAR_POS_CHANGED, self)

	return t
end

function ScrollBar:_setTextRect()
	self._text:setRect(0)
end

function ScrollBar:_onSetDim()
	if (self.ORIENTATION_HORZ == self._orientation) then
		self._thumb:setDim(5, self:height())
		self._thumb:setRange(0, self:width())
	else
		self._thumb:setDim(self:width(), 5)
		self._thumb:setRange(0, self:height())
	end
end

function ScrollBar:_handleThumbPosChanged(event)
	if (self._numItems <= self._pageSize) then
		self._thumb:setPos(0, 0)
		return
	end

	local thumbValue = self._thumb:getCurrValue()

	if (self.ORIENTATION_HORZ == self._orientation) then
		thumbValue = thumbValue / (self._thumb._maxValue - self._thumb:width() - self._thumb._minValue)
	else
		thumbValue = thumbValue / (self._thumb._maxValue - self._thumb:height() - self._thumb._minValue)
	end

	self._topItem = (self._numItems - self._pageSize) * thumbValue + 1
	self._topItem = math.floor(self._topItem + 0.5)

	local e = self:_createScrollBarPosChangedEvent()
	self:_handleEvent(self.EVENT_SCROLL_BAR_POS_CHANGED, e)

	return true
end

function ScrollBar:setNumItems(value)
	self._numItems = value
end

function ScrollBar:getNumItems()
	return self._numItems
end

function ScrollBar:setPageSize(value)
	self._pageSize = value
end

function ScrollBar:getPageSize()
	return self._pageSize
end

function ScrollBar:setOrientation(ori)
	if (self.ORIENTATION_HORZ == ori) then
		self._orientation = self.ORIENTATION_HORZ
		self._thumb:setOrientation(self._thumb.ORIENTATION_HORZ)
	else
		self._orientation = self.ORIENTATION_VERT
		self._thumb:setOrientation(self._thumb.ORIENTATION_VERT)
	end
end

function ScrollBar:getOrientation()
	return self._orientation
end

function ScrollBar:setTopItem(value)
	if (self._numItems < self._pageSize) then
		self._topItem = 1
		self._thumb:setPos(0, 0)

		return
	end

	if (value + self._pageSize > self._numItems) then
		value = self._numItems - self._pageSize
	end

	if (value < 1) then
		value = 1
	end

	if (self.ORIENTATION_HORZ == self._orientation) then
		-- self._thumb:setPos(((self:width() - self._thumb:width()) / (self._numItems - self._pageSize - 1)) * (value - 1), 0)
		self._thumb:setPos((self:width() - self._thumb:width()) * (value - 1), 0)
	else
		-- self._thumb:setPos(0, ((self:height() - self._thumb:height()) / (self._numItems - self._pageSize - 1)) * (value - 1))
		self._thumb:setPos(0, (self:height() - self._thumb:height()) * (value - 1))
	end
end

function ScrollBar:getTopItem()
	return self._topItem
end

function ScrollBar:setBackgroundImage(image)
	self._quads[self._BASE_OBJECTS_INDEX][1]:setTexture(image)

	self._backgroundImage = image
end

function ScrollBar:getBackgroundImage()
	return self._backgroundImage
end

function ScrollBar:setThumbImages(normal, hover)
	self._thumb:setImages(normal, hover, normal, normal)
end

function ScrollBar:getThumbNormalImage()
	return self._thumb:getNormalImage()
end

function ScrollBar:getThumbHoverImage()
	return self._thumb:getHoverImage()
end

function ScrollBar:_ScrollBarEvents()
	self.EVENT_SCROLL_BAR_POS_CHANGED = "EventScrollBarPosChanged"
end

function ScrollBar:init(gui)
	awindow.AWindow.init(self, gui)

	self:_ScrollBarEvents()

	self._type = "ScrollBar"

	self.ORIENTATION_HORZ = 0
	self.ORIENTATION_VERT = 1

	self._numItems = 0
	self._pageSize = 0
	self._topItem = 1
	self._backgroundImage = nil
	self._orientation = self.ORIENTATION_VERT

	self._thumb = thumb.Thumb(gui)
	self:_addWidgetChild(self._thumb)
	self._thumb:setOrientation(self._thumb.ORIENTATION_VERT)

	self._thumb:registerEventHandler(self._thumb.EVENT_THUMB_POS_CHANGED, self, "_handleThumbPosChanged")
end
