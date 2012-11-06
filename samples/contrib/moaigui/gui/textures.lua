--[[
* The MIT License
* Copyright (C) 2012 Derick Dong (derickdong@hotmail.com).  All rights reserved.
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

	FILE: textures.lua
	DESCRIPTION: Creates and handles textures
	AUTHOR: Derick Dong
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
	CREATED: 4-20-12
]]

local _M = {}

local textures = {}

DEFAULT_GROUP = "default"

function _M.get(fileName, group)
	if (nil == fileName) then return nil end

	group = (group or DEFAULT_GROUP)

	local groupTextures = textures[group]
	if (nil == groupTextures) then
		groupTextures = {}
		textures[group] = groupTextures
	end

	local tex = groupTextures[fileName]
	if (nil == tex) then
		tex = MOAITexture.new()
		tex:load(fileName)

		groupTextures[fileName] = tex
	end

	return tex
end

function _M.release(fileName, group)
	group = (group or DEFAULT_GROUP)

	local groupTextures = textures[group]
	if (nil == groupTextures) then return end

	local tex = groupTextures[fileName]
	if (nil ~= tex) then
		tex:release()
		groupTextures[fileName] = nil
	end
end

function _M.releaseGroup(group)
	group = (group or DEFAULT_GROUP)

	local groupTextures = textures[group]
	if (nil == groupTextures) then return end

	for k, v in pairs(groupTextures) do
		_M.release(k, group)
	end

	textures[group] = nil
end

function _M.releaseAll()
	for k, v in pairs(textures) do
		_M.releaseGroup(k)
	end

	textures = {}
end

return _M
