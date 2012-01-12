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

	FILE: group.lua
	DESCRIPTION: Allows grouping of widgets
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11
]]

module(..., package.seeall)

require "gui\\support\\class"

local array = require "gui\\support\\array"

Group = class()

function Group:clear()
	self:removeAllObjects()
end

function Group:getObjects()
	return self._guiObjects
end

function Group:addObject(obj)
	table.insert(self._guiObjects, obj)
end

function Group:removeObject(obj)
	array.removeObject(self._guiObjects, obj)
end

function Group:removeAllObjects()
	self._guiObjects = {}
end

function Group:show(flag)
	for i, v in ipairs(self._guiObjects) do
		v:show(flag)
	end
	
	self._show = flag
end

function Group:enable(flag)
	for i, v in ipairs(self._guiObjects) do
		v:setEnabled(flag)
	end

	self._enabled = flag
end

function Group:init()
	self._type = "group"
	self._show = true
	self._enabled = false

	self._guiObjects = {}
end
