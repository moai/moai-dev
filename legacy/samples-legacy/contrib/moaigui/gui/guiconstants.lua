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

	FILE: guiconstants.lua
	DESCRIPTION: Various values used by the system
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
]]

local _M = {}

_M.GUI_MOUSE_UP = 0
_M.GUI_MOUSE_DOWN = _M.GUI_MOUSE_UP + 1
_M.GUI_MOUSE_MOVE = _M.GUI_MOUSE_DOWN + 1
_M.GUI_MOUSE_CLICK = _M.GUI_MOUSE_MOVE + 1
_M.GUI_MOUSE_ENTERS = _M.GUI_MOUSE_CLICK + 1
_M.GUI_MOUSE_LEAVES = _M.GUI_MOUSE_ENTERS + 1

_M.GUI_KEY_UP = 20
_M.GUI_KEY_DOWN = _M.GUI_KEY_UP + 1

_M.GUI_TOUCH_UP = 30
_M.GUI_TOUCH_DOWN = _M.GUI_TOUCH_UP + 1
_M.GUI_TOUCH_MOVE = _M.GUI_TOUCH_DOWN + 1
_M.GUI_TOUCH_TAP = _M.GUI_TOUCH_MOVE + 1
_M.GUI_TOUCH_ENTERS = _M.GUI_TOUCH_TAP + 1
_M.GUI_TOUCH_LEAVES = _M.GUI_TOUCH_ENTERS + 1

_M.GUI_BUTTON = 50

_M.GUI_MOTION = 60

_M.GUI_GAIN_FOCUS = 70

_M.GUI_LOSE_FOCUS = 80

return _M
