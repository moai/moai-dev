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

	BUGS
	- Using a uv transform seems to affect all displayed text
]]

module(..., package.seeall)

require "gui\\support\\class"

local awindow = require "gui\\awindow"
local constants = require "gui\\guiconstants"
local awidgetevent = require "gui\\awidgetevent"

ProgressBar = class(awindow.AWindow)

function ProgressBar:_createProgressBarChangedEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_PROGRESS_BAR_CHANGED, self)

	return t
end

function ProgressBar:_createProgressBarDoneEvent()
	local t = awidgetevent.AWidgetEvent(self.EVENT_PROGRESS_BAR_DONE, self)

	return t
end

function ProgressBar:_postChangedEvent(prevValue, currValue)
	event = self._createProgressBarChangedEvent(self.EVENT_PROGRESS_BAR_CHANGED)
	event.prevValue = prevValue
	event.value = currValue

	return self:_handleEvent(self.EVENT_PROGRESS_BAR_CHANGED, event)
end

function ProgressBar:_postDoneEvent()
	event = self._createProgressBarDoneEvent(self.EVENT_PROGRESS_BAR_DONE)
	event.prevValue = prevValue
	event.value = currValue

	return self:_handleEvent(self.EVENT_PROGRESS_BAR_DONE, event)
end

function ProgressBar:setProgress(value)
	prevValue = self._progress

	if (100 < value) then
		value = 100
	end

	self._progress = value

	self:_postChangedEvent(prevValue, self._progress)

	if (100 < self._progress) then
		self:_postDoneEvent()
	else
		self._progressMaskUVTransform:setLoc((-value) * 0.01, 0)
	end
end

function ProgressBar:getProgress()
	return self._progress
end

function ProgressBar:adjustProgress(delta)
	self:setProgress(self:getProgress() + delta)
end

function ProgressBar:setStepSize(size)

end

function ProgressBar:getStepSize()
	return self._stepSize
end

function ProgressBar:setBackgroundImage(image)
	self._quads[self._BASE_OBJECTS_INDEX][1]:setTexture(image)
	self._backgroundImage = image
end

function ProgressBar:getBackgroundImage()
	return self._backgroundImage
end

function ProgressBar:setProgressBarImage(image)
	self._progressBarImage = image
	self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX][self._PROGRESS_BAR_INDEX]:setTexture(image)
end

function ProgressBar:getProgressBarImage()
	return self._progressBarImage
end

function ProgressBar:setProgressMaskImage(image)
	self._progressMaskImage = image
	tex = self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX][self._PROGRESS_MASK_INDEX]:setTexture(image)
	tex:setWrap(false)
end

function ProgressBar:getProgressMaskImage()
	return self._progressMaskImage
end

function ProgressBar:setImages(normal, bar, mask)
	self:setBackgroundImage(normal)
	self:setProgressBarImage(bar)
	self:setProgressMaskImage(mask)
end

function ProgressBar:_ProgressBarEvents()
	self.EVENT_PROGRESS_BAR_CHANGED = "EventProgressBarChanged"
	self.EVENT_PROGRESS_BAR_DONE = "EventProgressBarDone"
end

function ProgressBar:init(gui)
	awindow.AWindow.init(self, gui)

	ProgressBar:_ProgressBarEvents()

	self._type = "ProgressBar"
	self._progress = 0
	self._stepSize = 0
	self._backgroundImage = nil
	self._progressBarImage = nil
	self._progressBarMaskImage = nil

	self._PROGRESS_BAR_INDEX = 1
	self._PROGRESS_MASK_INDEX = 2

	progressQuad, progressProp = self._gui:_createRenderObject(self._priority + 1)
	self._gui:_registerHitObject(self, progressProp)
	table.insert(self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX], progressQuad)
	table.insert(self._props[self._WIDGET_SPECIFIC_OBJECTS_INDEX], progressProp)

	progressMaskQuad, progressMaskProp = self._gui:_createRenderObject(self._priority + 2)
	self._gui:_registerHitObject(self, progressMaskProp)
	table.insert(self._quads[self._WIDGET_SPECIFIC_OBJECTS_INDEX], progressMaskQuad)
	table.insert(self._props[self._WIDGET_SPECIFIC_OBJECTS_INDEX], progressMaskProp)

	self._progressMaskUVTransform = MOAITransform.new ()
	progressMaskProp:setUVTransform(self._progressMaskUVTransform)
end
