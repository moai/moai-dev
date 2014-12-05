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

	FILE: textstyles.lua
	DESCRIPTION: Creates and handles fonts
	AUTHOR: Derick Dong
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
	CREATED: 4-20-12
]]

local _M = {}

local resources = require "gui\\support\\resources"

local styles = {}

function _M.get(name)
	return styles[name]
end

function _M.create(name, font, size, r, g, b, a, scale)
	r = (r or 1)
	g = (g or 1)
	b = (b or 1)
	a = (a or 1)
	scale = (scale or 1)


	local style = styles[name]
	if (nil ~= style) then
		return style
	end

	style = MOAITextStyle.new()
	style:setColor(r, g, b, a)
	style:setFont(font)
	style:setSize(size)
	style:setScale(scale)

	styles[name] = style

	return style
end

function _M.release(name)
	styles[name] = nil
end

function _M.releaseAll()
	for k, v in pairs(styles) do
		_M.release(k)
	end
end

return _M
