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

	FILE: factory.lua
	DESCRIPTION: Creates widgets, using the specified theme
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3
]]

local _M = {}

require "gui\\support\\class"

local window = require "gui\\window"
local label = require "gui\\label"
local image = require "gui\\image"
local button = require "gui\\button"
local checkbox = require "gui\\checkbox"
local radiobutton = require "gui\\radiobutton"
local editbox = require "gui\\editbox"
local scrollbar = require "gui\\scrollbar"
local slider = require "gui\\slider"
local progressbar = require "gui\\progressbar"
local textbox = require "gui\\textbox"
local widgetlist = require "gui\\widgetlist"
local textstyles = require "gui\\textstyles"

_M.Factory = class()

function _M.Factory:_createBase(w)
	w:setTextStyle(self._textStyle)
end

function _M.Factory:_createSlider(orientation)
	local w = slider.Slider(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	local data
	if ("vert" == orientation) then
		data = self._theme.vertSliderTheme
	else
		data = self._theme.horzSliderTheme
	end

	w:setBackgroundImage(data.normal)
	w:setThumbImages(data.slidernormal, data.sliderhover)

	return w
end

function _M.Factory:_createScrollBar(orientation)
	local w = scrollbar.ScrollBar(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	if ("vert" == orientation) then
		data = self._theme.vertScrollBarTheme
	else
		data = self._theme.horzScrollBarTheme
	end

	w:setBackgroundImage(data.normal)
	w:setThumbImages(data.scrollernormal, data.scrollerhover)

	return w
end

function _M.Factory:_createWindow()
	local w = window.Window(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	return w
end

function _M.Factory:_createLabel()
	local w = label.Label(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	return w
end

function _M.Factory:_createImage()
	local w = image.Image(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	local data = self._theme.labelTheme

	return w
end

function _M.Factory:_createButton()
	local w = button.Button(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	local data = self._theme.buttonTheme
	w:setImages(data.normal, data.hover, data.pushed, data.disabled)

	return w
end

function _M.Factory:_createCheckBox()
	local w = checkbox.CheckBox(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	local data = self._theme.checkBoxTheme
	w:setImages(data.normal, data.hover, data.pushed, data.disabled, data.selected)

	return w
end

function _M.Factory:_createRadioButton()
	local w = radiobutton.RadioButton(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	local data = self._theme.radioButtonTheme
	w:setImages(data.normal, data.hover, data.pushed, data.disabled, data.selected)

	return w
end

function _M.Factory:_createEditBox()
	local w = editbox.EditBox(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	local data = self._theme.editBoxTheme
	w:setBackgroundImage(data.normal)

	return w
end

function _M.Factory:_createVertSlider()
	local w = self:_createSlider("vert")
	w:setOrientation(w.ORIENTATION_VERT)

	return w
end

function _M.Factory:_createHorzSlider()
	local w = self:_createSlider("horz")
	w:setOrientation(w.ORIENTATION_HORZ)

	return w
end

function _M.Factory:_createVertScrollBar()
	local w = self:_createScrollBar("vert")
	w:setOrientation(w.ORIENTATION_VERT)

	return w
end

function _M.Factory:_createHorzScrollBar()
	local w = self:_createScrollBar("horz")
	w:setOrientation(w.ORIENTATION_HORZ)

	return w
end

function _M.Factory:_createProgressBar()
	local w = progressbar.ProgressBar(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	local data = self._theme.progressBarTheme
	w:setImages(data.normal, data.bar, data.mask)

	return w
end

function _M.Factory:_createTextBox()
	local w = textbox.TextBox(self._gui)
	self:_createBase(w)

	if (nil == self._theme) then
		return w
	end

	local data = self._theme.textBoxTheme

	return w
end

function _M.Factory:_createWidgetList()
	local w = widgetlist.WidgetList(self._gui)
	if (nil == self._theme) then
		return w
	end

	local data = self._theme.widgetListTheme
	w:setSelectedTextStyle(textstyles.get(data.selected))
	w:setUnselectedTextStyle(textstyles.get(data.unselected))

	return w
end

function _M.Factory:_registerWidgets()
	self._registeredWidgets["window"] = self._createWindow
	self._registeredWidgets["label"] = self._createLabel
	self._registeredWidgets["image"] = self._createImage
	self._registeredWidgets["button"] = self._createButton
	self._registeredWidgets["checkbox"] = self._createCheckBox
	self._registeredWidgets["radiobutton"] = self._createRadioButton
	self._registeredWidgets["editbox"] = self._createEditBox
	self._registeredWidgets["vertslider"] = self._createVertSlider
	self._registeredWidgets["horzslider"] = self._createHorzSlider
	self._registeredWidgets["vertscrollbar"] = self._createVertScrollBar
	self._registeredWidgets["horzscrollbar"] = self._createHorzScrollBar
	self._registeredWidgets["progressbar"] = self._createProgressBar
	self._registeredWidgets["textbox"] = self._createTextBox
	self._registeredWidgets["widgetlist"] = self._createWidgetList
end

function _M.Factory:create(widgetType, ...)
	if (nil == widgetType) then return nil end

	widgetType = widgetType:lower()
	widgetType = widgetType:gsub(" ", "")

	local func = self._registeredWidgets[widgetType]
	if (nil == func) then return nil end

	local widget = func(self)
	return widget
end

function _M.Factory:setTheme(theme)
	self._theme = theme
end

function _M.Factory:setCurrTextStyle(style)
	self._textStyle = style
end

function _M.Factory:init(gui)
	self._gui = gui
	self._theme = nil
	self._textStyle = nil
	self._registeredWidgets = {}

	self:_registerWidgets()
end

return _M
