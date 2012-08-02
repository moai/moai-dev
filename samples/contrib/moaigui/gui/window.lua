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

	FILE: window.lua
	DESCRIPTION: A basic window
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

local awindow = require "gui\\awindow"

_M.Window = class(awindow.AWindow)

function _M.Window:setBackgroundImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setCurrImages(self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES)

end

function _M.Window:getBackgroundImage()
	return self._imageList:getImage(self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES)
end

function _M.Window:init(gui)
	awindow.AWindow.init(self, gui)

	self._type = "Window"

	self._BACKGROUND_INDEX = self._WIDGET_SPECIFIC_OBJECTS_INDEX
	self.BACKGROUND_IMAGES = self._WIDGET_SPECIFIC_IMAGES
end

return _M
