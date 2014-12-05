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

	FILE: layoutparser.lua
	DESCRIPTION: Loads a file containing gui layout information, and creates the appropriate widgets
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	UPDATED: 4-27-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3

	NOTES
	- _handleTextAlign *must* be before _baseFromData, as MOAITextBox seems to lose text alignment
	  information if setStringColor is called before setAlignment.
]]

local _M = {}

require "gui\\support\\class"

local utilities = require "gui\\support\\utilities"
local resources = require "gui\\support\\resources"
local fonts = require "gui\\fonts"

_M.LayoutParser = class()

function _M.LayoutParser:_handleTextAlign(win, data)
	local horzText = data.textHorzAlign
	local vertText = data.textVertAlign

	if (nil ~= horzText and nil ~= vertText) then
		local horz, vert
		if ("left" == string.lower(horzText)) then
			horz = win.TEXT_ALIGN_LEFT
		elseif ("right" == string.lower(horzText)) then
			horz = win.TEXT_ALIGN_RIGHT
		end

		if ("top" == string.lower(vertText)) then
			vert = win.TEXT_ALIGN_TOP
		elseif ("bottom" == string.lower(vertText)) then
			vert = win.TEXT_ALIGN_BOTTOM
		end
		win:setTextAlignment(horz, vert)
	end
end

function _M.LayoutParser:_baseFromData(w, data)
	if (nil ~= data.pos) then
		w:setPos(unpack(data.pos))
	end

	if (nil ~= data.dim) then
		w:setDim(unpack(data.dim))
	end

	if (nil ~= data.text) then
		w:setText(data.text)
	end

	if (nil ~= data.userData) then
		w:setUserData(data.userData)
	end

	if (nil ~= data.font) then
		local font = fonts.loadFont(data.font.type, data.font.name, data.font.size)
		w:setTextFont(font, data.font.size)
	end

	if (nil ~= data.textColor) then
		w:setTextColor(data.textColor[1], data.textColor[2], data.textColor[3], data.textColor[4])
	end

	if (nil ~= data.alpha) then
		w:setAlpha(data.alpha)
	end

	if (nil ~= data.draggable and true == data.draggable) then
		w:setDraggable(true)
	end

	if (nil ~= data.inputPassThrough and true == data.inputPassThrough) then
		w:setInputPassThrough(true)
	end

	return base
end

function _M.LayoutParser:_windowFromData(name, data)
	local w = self._factory:create("window")
	self:_baseFromData(w, data)
	w:setName(name)

	local bg = ""
	local bgColor = {1, 1, 1, 1}
	if (nil ~= data.background) then
		bg = data.background
	end

	if (nil ~= data.backgroundColor) then
		bgColor = data.backgroundColor
	end

	w:setBackgroundImage(resources.getPath(bg), bgColor[1], bgColor[2], bgColor[3], bgColor[4])

	return w
end

function _M.LayoutParser:_labelFromData(name, data)
	local w = self._factory:create("label")
	self:_handleTextAlign(w, data)
	self:_baseFromData(w, data)
	w:setName(name)

	return w
end

