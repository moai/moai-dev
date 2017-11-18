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

	FILE: progressbar.lua
	DESCRIPTION: Displays a bar indicating progress
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3

	BUGS
	- Using a uv transform seems to affect all displayed text
]]

local _M = {}

require "gui\\support\\class"

local awindow = require "gui\\awindow"
local constants = require "gui\\guiconstants"
local awidgetevent = require "gui\\awidgetevent"

_M.ProgressBar = class(awindow.AWindow)

function _M.ProgressBar:_createProgressBarChangedEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_PROGRESS_BAR_CHANGED, self)

	return t
end

function _M.ProgressBar:_createProgressBarDoneEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_PROGRESS_BAR_DONE, self)

	return t
end

function _M.ProgressBar:_postChangedEvent(prevValue, currValue)
	event = self._createProgressBarChangedEvent(self.EVENT_PROGRESS_BAR_CHANGED)
	event.prevValue = prevValue
	event.value = currValue

	return self:_handleEvent(self.EVENT_PROGRESS_BAR_CHANGED, event)
end

function _M.ProgressBar:_postDoneEvent()
	event = self._createProgressBarDoneEvent(self.EVENT_PROGRESS_BAR_DONE)
	event.prevValue = prevValue
	event.value = currValue

	return self:_handleEvent(self.EVENT_PROGRESS_BAR_DONE, event)
end

function _M.ProgressBar:_updateUVTransforms()
	for i, v in ipairs(self._progressMaskUVTransforms) do
		v:setLoc((-self._progress) * 0.01, 0)
	end
end

function _M.ProgressBar:setProgress(value)
	prevValue = self._progress

	if (100 < value) then
		value = 100
	end

	self._progress = value

	self:_postChangedEvent(prevValue, self._progress)

	if (100 < self._progress) then
		self:_postDoneEvent()
	else
		self:_updateUVTransforms()
	end
end

function _M.ProgressBar:getProgress()
	return self._progress
end

function _M.ProgressBar:adjustProgress(delta)
	self:setProgress(self:getProgress() + delta)
end

function _M.ProgressBar:setStepSize(size)

end

function _M.ProgressBar:getStepSize()
	return self._stepSize
end

function _M.ProgressBar:setBackgroundImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setCurrImages(self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES)
end

function _M.ProgressBar:getBackgroundImage()
	return self._imageList:get(self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES)
end

function _M.ProgressBar:setProgressBarImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BAR_INDEX, self.BAR_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setCurrImages(self._BAR_INDEX, self.BAR_IMAGES)
end

function _M.ProgressBar:getProgressBarImage()
	return self._imageList:get(self._BAR_INDEX, self.BAR_IMAGES)
end

function _M.ProgressBar:setProgressMaskImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._MASK_INDEX, self.MASK_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setCurrImages(self._MASK_INDEX, self.MASK_IMAGES)

	for i, v in ipairs(self:_getPropObjList(self._MASK_INDEX)) do
		local transform = self._progressMaskUVTransforms[i]
		if (nil == transform) then
			transform = MOAITransform.new()
			v:setUVTransform(transform)
			self._progressMaskUVTransforms[i] = transform
		end
	end

	self:_updateUVTransforms()
end

function _M.ProgressBar:getProgressMaskImage()
	return self._imageList:get(self._MASK_INDEX, self.MASK_IMAGES)
end

function _M.ProgressBar:setImages(normal, bar, mask)
	self:setBackgroundImage(normal)
	self:setProgressBarImage(bar)
	self:setProgressMaskImage(mask)
end

function _M.ProgressBar:_ProgressBarEvents()
	self.EVENT_PROGRESS_BAR_CHANGED = "EventProgressBarChanged"
	self.EVENT_PROGRESS_BAR_DONE = "EventProgressBarDone"
end

function _M.ProgressBar:init(gui)
	awindow.AWindow.init(self, gui)

	self:_ProgressBarEvents()

	self._type = "ProgressBar"
	self._progress = 0
	self._stepSize = 0

	self._BACKGROUND_INDEX = self._WIDGET_SPECIFIC_OBJECTS_INDEX
	self._BAR_INDEX = self._BACKGROUND_INDEX + 1
	self._MASK_INDEX = self._BAR_INDEX + 1

	self.BACKGROUND_IMAGES = self._WIDGET_SPECIFIC_IMAGES
	self.BAR_IMAGES = self.BACKGROUND_IMAGES + 1
	self.MASK_IMAGES = self.BAR_IMAGES + 1

	self._progressMaskUVTransforms = {}
end

return _M
