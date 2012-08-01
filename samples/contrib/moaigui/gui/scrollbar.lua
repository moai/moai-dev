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

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3

	BUGS
	- setTopItem - setting the thumb position
]]

local _M = {}

require "gui\\support\\class"

local awindow = require "gui\\awindow"
local thumb = require "gui\\thumb"
local awidgetevent = require "gui\\awidgetevent"

_M.ScrollBar = class(awindow.AWindow)

function _M.ScrollBar:_createScrollBarPosChangedEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_SCROLL_BAR_POS_CHANGED, self)

	return t
end

function _M.ScrollBar:_setTextRect()
	self._text:setRect(0)
end

function _M.ScrollBar:_onSetDim()
	if (nil ~= self._thumb) then
		if (self.ORIENTATION_HORZ == self._orientation) then
			self._thumb:setDim(5, self:height())
			self._thumb:setRange(0, self:width())
		else
			self._thumb:setDim(self:width(), 5)
			self._thumb:setRange(0, self:height())
		end
	end
end

function _M.ScrollBar:_handleThumbPosChanged(event)
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

function _M.ScrollBar:setNumItems(value)
	self._numItems = value
end

function _M.ScrollBar:getNumItems()
	return self._numItems
end

function _M.ScrollBar:setPageSize(value)
	self._pageSize = value
end

function _M.ScrollBar:getPageSize()
	return self._pageSize
end

function _M.ScrollBar:setOrientation(ori)
	if (self.ORIENTATION_HORZ == ori) then
		self._orientation = self.ORIENTATION_HORZ
		self._thumb:setOrientation(self._thumb.ORIENTATION_HORZ)
	else
		self._orientation = self.ORIENTATION_VERT
		self._thumb:setOrientation(self._thumb.ORIENTATION_VERT)
	end
end

function _M.ScrollBar:getOrientation()
	return self._orientation
end

function _M.ScrollBar:setTopItem(value)
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

function _M.ScrollBar:getTopItem()
	return self._topItem
end

function _M.ScrollBar:setBackgroundImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setCurrImages(self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES)
end

function _M.ScrollBar:getBackgroundImage()
	return self._imageList:getImage(self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES)
end

function _M.ScrollBar:setThumbImages(normal, hover)
	self._thumb:setImages(normal, hover, normal, normal)
end

function _M.ScrollBar:getThumbNormalImage()
	return self._thumb:getNormalImage()
end

function _M.ScrollBar:getThumbHoverImage()
	return self._thumb:getHoverImage()
end

function _M.ScrollBar:_ScrollBarEvents()
	self.EVENT_SCROLL_BAR_POS_CHANGED = "EventScrollBarPosChanged"
end

function _M.ScrollBar:init(gui)
	awindow.AWindow.init(self, gui)

	self:_ScrollBarEvents()

	self._type = "ScrollBar"

	self.ORIENTATION_HORZ = 0
	self.ORIENTATION_VERT = 1

	self._BACKGROUND_INDEX = self._WIDGET_SPECIFIC_OBJECTS_INDEX
	self.BACKGROUND_IMAGES = self._WIDGET_SPECIFIC_IMAGES

	self._numItems = 0
	self._pageSize = 0
	self._topItem = 1
	self._orientation = self.ORIENTATION_VERT

	self._thumb = thumb.Thumb(gui)
	self:_addWidgetChild(self._thumb)
	self._thumb:setOrientation(self._thumb.ORIENTATION_VERT)

	self._thumb:registerEventHandler(self._thumb.EVENT_THUMB_POS_CHANGED, self, "_handleThumbPosChanged")
end

return _M
