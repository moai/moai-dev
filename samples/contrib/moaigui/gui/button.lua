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

	FILE: button.lua
	DESCRIPTION: Accepts and responds to input 'clicks'
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

local abutton = require "gui\\abutton"
local inputconstants = require "gui\\support\\inputconstants"
local awidgetevent = require "gui\\awidgetevent"

_M.Button = class(abutton.AButton)

function _M.Button:_createButtonClickEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_BUTTON_CLICK, self)

	return t
end

function _M.Button:_inputClick(event)
	if (false == self._visible or false == self._enabled) then
		return
	end

	local e = self:_createButtonClickEvent()

	return self:_handleEvent(self.EVENT_BUTTON_CLICK, e)
end

function _M.Button:_ButtonEvents()
	self.EVENT_BUTTON_CLICK = "EventButtonClick"
end

function _M.Button:init(gui)
	abutton.AButton.init(self, gui)

	self:_ButtonEvents()

	self._type = "Button"
end

return _M
