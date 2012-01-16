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

	FILE: image.lua
	DESCRIPTION: Displays a static image
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11
]]

module(..., package.seeall)

require "gui\\support\\class"

local awindow = require "gui\\awindow"

ImageData = class()
Image = class(awindow.AWindow)

function ImageData:init()
	self.fileName = nil
	self.color = {1, 1, 1, 1}
	self.blendSrc = MOAIProp2D.BLEND_NORMAL
	self.blendDst = nil
end

function Image:_addImages(idx)
	if (nil == idx) then return end

	for i = #self._quads[self._BASE_OBJECTS_INDEX], idx - 1 do
		local quad, prop = self._gui:_createRenderObject(self._priority + i)
		self._gui:_registerHitObject(self, prop)

		self._quads[self._BASE_OBJECTS_INDEX][#self._quads[self._BASE_OBJECTS_INDEX] + 1] = quad
		self._props[self._BASE_OBJECTS_INDEX][#self._props[self._BASE_OBJECTS_INDEX] + 1] = prop
	end

	self:setPos(self._x, self._y)
	self:setDim(self._width, self._height)
end

function Image:_setImages()
	for i = 1, #self._images do
		self._quads[self._BASE_OBJECTS_INDEX][i]:setTexture(self._images[i].fileName)
		self._props[self._BASE_OBJECTS_INDEX][i]:setColor(self._images[i].color[1], self._images[i].color[2], self._images[i].color[3], self._images[i].color[4])
		self._props[self._BASE_OBJECTS_INDEX][i]:setBlendMode(self._images[i].blendSrc, self._images[i].blendDst)
	end

	-- for i = #self._images + 1, #self._quads[self._BASE_OBJECTS_INDEX] do
		-- self._quads[self._BASE_OBJECTS_INDEX][i]:setTexture(nil)
	-- end
end

function Image:setImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	if (nil == idx) then idx = 1 end

	self:_addImages(idx)

	if (nil == fileName) then
		self._images[idx] = nil

		self._quads[self._BASE_OBJECTS_INDEX][idx]:setTexture(nil)
		self._props[self._BASE_OBJECTS_INDEX][idx]:setColor(1, 1, 1, 1)
		self._props[self._BASE_OBJECTS_INDEX][idx]:setBlendMode(MOAIProp2D.NORMAL)

	else
		if (nil == r) then r = 1 end
		if (nil == g) then g = 1 end
		if (nil == b) then b = 1 end
		if (nil == a) then a = 1 end

		local curr = self._images[idx]
		if (nil == curr) then
			curr = ImageData()
			self._images[idx] = curr
		end

		curr.fileName = fileName
		curr.color = {r, g, b, a}
		curr.blendSrc = blendSrc
		curr.blendDst = blendDst

		self:_setImages()
	end
end

function Image:getImage(idx)
	if (nil == idx) then idx = 1 end
	return self._images[idx]
end

function Image:clearImages()
	for i, v in ipairs(self._images) do
		self:setImage(nil, 1, 1, 1, 1, i)
	end
end

function Image:init(gui)
	awindow.AWindow.init(self, gui)

	self._type = "Image"

	self._images = {}
end
