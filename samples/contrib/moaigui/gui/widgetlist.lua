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

	FILE: gui.lua
	DESCRIPTION: A scrollable list of items, allowing selection of these items
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

local array = require "gui\\support\\array"
local awindow = require "gui\\awindow"
local image = require "gui\\image"
local button = require "gui\\button"
local awidgetevent = require "gui\\awidgetevent"

local resources = require "gui\\support\\resources"

local ColumnWidget = class()
local WidgetListColumn = class(button.Button)
local WidgetListHeader = class(awindow.AWindow)
local WidgetListRow = class(image.Image)
_M.WidgetList = class(awindow.AWindow)

local SCROLL_BAR_WIDTH = 5
local HEADER_HEIGHT = 5
local ROW_HEIGHT = HEADER_HEIGHT

function ColumnWidget:init(obj, func, data)
	self.obj = obj
	self.func = func
	self.data = data
end

function WidgetListColumn:init(gui)
	button.Button.init(self, gui)

	self._type = "WidgetListColumn"
end

function WidgetListHeader:setHeaderText(col, text)
	if (col < 1 or col > #self._children) then
		return
	end

	self._children[col]:setText(text)
end

function WidgetListHeader:addColumn(text, width)
	local x = 0
	for i, v in ipairs(self._widgetChildren) do
		x = x + v:width()
	end

	local col = WidgetListColumn(self._gui)
	self:_addWidgetChild(col)
	col:setPos(x, 0)
	col:setDim(width, self._height)
	col:setText(text)
	col:setTextStyle(self._gui._factory._textStyle)
	col:setTextAlignment(col.TEXT_ALIGN_LEFT, col.TEXT_ALIGN_CENTER)
	col:setAllImages(resources.getPath("empty.png"))

	self._colWidths[#self._colWidths + 1] = width
end

function WidgetListHeader:getColumnWidths()
	return self._colWidths
end

function WidgetListHeader:init(gui, height)
	awindow.AWindow.init(self, gui)

	self._type = "WidgetListHeader"

	self._height = height
	self._colWidths = {}
end

function WidgetListRow:_onHandleMouseClick(event)
	self:toggleSelection()

	return true
end

function WidgetListRow:getIdx()
	return self._idx
end

function WidgetListRow:setWidget(idx, widget)
	local x = 0
	for i = 1, idx - 1 do
		x = x + self._colWidths[i]
	end

	widget:setPos(x, 0)
	widget:setDim(self._colWidths[idx], self._height)
	widget:setTextAlignment(widget.TEXT_ALIGN_LEFT, widget.TEXT_ALIGN_CENTER)
	widget:setInputPassThrough(true)
	self._items[idx] = widget
	self:addChild(widget)
end

function WidgetListRow:getCell(idx)
	if (idx < 1 or idx > #self._items) then
		return nil
	end

	return self._items[idx]
end

function WidgetListRow:setSelectionImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._IMAGE_INDEX, self.SELECTED_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)

	if (true == self._selected) then
		self:_setCurrImages(self._IMAGE_INDEX, self.SELECTED_IMAGES)
	end
end

function WidgetListRow:setSelectedTextStyle(style)
	self._selectedTextStyle = style
end

function WidgetListRow:setUnselectedTextStyle(style)
	self._unselectedTextStyle = style
end

function WidgetListRow:setSelected(flag)
	self._selected = flag
	local tex, style

	if (true == flag) then
		tex = self.SELECTED_IMAGES
		style = self._selectedTextStyle
		self._parent:_addSelection(self._idx)

	else
		tex = self.UNSELECTED_IMAGES
		style = self._unselectedTextStyle
		self._parent:_removeSelection(self._idx)
	end

	self:_setCurrImages(self._IMAGE_INDEX, tex)
	for i, v in ipairs(self._items) do
		v:setTextStyle(style)
	end
end

function WidgetListRow:getSelected()
	return self._selected
end

function WidgetListRow:toggleSelection()
	self:setSelected(not self:getSelected())
end

function WidgetListRow:init(gui, idx, widths, width, height)
	awindow.AWindow.init(self, gui)

	self._type = "WidgetListRow"

	self._idx = idx
	self._selected = false
	self._colWidths = widths
	self._height = height
	self._items = {}
	self._selectedTextStyle = nil
	self._unselectedTextStyle = nil

	self._IMAGE_INDEX = self._WIDGET_SPECIFIC_OBJECTS_INDEX
	self.SELECTED_IMAGES = self._WIDGET_SPECIFIC_IMAGES
	self.UNSELECTED_IMAGES = self.SELECTED_IMAGES + 1

	self:setDim(width, height)

	self:_setImage(self._rootProp, self._IMAGE_INDEX, self.UNSELECTED_IMAGES, resources.getPath("empty.png"))
	self:_setCurrImages(self._IMAGE_INDEX, self.UNSELECTED_IMAGES)
end

function _M.WidgetList:_createWidgetListSelectEvent(idx)
	local t = awidgetevent.AWidgetEvent(self.EVENT_WIDGET_LIST_SELECT, self)
	t.selection = idx

	return t
end

function _M.WidgetList:_createWidgetListUnselectEvent(idx)
	local t = awidgetevent.AWidgetEvent(self.EVENT_WIDGET_LIST_UNSELECT, self)
	t.selection = idx

	return t
end

function _M.WidgetList:_createWidgetListAddRowEvent(idx)
	local t = awidgetevent.AWidgetEvent(self.EVENT_WIDGET_LIST_ADD_ROW, self)
	t.row = idx

	return t
end

function _M.WidgetList:_createWidgetListRemoveRowEvent(idx)
	local t = awidgetevent.AWidgetEvent(self.EVENT_WIDGET_LIST_REMOVE_ROW, self)
	t.row = idx

	return t
end

function _M.WidgetList:_calcScrollBarPageSize()
	return math.floor(((self:height() - HEADER_HEIGHT) / self._rowHeight) + 0.5)
end

function _M.WidgetList:_onSetPos()
	if (nil ~= self._scrollBar) then
		self._scrollBar:setPos(self:width() - SCROLL_BAR_WIDTH, HEADER_HEIGHT)
	end
end

function _M.WidgetList:_onSetDim()
	if (nil ~= self._header) then
		self._header:setDim(self:width(), HEADER_HEIGHT)
	end

	if (nil ~= self._scrollBar) then
		self._scrollBar:setDim(SCROLL_BAR_WIDTH, self:height() - HEADER_HEIGHT)
		self._scrollBar:setPos(self:width() - SCROLL_BAR_WIDTH, HEADER_HEIGHT)
		self._scrollBar:setPageSize(self:_calcScrollBarPageSize())
	end
end

function _M.WidgetList:_addSelection(idx)
	if (#self._selections + 1 > self._maxSelections) then
		self:removeSelection(self._selections[#self._selections])
	end

	self._selections[#self._selections + 1] = idx

	local e = self:_createWidgetListSelectEvent(idx)

	self:_handleEvent(self.EVENT_WIDGET_LIST_SELECT, e)
end

function _M.WidgetList:_removeSelection(idx)
	local i = array.find(self._selections, idx)
	if (nil == i) then
		return 
	end

	table.remove(self._selections, i)

	local e = self:_createWidgetListUnselectEvent(idx)

	self:_handleEvent(self.EVENT_WIDGET_LIST_UNSELECT, e)
end

function _M.WidgetList:_displayRows()
	if (0 == #self._rows) then return end

	for i, v in ipairs(self._rows) do
		v:hide()
	end

	local minRow, maxRow

	minRow = math.min(#self._rows, self._scrollBar:getTopItem())
	maxRow = math.min(#self._rows, self._scrollBar:getTopItem() + self._scrollBar:getPageSize() - 1)

	for i = minRow, maxRow do
		self._rows[i]:show()
		self._rows[i]:setPos(0.5, HEADER_HEIGHT + (i - minRow) * self._rowHeight)
	end
end

function _M.WidgetList:_handleScrollPosChange(event)
	self:_displayRows()
end

function _M.WidgetList:addColumn(text, width)
	self._header:addColumn(text, width)
end

function _M.WidgetList:insertRow(before, data)
	local widths = self._header:getColumnWidths()
	local row = WidgetListRow(self._gui, #self._rows + 1, widths, self:width() - SCROLL_BAR_WIDTH, self._rowHeight)
	row:setUserData(data)
	self:_addWidgetChild(row)

	for i, v in ipairs(self._colWidgets) do
		if (nil ~= v.func) then
			local widget
			if (nil == v.obj) then
				widget = v.func()
			else
				widget = v.obj[v.func](v.obj)
			end

			widget:setUserData(v.data)
			row:setWidget(i, widget)
			row:setSelectedTextStyle(self._selectedTextStyle)
			row:setUnselectedTextStyle(self._unselectedTextStyle)

			local images = self._imageList:getImageType(self.SELECTION_IMAGES)
			for i2, v2 in ipairs(images) do
				row:setSelectionImage(v2.fileName, v2.colorData[1], v2.colorData[2], v2.colorData[3], v2.colorData[4], i2, v2.blendSrc, v2.blendDst)
			end
		end
	end

	table.insert(self._rows, before, row)

	for i = 1, #self._selections do
		if (self._selections[i] > before - 1) then
			self._selections[i] = self._selections[i] + 1
		end
	end

	self._scrollBar:setTopItem(1)
	self._scrollBar:setNumItems(self._scrollBar:getNumItems() + 1)

	self:_displayRows()

	local e = self:_createWidgetListAddRowEvent(before)

	self:_handleEvent(self.EVENT_WIDGET_LIST_ADD_ROW, e)

	return row
end

function _M.WidgetList:addRow(data)
	return self:insertRow(#self._rows + 1, data)
end

function _M.WidgetList:removeRow(idx)
	if (idx < 1 or idx > #self._rows) then return end

	local i = array.find(self._selections, idx)
	if (nil ~= i) then
		table.remove(self._selections, i)
	end

	for i = 1, #self._selections do
		if (self._selections[i] > idx) then
			self._selections[i] = self._selections[i] - 1
		end
	end

	for i = idx, #self._rows do
		self._rows[i]._idx = self._rows[i]._idx - 1
	end

	self._scrollBar:setTopItem(1)
	self._scrollBar:setNumItems(self._scrollBar:getNumItems() - 1)
	self:_removeWidgetChild(self._rows[idx])

	table.remove(self._rows, idx)

	self:_displayRows()

	local e = self:_createWidgetListRemoveRowEvent(before)

	self:_handleEvent(self.EVENT_WIDGET_LIST_REMOVE_ROW, e)
end

function _M.WidgetList:getRow(idx)
	return self._rows[idx]
end

function _M.WidgetList:numRows()
	return #self._rows
end

function _M.WidgetList:setRowHeight(height)
	self._rowHeight = height
	self._scrollBar:setPageSize(self:_calcScrollBarPageSize())
end

function _M.WidgetList:getRowHeight()
	return self._rowHeight
end

function _M.WidgetList:setColumnWidget(col, obj, func, data)
	local widget = ColumnWidget(obj, func, data)
	self._colWidgets[col] = widget
end

function _M.WidgetList:setBackgroundImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setImage(self._rootProp, self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES, fileName, r, g, b, a, idx, blendSrc, blendDst)
	self:_setCurrImages(self._BACKGROUND_INDEX, self.BACKGROUND_IMAGES)
end

function _M.WidgetList:getBackgroundImage()
	return self._imageList:getImage(self._EDIT_BOX_INDEX, self.BACKGROUND_IMAGES)
end

function _M.WidgetList:setSelectionImage(fileName, r, g, b, a, idx, blendSrc, blendDst)
	idx = (idx or 1)
	self._imageList:setImage(self.SELECTION_IMAGES, idx, fileName, r, g, b, a, blendSrc, blendDst)
end

function _M.WidgetList:getSelectionImage(idx)
	return self._imageList:getImage(self.SELECTION_IMAGES, idx)
end

function _M.WidgetList:setSelectedTextStyle(style)
	self._selectedTextStyle = style
end

function _M.WidgetList:getSelectedTextStyle()
	return self._selectedTextStyle
end

function _M.WidgetList:setUnselectedTextStyle(style)
	self._unselectedTextStyle = style
end

function _M.WidgetList:getUnselectedTextStyle()
	return self._unselectedTextStyle
end

function _M.WidgetList:setMaxSelections(num)
	self._maxSelections = num

	while (self._maxSelections < #self._selections) do
		self:removeSelection(self._selections[#self._selections])
	end
end

function _M.WidgetList:getMaxSelections()
	return self._maxSelections
end

function _M.WidgetList:setSelection(idx)
	if (idx < 1 or idx > #self._rows) then
		return
	end

	self._rows[idx]:setSelected(true)
end

function _M.WidgetList:removeSelection(idx)
	self:_removeSelection(idx)

	if (idx < 1 and idx > #self._rows) then
		return
	end

	self._rows[idx]:setSelected(false)
end

function _M.WidgetList:setSelections(selections)
	self:clearSelections()

	while (#selections > self._maxSelections) do
		table.remove(selections, #selections)
	end

	for i, v in ipairs(selections) do
		self:setSelection(v)
	end
end

function _M.WidgetList:getSelections()
	return self._selections
end

function _M.WidgetList:clearSelections()
	for i, v in ipairs(self._selections) do
		self:removeSelection(v)
	end

	self._selections = {}
end

function _M.WidgetList:clearList()
	self:clearSelections()

	while (#self._rows > 0) do
		self:removeRow(1)
	end
end

function _M.WidgetList:_WidgetListEvents()
	self.EVENT_WIDGET_LIST_SELECT = "EventWidgetListSelect"
	self.EVENT_WIDGET_LIST_UNSELECT = "EventWidgetListUnselect"
	self.EVENT_WIDGET_LIST_ADD_ROW = "EventWidgetListAddRow"
	self.EVENT_WIDGET_LIST_REMOVE_ROW = "EventWidgetListRemoveRow"
end

function _M.WidgetList:init(gui)
	awindow.AWindow.init(self, gui)

	self._type = "WidgetList"

	self:_WidgetListEvents()

	self._BACKGROUND_INDEX = self._WIDGET_SPECIFIC_OBJECTS_INDEX
	self.BACKGROUND_IMAGES = self._WIDGET_SPECIFIC_IMAGES

	self._SELECTION_INDEX = self._BACKGROUND_INDEX + 1
	self.SELECTION_IMAGES = self.BACKGROUND_IMAGES + 1

	self._rowHeight = 0
	self._colWidgets = {}
	self._rows = {}
	self._selections = {}
	self._selectionImage = nil
	self._maxSelections = 1
	self._selectedTextStyle = nil
	self._unselectedTextStyle = nil

	self._scrollBar = gui:createVertScrollBar()
	self:_addWidgetChild(self._scrollBar)
	self._scrollBar:registerEventHandler(self._scrollBar.EVENT_SCROLL_BAR_POS_CHANGED, self, "_handleScrollPosChange")

	self._header = WidgetListHeader(gui, HEADER_HEIGHT)
	self:_addWidgetChild(self._header)

	self:setRowHeight(ROW_HEIGHT)
end

return _M
