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

	FILE: text.lua
	DESCRIPTION: Handles text and its display properties
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11
]]

module(..., package.seeall)

require "gui\\support\\class"

Text = class()

function Text:setString(s)
	self._string = s
	if (true == self._visible) then
		self._textBox:setString(s)
	end

	self:setRect(self:width(), self._align)
	self:setPos(self:x(), self:y())
	self:setColor(self._red, self._green, self._blue, self._alpha)
end

function Text:getString()
	return self._string
end

function Text:setFont(font, size)
	self._font = font
	self._textBox:setFont(font)

	if (nil == size) then
		self._textBox:setTextSize(font:getScale())
	else
		self._textBox:setTextSize(size)
	end

	self:setRect(self:width(), self._align)
	self:setPos(self:x(), self:y())
end

function Text:getFont()
	return self._font
end

function Text:getPos()
	return self._x, self._y
end

function Text:x()
	return self._x
end

function Text:y()
	return self._y
end

function Text:getDim()
	return self._width, self._height
end

function Text:width()
	return self._width
end

function Text:height()
	return self._height
end

function Text:setRect(width, align)
	self._width = width

	if (nil ~= self._font) then
		local scale = self._font:getScale()
		self._height = scale + scale / 6
		self._textBox:setRect(0, -self._height, self._width, 0)
	end
end

function Text:setAlignment(align)
	self._align = align
	self._textBox:setAlignment(align)
end

function Text:setPos(x, y)
	self._x = x
	self._y = y

	self._textBox:setLoc(x, y)
end

function Text:setColor(r, g, b, a)
	self._red = r
	self._green = g
	self._blue = b
	self._alpha = a

	self._textBox:setStringColor(1, #self._string, r, g, b, a)
end

function Text:getColor()
	return self._red, self._green, self._blue, self._alpha
end

function Text:setPriority(priority)
	self._priority = priority
	self._textBox:setPriority(priority)
end

function Text:getPriority()
	return self._priority
end

function Text:show()
-- MOAITextBox does not seem to handle setVisible properly, so we blank out the string when hiding, and
-- restore it when showing.
	-- self._textBox:setVisible(true)
	self._textBox:setString(self._string)
	self._visible = true
end

function Text:hide()
	-- self._textBox:setVisible(false)
	self._textBox:setString("")
	self._visible = false
end

function Text:destroy()
	self._parent._gui:partition():removeProp(self._textBox)
	self._parent._gui:_unregisterHitObject(self._textBox)
end

function Text:init(parent)
	self._parent = parent
	self._layer = layer
	self._string = ""
	self._font = nil
	self._align = nil
	self._visible = true
	self._x = 0
	self._y = 0
	self._width = 0
	self._height = 0
	self._red = 1
	self._green = 1
	self._blue = 1
	self._alpha = 1
	self._priority = 0

	self._textBox = MOAITextBox.new()
	self._textBox:setAlignment(MOAITextBox.LEFT_JUSTIFY)
	self._textBox:setYFlip(true)

	self._parent._gui:partition():insertProp(self._textBox)
	self._parent._gui:_registerHitObject(self._parent, self._textBox)
end
