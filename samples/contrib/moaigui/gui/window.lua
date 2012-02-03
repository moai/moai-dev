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
]]

module(..., package.seeall)

require "gui\\support\\class"

local awindow = require "gui\\awindow"

Window = class(awindow.AWindow)

function Window:setBackground(image, r, g, b, a)
	if (nil == r) then r = 1 end
	if (nil == g) then g = 1 end
	if (nil == b) then b = 1 end
	if (nil == a) then a = 1 end

	self._quads[self._BASE_OBJECTS_INDEX][1]:setTexture(image)
	self._props[self._BASE_OBJECTS_INDEX][1]:setColor(r, g, b, a)
	self._image = image
	self._color = {r, g, b, a}
end

function Window:getBackgroundImage()
	return self._image
end

function Window:getBackgroundColor()
	return self._color
end

function Window:init(gui)
	awindow.AWindow.init(self, gui)

	self._type = "Window"

	self._image = nil
end
