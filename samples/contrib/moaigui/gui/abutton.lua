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


	FILE: abutton.lua
	DESCRIPTION: Base class for buttons
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11
]]


module(..., package.seeall)

require "gui\\support\\class"

local awindow = require "gui\\awindow"
local inputconstants = require "gui\\support\\inputconstants"

ImageData = class()
AButton = class(awindow.AWindow)

function ImageData:init()
	self.fileName = nil
	self.color = {1, 1, 1, 1}
	self.blendSrc = MOAIProp2D.BLEND_NORMAL
	self.blendDst = nil
end

function AButton:_addImages(idx)
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

function AButton:_setImage(imageType, idx, fileName, r, g, b, a, blendSrc, blendDst)
	local images = self._images[imageType]
	if (nil == images) then return end

	if (nil == fileName) then
		images[idx] = nil
		for i = idx, #images do
			images[i] = images[i + 1]
		end

	else
		if (nil == r) then r = 1 end
		if (nil == g) then g = 1 end
		if (nil == b) then b = 1 end
		if (nil == a) then a = 1 end

		local curr = images[idx]
		if (nil == curr) then
			curr = ImageData()
			images[idx] = curr
		end

		curr.fileName = fileName
		curr.color = {r, g, b, a}
		curr.blendSrc = blendSrc
		curr.blendDst = blendDst
	end
end

function AButton:_setCurrImages(imageType)
	local images = self._images[imageType]
	if (nil == images) then return end

	for i = 1, #images do
		self._quads[self._BASE_OBJECTS_INDEX][i]:setTexture(images[i].fileName)
		self._props[self._BASE_OBJECTS_INDEX][i]:setColor(images[i].color[1], images[i].color[2], images[i].color[3], images[i].color[4])
		self._props[self._BASE_OBJECTS_INDEX][i]:setBlendMode(images[i].blendSrc, images[i].blendDst)
	end

	for i = #images + 1, #self._quads[self._BASE_OBJECTS_INDEX] do
		self._quads[self._BASE_OBJECTS_INDEX][i]:setTexture(nil)
	end
end

function AButton:_inputClick(event)
	return false
end

function AButton:_inputEnters(event)
	self._over = true
	self:_setCurrImages(self.HOVER_IMAGES)

	return false
end

function AButton:_inputLeaves(event)
	self._over = false
	self:_setCurrImages(self.NORMAL_IMAGES)

	return false
end

function AButton:_inputUp(event)
	self._pushed = false
	if (false == self._over) then
		self:_setCurrImages(self.NORMAL_IMAGES)
	else
		self:_setCurrImages(self.HOVER_IMAGES)
	end

	return false
end

function AButton:_inputDown(event)
	self._pushed = true
	self:_setCurrImages(self.PUSHED_IMAGES)

	return false
end

function AButton:_onHandleMouseEnters(event)
	if (self:_inputEnters()) then
		return true
	end

	return self:_baseHandleMouseEnters(event)
end

function AButton:_onHandleMouseLeaves(event)
	if (self:_inputLeaves()) then
		return true
	end

	return self:_baseHandleMouseLeaves(event)
end

function AButton:_onHandleMouseUp(event)
	if (event.button == inputconstants.LEFT_MOUSE_BUTTON) then
		if (self:_inputUp()) then
			return true
		end
	end

	return self:_baseHandleMouseUp(event)
end

function AButton:_onHandleMouseDown(event)
	if (event.button == inputconstants.LEFT_MOUSE_BUTTON) then
		if (self:_inputDown()) then
			return true
		end
	end

	return self:_baseHandleMouseDown(event)
end

function AButton:_onHandleMouseClick(event)
	if (event.button == inputconstants.LEFT_MOUSE_BUTTON) then
		if (self:_inputClick(event)) then
			return true
		end
	end

	return self:_baseHandleMouseClick(event)
end

function AButton:_onHandleTouchEnters(event)
	if (self:_inputEnters()) then
		return true
	end

	return self:_baseHandleTouchEnters(event)
end

function AButton:_onHandleTouchLeaves(event)
	if (self:_inputLeaves()) then
		return true
	end

	return self:_baseHandleTouchLeaves(event)
end

function AButton:_onHandleTouchUp(event)
	if (self:_inputUp()) then
		return true
	end
end

function AButton:_onHandleTouchDown(event)
	if (self:_inputDown()) then
		return true
	end
end

function AButton:_onHandleTouchTap(event)
	if (self:_inputClick(event)) then
		return true
	end
end

function AButton:setNormalImage(image, r, g, b, a, idx, blendSrc, blendDst)
	if (nil == idx) then idx = 1 end

	self:_addImages(idx)
	self:_setImage(self.NORMAL_IMAGES, idx, image, r, g, b, a, blendSrc, blendDst)

	if (false == self._over and false == self._pushed) then
		self:_setCurrImages(self.NORMAL_IMAGES)
	end
end

function AButton:setHoverImage(image, r, g, b, a, idx, blendSrc, blendDst)
	if (nil == idx) then idx = 1 end

	self:_addImages(idx)
	self:_setImage(self.HOVER_IMAGES, idx, image, r, g, b, a, blendSrc, blendDst)

	if (true == self._over and false == self._pushed) then
		self:_setCurrImages(self.HOVER_IMAGES)
	end
end

function AButton:setPushedImage(image, r, g, b, a, idx, blendSrc, blendDst)
	if (nil == idx) then idx = 1 end

	self:_addImages(idx)
	self:_setImage(self.PUSHED_IMAGES, idx, image, r, g, b, a, blendSrc, blendDst)

	if (false == self._over and true == self._pushed) then
		self:_setCurrImages(self.PUSHED_IMAGES)
	end
end

function AButton:setDisabledImage(image, r, g, b, a, idx, blendSrc, blendDst)
	if (nil == idx) then idx = 1 end

	self:_addImages(idx)
	self:_setImage(self.DISABLED_IMAGES, idx, image, r, g, b, a, blendSrc, blendDst)

	if (true == self._disabled) then
		self:_setCurrImages(self.DISABLED_IMAGES)
	end
end

function AButton:setImages(normal, hover, pushed, disabled)
	self:setNormalImage(normal)
	self:setHoverImage(hover)
	self:setPushedImage(pushed)
	self:setDisabledImage(disabled)
end

function AButton:setAllImages(image)
	self:setNormalImage(image)
	self:setHoverImage(image)
	self:setPushedImage(image)
	self:setDisabledImage(image)
end

function AButton:clearImages(imageType)
	if (nil == imageType) then
		for k, v in pairs(self._images) do
			for i = #v, 1, -1 do
				self:_setImage(k, i, nil)
			end
		end

		self:_setCurrImages(self.NORMAL_IMAGES)

	else
		local images = self._images[imageType]
		if (nil == images) then return end

		for i, v in ipairs(images) do
			self:_setImage(imageType, i, nil)
		end
	end
end

function AButton:init(gui)
	awindow.AWindow.init(self, gui)

	self.NORMAL_IMAGES = 1
	self.HOVER_IMAGES = 2
	self.PUSHED_IMAGES = 3
	self.DISABLED_IMAGES = 4

	self._type = "AButton"
	self._over = false
	self._pushed = false

	self._images = {}
	self._images[self.NORMAL_IMAGES] = {}
	self._images[self.HOVER_IMAGES] = {}
	self._images[self.PUSHED_IMAGES] = {}
	self._images[self.DISABLED_IMAGES] = {}

	self._onClick = nil
	self._onClickData = nil
end
