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

	FILE: editbox.lua
	DESCRIPTION: Allows for text entry from the user
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11
]]

module(..., package.seeall)

require "gui\\support\\class"

local awindow = require "gui\\awindow"
local awidgetevent = require "gui\\awidgetevent"

local CURSOR = ":"

local K_BACKSPACE = 8
local K_ENTER = 13

EditBox = class(awindow.AWindow)

function EditBox:_createEditBoxTextAcceptedEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_EDIT_BOX_TEXT_ACCEPTED, self)
	t.text = self._internalText

	return t
end

function EditBox:_addCursor()
	local text = self._text:getString()
	text = text:sub(0, self._cursorPos - 1) .. CURSOR .. text:sub(self._cursorPos)
	self._text:setString(text)
end

function EditBox:_onHandleGainFocus(event)
	self._cursorPos = #self._internalText + 1
	self:_addCursor()

	return self:_baseHandleGainFocus(event)
end

function EditBox:_onHandleLoseFocus(event)
	local text = self._text:getString()
	text = text:sub(0, self._cursorPos - 1) .. text:sub(self._cursorPos + 1)
	self._text:setString(text)

	return self:_baseHandleLoseFocus(event)
end

function EditBox:_onHandleKeyDown(event)
	local key = event.key
	if (K_BACKSPACE == key) then
		local text = self._internalText:sub(0, self._cursorPos - 2) .. self._internalText:sub(self._cursorPos)
		self:setText(text)
		self._cursorPos = self._cursorPos - 1
		self:_addCursor()

		return true
	end

	if (K_ENTER == key) then
		local e = self:_createEditBoxTextAcceptedEvent()
		return self:_handleEvent(self.EVENT_EDIT_BOX_TEXT_ACCEPTED, e)
	end

	if (#self._internalText >= self._maxLength) then
		return false
	end

	-- Can't use string.find, as when certain characters are used (eg. %, $), the function
	-- thinks we're trying to send in a pattern, and crashes the program.
	key = string.char(key)
	local allowed = false
	for i = 1, #self._allowedChars do
		if (key == self._allowedChars:sub(i, i)) then
			allowed = true
			break
		end
	end

	if (false == allowed) then return false end

	local text = self._internalText:sub(0, self._cursorPos) .. key .. self._internalText:sub(self._cursorPos + 1)
	self:setText(text)
	self._cursorPos = self._cursorPos + 1
	self:_addCursor()

	return true
end

function EditBox:_onHandleKeyUp(event)
	
end

function EditBox:setText(text)
	self._internalText = text

	if (nil ~= self._passwordChar) then
		text = string.rep(self._passwordChar, #text)
	end

	self._text:setString(text)

	self:_setTextRect()
	self:_setTextAlignment()
end

function EditBox:getText()
	return self._internalText
end

function EditBox:setBackgroundImage(image)
	self._quads[self._BASE_OBJECTS_INDEX][1]:setTexture(image)

	self._backgroundImage = image
end

function EditBox:getBackgroundImage()
	return self._backgroundImage
end

function EditBox:setPasswordChar(char)
	self._passwordChar = char

	self:setText(self._internalText)
end

function EditBox:getPasswordChar()
	return self._passwordChar
end

function EditBox:setCursorPos(pos)
	if (pos < 1) then
		pos = 1
	end

	if (pos > self._maxLength) then
		pos = self._maxLength
	end

	self._cursorPos = pos
end

function EditBox:getCursorPos()
	return self._cursorPos
end

function EditBox:setMaxLength(length)
	self._maxLength = length

	local text = self._text:getString()
	text = text:sub(1, length)

	self._internalText = text

	self:setText(text)
end

function EditBox:getMaxLength()
	return self._maxLength
end

function EditBox:setAllowedChars(s)
	self._allowedChars = s
end

function EditBox:getAllowedChars()
	return self._allowedChars
end

function EditBox:_EditBoxEvents()
	self.EVENT_EDIT_BOX_TEXT_ACCEPTED = "EventEditBoxTextAccepted"
end

function EditBox:init(gui)
	awindow.AWindow.init(self, gui)

	self._type = "EditBox"

	self:_EditBoxEvents()

	self._passwordChar = nil
	self._cursorPos = 1
	self._internalText = ""
	self._maxLength = 20
	self._allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.:,;(:*!?' "

	self:setTextAlignment(self.TEXT_ALIGN_LEFT, self.TEXT_ALIGN_CENTER)
end
