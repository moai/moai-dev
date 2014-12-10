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

	FILE: resources.lua
	DESCRIPTION: Maintains a list of folders that will be searched through when looking for a file
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
]]

local _M = {}

local array = require "gui\\support\\array"
local filesystem = require "gui\\support\\filesystem"

local DEFAULT_GROUP = "default"

local paths = {}

local function Path(p, priority)
	local self = {}
	self.path = p
	self.priority = priority

	return self
end

function _M.addToPath(folder, group, priority)
	local pathGroup
	group = (group or DEFAULT_GROUP)
	priority = (priority or 0)

	local pathGroup = paths[group]
	if (nil == pathGroup) then
		pathGroup = {}
		paths[group] = pathGroup
	end

	local newPath = Path(folder, priority)

	for i, v in ipairs(pathGroup) do
		if (priority < v.priority) then
			table.insert(pathGroup, newPath)
			return
		end
	end

	pathGroup[#pathGroup + 1] = newPath
end

function _M.removeFromPath(folder, group)
	group = (group or DEFAULT_GROUP)

	local pathGroup = paths[group]
	if (nil == pathGroup) then return end

	for i, v in ipairs(pathGroup) do
		if (v.path == folder) then
			table.remove(pathGroup, i)
			return
		end
	end
end

function _M.clearPath(group)
	paths[group] = nil
end

function _M.clearAllPaths()
	paths = {}
end

function _M.getPath(fileName, group)
	group = (group or DEFAULT_GROUP)
	local pathGroup = paths[group]
	if (nil == pathGroup) then return nil end

	for i, v in ipairs(pathGroup) do
		local fullPath = filesystem.pathJoin(v.path, fileName)
		if (true == MOAIFileSystem.checkFileExists(fullPath)) then
			return fullPath
		end
	end

	return nil
end

function _M.getFolderPath(folderName, group)
	group = (group or DEFAULT_GROUP)
	local pathGroup = paths[group]
	if (nil == pathGroup) then return nil end

	for i, v in ipairs(pathGroup) do
		local fullPath = filesystem.pathJoin(v.path, folderName)
		if (true == MOAIFileSystem.checkPathExists(fullPath)) then
			return fullPath
		end
	end

	return nil
end

return _M