function _M.LayoutParser:_calcImageTable(data, idx)
	local t = {}
	t[#t + 1] = resources.getPath(data.fileName)
	local color = data.color
	local blend = data.blend

	if (nil == color) then color = {1, 1, 1, 1} end
	if (nil == blend) then blend = {MOAIProp.BLEND_NORMAL} end

	for i = 1, math.max(4, math.min(4, #color)) do
		t[#t + 1] = color[i]
	end

	t[#t + 1] = idx

	for i = 1, math.max(2, math.min(2, #blend)) do
		t[#t + 1] = blend[i]
	end

	return t
end

function _M.LayoutParser:_imageFromData(name, data)
	local w = self._factory:create("image")
	self:_handleTextAlign(w, data)
	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.images) then
		for i = 1, #data.images do
			local t = self:_calcImageTable(data.images[i], i)

			w:setImage(unpack(t))
		end
	end

	return w
end

function _M.LayoutParser:_buttonFromData(name, data)
	local w = self._factory:create("button")
	self:_handleTextAlign(w, data)
	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.images) then
		if (nil ~= data.images.normal) then
			for i = 1, #data.images.normal do
				local t = self:_calcImageTable(data.images.normal[i], i)
				w:setNormalImage(unpack(t))
			end
		end

		if (nil ~= data.images.hover) then
			for i = 1, #data.images.hover do
				local t = self:_calcImageTable(data.images.hover[i], i)
				w:setHoverImage(unpack(t))
			end
		end

		if (nil ~= data.images.pushed) then
			for i = 1, #data.images.pushed do
				local t = self:_calcImageTable(data.images.pushed[i], i)
				w:setPushedImage(unpack(t))
			end
		end

		if (nil ~= data.images.disabled) then
			for i = 1, #data.images.disabled do
				local t = self:_calcImageTable(data.images.disabled[i], i)
				w:setDisabledImage(unpack(t))
			end
		end
	end

	if (nil ~= data.data) then
		w:setOnClickData(data.data)
	end

	return w
end

function _M.LayoutParser:_checkBoxFromData(name, data)
	local w = self._factory:create("check box")
	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.checked) then
		w:setChecked(data.checked)
	end

	return w
end

function _M.LayoutParser:_radioButtonFromData(name, data)
	local w = self._factory:create("radio button")
	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.selected) then
		w:setSelected(data.selected)
	end

	if (nil ~= data.radioButtonGroup) then
		w:setGroup(data.radioButtonGroup)
	end

	return w
end

function _M.LayoutParser:_editBoxFromData(name, data)
	local w = self._factory:create("edit box")
	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.maxLength) then
		w:setMaxLength(data.maxLength)
	end

	if (nil ~= data.allowedChars) then
		w:setAllowedChars(data.allowedChars)
	end

	return w
end

function _M.LayoutParser:_sliderFromData(name, data)
	local w

	if (true == data.vert) then
		w = self._factory:create("vert slider")
	else
		w = self._factory:create("horz slider")
	end

	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.maxValue) then
		w:setMaxValue(data.maxValue)
	end

	if (nil ~= data.minValue) then
		w:setMinValue(data.minValue)
	end

	if (nil ~= data.currValue) then
		w:setCurrValue(data.currValue)
	end

	if (nil ~= data.step) then
		w:setStep(step)
	end
	
	if (nil ~= data.valueLoc) then
		local loc = data.valueLoc
		if ("top" == loc) then
			w:setValueDisplayLoc(w.VALUE_DISPLAY_TOP)
		elseif ("bottom" == loc) then
			w:setValueDisplayLoc(w.VALUE_DISPLAY_BOTTOM)
		elseif ("left" == loc) then
			w:setValueDisplayLoc(w.VALUE_DISPLAY_LEFT)
		elseif ("right" == loc) then
			w:setValueDisplayLoc(w.VALUE_DISPLAY_RIGHT)
		end
	end

	return w
end

function _M.LayoutParser:_scrollBarFromData(name, data)
	local w

	if (true == data.horz) then
		w = self._factory:create("horz scroll bar")
	else
		s = self._factory:create("vert scroll bar")
	end

	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.numItems) then
		w:setNumItems(data.numItems)
	end

	if (nil ~= data.pageSize) then
		w:setPageSize(data.pageSize)
	end

	return w
end

function _M.LayoutParser:_progressBarFromData(name, data)
	local w = self._factory:create("progress bar")
	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.progress) then
		w:setProgress(data.progress)
	end

	if (nil ~= data.stepSize) then
		w:setStepSize(data.stepSize)
	end

	return w
end

function _M.LayoutParser:_textBoxFromData(name, data)
	local w = self._factory:create("text box")
	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.lineHeight) then
		w:setLineHeight(data.lineHeight)
	end

	return w
end

function _M.LayoutParser:_widgetListFromData(name, data)
	local w = self._factory:create("widget list")
	self:_baseFromData(w, data)
	w:setName(name)

	if (nil ~= data.columns) then
		for i, v in ipairs(data.columns) do
			w:addColumn(v[1], v[2])

			local func = "_create" .. v[3]:sub(1, 1):upper() .. v[3]:sub(2)
			w:setColumnWidget(i, self._factory, func)
		end
	end

	if (nil ~= data.selectionImage) then
		w:setSelectionImage(resources.getPath(data.selectionImage))
	end

	if (nil ~= data.maxSelect) then
		w:setMaxSelections(data.maxSelect)
	end

	if (nil ~= data.rowHeight) then
		w:setRowHeight(data.rowHeight)
	end

	local bg = ""
	local bgColor = {1, 1, 1, 1}
	if (nil ~= data.background) then
		bg = data.background
	end

	if (nil ~= data.backgroundColor) then
		bgColor = data.backgroundColor
	end

	w:setBackgroundImage(resources.getPath(bg), bgColor[1], bgColor[2], bgColor[3], bgColor[4])

	return w
