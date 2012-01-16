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
]]

module(..., package.seeall)

array = require "gui\\support\\array"
filesystem = require "gui\\support\\filesystem"

local path = {}
local groupPaths = {}

function addToPath(folder, group)
	local p

	if (nil ~= group) then
		if (nil == groupPaths[group]) then
			p = {}
			groupPaths[group] = p
		else
			p = groupPaths[group]
		end
	else
		p = path
	end

	table.insert(p, folder)
end

function removeFromPath(folder, group)
	if (nil ~= group) then
		if (nil ~= groupPaths[group] and nil ~= groupPaths[group][folder]) then
			groupPaths[group][folder] = nil
		end
	else
		if (nil ~= path[folder]) then
			array.removeObject(folder)
		end
	end
end

function clearPath(group)
	if (nil ~= group) then
		if (nil ~= groupPaths[group]) then
			groupPaths[group] = nil
		end
	else
		path = {}
	end
end

function clearAllPaths()
	clearPath()

	for k, v in pairs(groupPaths) do
		clearPath(k)
	end
end

function getPath(fileName, group)
	local p

	if (nil == group) then
		p = path
	else
		p = groupPaths[group]
	end

	if (nil == p) then
		return nil
	end

	for i, v in ipairs(p) do
		local fullPath = filesystem.pathJoin(v, fileName)
		if (true == MOAIFileSystem.checkFileExists(fullPath)) then
			return fullPath
		end
	end

	return nil
end

function getFolderPath(folderName, group)
	local p

	if (nil == group) then
		p = path
	else
		p = groupPaths[group]
	end

	if (nil == p) then
		return nil
	end

	for i, v in ipairs(p) do
		local fullPath = filesystem.pathJoin(v, folderName)
		if (true == MOAIFileSystem.checkPathExists(fullPath)) then
			return fullPath
		end
	end

	return nil
end
