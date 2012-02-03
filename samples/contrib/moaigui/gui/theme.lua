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

	FILE: theme.lua
	DESCRIPTION: Information about how widgets should be presented (fonts, colors, images, etc)
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11
]]

module(..., package.seeall)

require "gui\\support\\class"

local utilities = require "gui\\support\\utilities"
local resources = require "gui\\support\\resources"
local fonts = require "gui\\fonts"

AWidgetTheme = class()
LabelTheme = class(AWidgetTheme)
ButtonTheme = class(AWidgetTheme)
CheckBoxTheme = class(AWidgetTheme)
RadioButtonTheme = class(AWidgetTheme)
EditBoxTheme = class(AWidgetTheme)
ScrollBarTheme = class(AWidgetTheme)
SliderTheme = class(AWidgetTheme)
ProgressBarTheme = class(AWidgetTheme)
TextBoxTheme = class(AWidgetTheme)
WidgetListTheme = class(AWidgetTheme)
Theme = class()

function AWidgetTheme:init(data)

end

function LabelTheme:init(data)
	AWidgetTheme.init(self, data)

end

function ButtonTheme:init(data)
	AWidgetTheme.init(self, data)

	self.normal = resources.getPath(data.normal)
	self.pushed = resources.getPath(data.pushed)
	self.hover = resources.getPath(data.hover)
end

function CheckBoxTheme:init(data)
	AWidgetTheme.init(self, data)

	self.normal = resources.getPath(data.normal)
	self.hover = resources.getPath(data.hover)
	self.pushed = resources.getPath(data.pushed)
	self.selected = resources.getPath(data.selected)
end

function RadioButtonTheme:init(data)
	AWidgetTheme.init(self, data)

	self.normal = resources.getPath(data.normal)
	self.hover = resources.getPath(data.hover)
	self.pushed = resources.getPath(data.pushed)
	self.selected = resources.getPath(data.selected)
end

function EditBoxTheme:init(data)
	AWidgetTheme.init(self, data)

	self.normal = resources.getPath(data.normal)
end

function ScrollBarTheme:init(data)
	AWidgetTheme.init(self, data)

	self.normal = resources.getPath(data.normal)
	self.scrollernormal = resources.getPath(data.scrollernormal)
	self.scrollerhover = resources.getPath(data.scrollerhover)
end

function SliderTheme:init(data)
	AWidgetTheme.init(self, data)

	self.normal = resources.getPath(data.normal)
	self.slidernormal = resources.getPath(data.slidernormal)
	self.sliderhover = resources.getPath(data.sliderhover)
end

function ProgressBarTheme:init(data)
	AWidgetTheme.init(self, data)

	self.normal = resources.getPath(data.normal)
	self.bar = resources.getPath(data.bar)
	self.mask = resources.getPath(data.mask)
end

function TextBoxTheme:init(data)
	AWidgetTheme.init(self, data)

end

function WidgetListTheme:init(data)
	AWidgetTheme.init(self, data)

end

function Theme:_loadFonts(data)
	local fontList = data.fonts
	if (nil == fontList) then
		return
	end

	for k, v in pairs(fontList) do
		fonts.loadFont(k, v.fileType, v.name, v.size)
	end
end

function Theme:_loadWidgetTheme(name, data, func)
	local widgetData = data[name]
	if (nil == widgetData) then
		return nil
	end

	return func(widgetData)
end

function Theme:_load(fileName)
	local data = utilities.loadFileData(fileName)
	if (nil == data) then
		return
	end

	self:_loadFonts(data)

	self.labelTheme = self:_loadWidgetTheme("label", data, LabelTheme)
	self.buttonTheme = self:_loadWidgetTheme("button", data, ButtonTheme)
	self.checkBoxTheme = self:_loadWidgetTheme("checkbox", data, CheckBoxTheme)
	self.radioButtonTheme = self:_loadWidgetTheme("radiobutton", data, RadioButtonTheme)
	self.editBoxTheme = self:_loadWidgetTheme("editbox", data, EditBoxTheme)
	self.vertScrollBarTheme = self:_loadWidgetTheme("vertscrollbar", data, ScrollBarTheme)
	self.horzScrollBarTheme = self:_loadWidgetTheme("horzscrollbar", data, ScrollBarTheme)
	self.vertSliderTheme = self:_loadWidgetTheme("vertslider", data, SliderTheme)
	self.horzSliderTheme = self:_loadWidgetTheme("horzslider", data, SliderTheme)
	self.progressBarTheme = self:_loadWidgetTheme("progressbar", data, ProgressBarTheme)
	self.textBoxTheme = self:_loadWidgetTheme("textbox", data, TextBoxTheme)
	self.widgetListTheme = self:_loadWidgetTheme("widgetlist", data, WidgetListTheme)
end

function Theme:init(fileName)
	self.texture = nil
	self.labelTheme = nil
	self.buttonTheme = nil
	self.checkBoxTheme = nil
	self.radioButtonTheme = nil
	self.editBoxTheme = nil
	self.vertScrollBarTheme = nil
	self.horzScrollBarTheme = nil
	self.vertSliderTheme = nil
	self.horzSliderTheme = nil
	self.progressBarTheme = nil
	self.textBoxTheme = nil
	self.widgetListTheme = nil

	self:_load(fileName)
end
