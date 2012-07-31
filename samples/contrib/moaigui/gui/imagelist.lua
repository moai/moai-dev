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

	FILE: imagelist.lua
	DESCRIPTION: List of images for a window
	AUTHOR: Derick Dong
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
	CREATED: 4-20-12
]]

local _M = {}

require "gui\\support\\class"

local textures = require "gui\\textures"

local ImageData = class()
_M.ImageList = class()

function ImageData:init()
	self.fileName = ""
	self.texture = nil
	self.color = MOAIColor.new()
	self.color:setColor(1, 1, 1, 1)
	self.colorData = {1, 1, 1, 1}
	self.blendSrc = MOAIProp.BLEND_NORMAL
	self.blendDst = nil
end

function _M.ImageList:setImage(imageType, idx, fileName, r, g, b, a, blendSrc, blendDst)
	local images = self._images[imageType]
	if (nil == images) then
		images = {}
		self._images[imageType] = images
	end

	if (nil == fileName) then
		table.remove(images, idx)
		return
	end

	r = (r or 1)
	g = (g or 1)
	b = (b or 1)
	a = (a or 1)

	local curr = images[idx]
	if (nil == curr) then
		curr = ImageData()
		images[idx] = curr
	end

	curr.fileName = fileName
	curr.texture = textures.get(fileName)
	curr.color:setColor(r, g, b, a)
	curr.colorData = {r, g, b, a}
	curr.blendSrc = blendSrc
	curr.blendDst = blendDst
end

function _M.ImageList:getImage(imageType, idx)
	local images = self._images[imageType]
	if (nil == images) then return nil end

	return images[idx]
end

function _M.ImageList:getImageType(imageType)
	return self._images[imageType]
end

function _M.ImageList:clearImageType(imageType)
	local images = self._images[imageType]
	if (nil == images) then return end

	while (#images > 0) do
		self:setImage(imageType, 1, nil)
	end
end

function _M.ImageList:clearAllImages()
	for k, v in pairs(self._images) do
		self:clearImageType(k)
	end
end

function _M.ImageList:init()
	self._images = {}
end

return _M