end

function _M.LayoutParser:_registerWidgetCreateFuncs()
	self:registerWidgetCreateFunc("window", self._windowFromData)
	self:registerWidgetCreateFunc("label", self._labelFromData)
	self:registerWidgetCreateFunc("image", self._imageFromData)
	self:registerWidgetCreateFunc("button", self._buttonFromData)
	self:registerWidgetCreateFunc("check box", self._checkBoxFromData)
	self:registerWidgetCreateFunc("radio button", self._radioButtonFromData)
	self:registerWidgetCreateFunc("edit box", self._editBoxFromData)
	self:registerWidgetCreateFunc("slider", self._sliderFromData)
	self:registerWidgetCreateFunc("scroll bar", self._scrollBarFromData)
	self:registerWidgetCreateFunc("progress bar", self._progressBarFromData)
	self:registerWidgetCreateFunc("text box", self._textBoxFromData)
	self:registerWidgetCreateFunc("widget list", self._widgetListFromData)
end

function _M.LayoutParser:_handleIncludes(children)
	if (nil == children["include"]) then
		return
	end

	for i = 1, #children["include"] do
		local includeData = utilities.loadFileData(resources.getPath(children["include"][i]))

		if (nil ~= includeData) then
			if (nil ~= includeData["include"]) then
				self:_handleIncludes(includeData)
			end
			
			for k, v in pairs(includeData) do
				children[k] = v
			end
		end
	end

	children["include"] = nil
end

function _M.LayoutParser:_parseWidgetData(fileName, prefix, parent, name, data, widgets, currChildren, groups)
	if (nil == data.widget) then
		return
	end

	local widgetType = string.lower(data.widget)
	if (nil == self._widgetCreateFuncs[widgetType]) then
		print("Error - unrecognized widget type: " .. widgetType, name)
		return
	end

	name = prefix .. name
	local widget = self._widgetCreateFuncs[widgetType](self, name, data)
	widgets[name] = {}
	widgets[name]["window"] = widget

	if (nil ~= currChildren) then
		currChildren[name] = {}
		currChildren[name]["window"] = widget
	end

	if (nil ~= parent) then
		parent:addChild(widget)
	end

	if (nil ~= data["group"]) then
		local groupName = data.group
		if (nil == groups[groupName]) then
			groups[groupName] = group.Group()
		end

		groups[groupName]:addObject(widget)
	end

	if (nil ~= data["children"]) then
		local children = {}
		if (nil == currChildren) then
			widgets[name]["children"] = children
		else
			currChildren[name]["children"] = children
		end

		if (nil ~= data["children"]["include"]) then
			self:_handleIncludes(data["children"])
		end

		for k, v in pairs(data["children"]) do
			self:_parseWidgetData(fileName, prefix, widget, k, v, widgets, children, groups)
		end
	end
end

function _M.LayoutParser:createFromLayout(fileName, prefix, parent, params)
	local roots = {}
	local widgets = {}
	local groups = {}

	local data = utilities.loadFileData(fileName)

	if (nil == data) then
		return roots, widgets, groups
	end

	if (nil == prefix) then prefix = "" end
	if (nil == params) then params = {} end

	-- if (nil ~= data.create) then
		-- data.create(params)
	-- end

	for k, v in pairs(data) do
		table.insert(roots, prefix .. k)
		self:_parseWidgetData(fileName, prefix, parent, k, v, widgets, nil, groups)
	end

	return roots, widgets, groups
end

function _M.LayoutParser:registerWidgetCreateFunc(name, func)
	self._widgetCreateFuncs[name] = func
end

function _M.LayoutParser:init(factory)
	self._factory = factory

	self._widgetCreateFuncs = {}

	self:_registerWidgetCreateFuncs()
end

return _M
