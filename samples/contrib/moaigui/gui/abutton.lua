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

	FILE: abutton.lua
	DESCRIPTION: Base class for buttons
	AUTHOR: Derick Dong
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
	CREATED: 9-9-11

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
]]

local _M = {}

require "gui\\support\\class"

local awindow = require "gui\\awindow"
local imagelist = require "gui\\imagelist"
local inputconstants = require "gui\\support\\inputconstants"

_M.AButton = class(awindow.AWindow)

function _M.AButton:_inputClick(event)
	return false
end

function _M.AButton:_inputEnters(event)
	self._over = true
	self:_setCurrImages(self._BUTTON_INDEX, self.HOVER_IMAGES)

	return false
end

function _M.AButton:_inputLeaves(event)
	self._over = false
	self:_setCurrImages(self._BUTTON_INDEX, self.NORMAL_IMAGES)

	return false
end

function _M.AButton:_inputUp(event)
	self._pushed = false
	if (false == self._over) then
		self:_setCurrImages(self._BUTTON_INDEX, self.NORMAL_IMAGES)
	else
		self:_setCurrImages(self._BUTTON_INDEX, self.HOVER_IMAGES)
	end

	return false
end

function _M.AButton:_inputDown(event)
	self._pushed = true
	self:_setCurrImages(self._BUTTON_INDEX, self.PUSHED_IMAGES)

	return false
end

function _M.AButton:_onHandleMouseEnters(event)
	if (self:_inputEnters()) then
		return true
	end

	return self:_baseHandleMouseEnters(event)
end

function _M.AButton:_onHandleMouseLeaves(event)
	if (self:_inputLeaves()) then
		return true
	end

	return self:_baseHandleMouseLeaves(event)
end

function _M.AButton:_onHandleMouseUp(event)
	if (event.button == inputconstants.LEFT_MOUSE_BUTTON) then
		if (self:_inputUp()) then
			return true
		end
	end

	return self:_baseHandleMouseUp(event)
end

function _M.AButton:_onHandleMouseDown(event)
	if (event.button == inputconstants.LEFT_MOUSE_BUTTON) then
		if (self:_inputDown()) then
			return true
		end
	end

	return self:_baseHandleMouseDown(event)
end

function _M.AButton:_onHandleMouseClick(event)
	if (event.button == inputconstants.LEFT_MOUSE_BUTTON) then
		if (self:_inputClick(event)) then
			return true
		end
	end

	return self:_baseHandleMouseClick(event)
end

function _M.AButton:_onHandleTouchEnters(event)
	if (self:_inputEnters()) then
		return true
	end

	return self:_baseHandleTouchEnters(event)
end

function _M.AButton:_onHandleTouchLeaves(event)
	if (self:_inputLeaves()) then
		return true
	end

	return self:_baseHandleTouchLeaves(event)
end

function _M.AButton:_onHandleTouchUp(event)
	if (self:_inputUp()) then
		return true
	end
end

function _M.AButton:_onHandleTouchDown(event)
	if (self:_inputDown()) then
		return true
	end
end

function _M.AButton:_onHandleTouchTap(event)
	if (self:_inputClick(event)) then
		return true
	end
end

function _M.AButton:setNormalImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BUTTON_INDEX, self.NORMAL_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)

	if (false == self._over and false == self._pushed) then
		self:_setCurrImages(self._BUTTON_INDEX, self.NORMAL_IMAGES)
	end
end

function _M.AButton:setHoverImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BUTTON_INDEX, self.HOVER_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)

	if (true == self._over and false == self._pushed) then
		self:_setCurrImages(self._BUTTON_INDEX, self.HOVER_IMAGES)
	end
end

function _M.AButton:setPushedImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BUTTON_INDEX, self.PUSHED_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)

	if (false == self._over and true == self._pushed) then
		self:_setCurrImages(self._BUTTON_INDEX, self.PUSHED_IMAGES)
	end
end

function _M.AButton:setDisabledImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BUTTON_INDEX, self.DISABLED_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)

	if (true == self._disabled) then
		self:_setCurrImages(self._BUTTON_INDEX, self.DISABLED_IMAGES)
	end
end

function _M.AButton:setImages(normal, hover, pushed, disabled, idx)
	self:setNormalImage(normal, nil, nil, nil, nil, idx)
	self:setHoverImage(hover, nil, nil, nil, nil, idx)
	self:setPushedImage(pushed, nil, nil, nil, nil, idx)
	self:setDisabledImage(disabled, nil, nil, nil, nil, idx)
end

function _M.AButton:setAllImages(fileName, idx)
	self:setNormalImage(fileName, nil, nil, nil, nil, idx)
	self:setHoverImage(fileName, nil, nil, nil, nil, idx)
	self:setPushedImage(fileName, nil, nil, nil, nil, idx)
	self:setDisabledImage(fileName, nil, nil, nil, nil, idx)
end

function _M.AButton:getImage(imageType, idx)
	imageType = (imageType or self.NORMAL_IMAGES)
	idx = (idx or 1)

	return self._imageList:getImage(imageType, idx)
end

function _M.AButton:clearImages(imageType)
	if (nil == imageType) then
		self._imageList:clearAllImages()
		self:_setCurrImages(self._BUTTON_INDEX, self.NORMAL_IMAGES)

	else
		self._imageList:clearImageType(imageType)
	end
end

function _M.AButton:init(gui)
	awindow.AWindow.init(self, gui)

	self._BUTTON_INDEX = self._WIDGET_SPECIFIC_OBJECTS_INDEX
	self.NORMAL_IMAGES = self._WIDGET_SPECIFIC_IMAGES
	self.HOVER_IMAGES = self.NORMAL_IMAGES + 1
	self.PUSHED_IMAGES = self.HOVER_IMAGES + 1
	self.DISABLED_IMAGES = self.PUSHED_IMAGES + 1

	self._type = "AButton"
	self._over = false
	self._pushed = false
end

return _M
