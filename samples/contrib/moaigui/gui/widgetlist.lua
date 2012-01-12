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
]]

module(..., package.seeall)

require "gui\\support\\class"

local array = require "gui\\support\\array"
local awindow = require "gui\\awindow"
local image = require "gui\\image"
local button = require "gui\\button"
local awidgetevent = require "gui\\awidgetevent"

ColumnWidget = class()
WidgetListColumn = class(button.Button)
WidgetListHeader = class(awindow.AWindow)
WidgetListRow = class(awindow.AWindow)
WidgetList = class(awindow.AWindow)

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
	col:setTextFont(self._gui._factory._font)
	col:setTextAlignment(col.TEXT_ALIGN_LEFT, col.TEXT_ALIGN_CENTER)

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
	local widgetParent = image.Image(self._gui)
	self:_addWidgetChild(widgetParent)
	widgetParent:_addWidgetChild(widget)
	self._items[idx] = widgetParent

	local x = 0
	local y = 0
	for i = 1, idx - 1 do
		x = x + self._colWidths[i]
	end

	widgetParent:setPos(x, 0)
	widgetParent:setDim(self._colWidths[idx], self._height)

	widget:setPos(0, 0)
	widget:setDim(self._colWidths[idx], self._height)
	widget:setTextAlignment(widget.TEXT_ALIGN_LEFT, widget.TEXT_ALIGN_CENTER)

end

function WidgetListRow:getCell(idx)
	if (idx < 1 or idx > #self._items) then
		return nil
	end

	return self._items[idx]:_getWidgetChild(1)
end

function WidgetListRow:setSelectionImage(image)
	self._selectionImage = image
end

function WidgetListRow:setSelectedTextColor(r, g, b, a)
	self._selectedTextColor = {r, g, b, a}
end

function WidgetListRow:setUnselectedTextColor(r, g, b, a)
	self._unselectedTextColor = {r, g, b, a}
end

function WidgetListRow:setSelected(flag)
	self._selected = flag
	local tex, color

	if (true == flag) then
		tex = self._selectionImage
		color = self._selectedTextColor
		self._parent:_addSelection(self._idx)
	else
		tex = nil
		color = self._unselectedTextColor
		self._parent:_removeSelection(self._idx)
	end

	for i, v in ipairs(self._items) do
		v:setImage(tex)
		v:_getWidgetChild(1):setTextColor(color[1], color[2], color[3], color[4])
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
	self._selectionImage = nil
	self._selectedTextColor = {0, 0, 0, 1}
	self._unselectedTextColor = {1, 1, 1, 1}

	for i = 1, #widths do
		table.insert(self._items, nil)
	end

	self:setDim(width, height)
end

function WidgetList:_createWidgetListSelectEvent(idx)
	local t = awidgetevent.AWidgetEvent(self.EVENT_WIDGET_LIST_SELECT, self)
	t.selection = idx

	return t
end

function WidgetList:_createWidgetListUnselectEvent(idx)
	local t = awidgetevent.AWidgetEvent(self.EVENT_WIDGET_LIST_UNSELECT, self)
	t.selection = idx

	return t
end

function WidgetList:_createWidgetListAddRowEvent(idx)
	local t = awidgetevent.AWidgetEvent(self.EVENT_WIDGET_LIST_ADD_ROW, self)
	t.row = idx

	return t
end

function WidgetList:_createWidgetListRemoveRowEvent(idx)
	local t = awidgetevent.AWidgetEvent(self.EVENT_WIDGET_LIST_REMOVE_ROW, self)
	t.row = idx

	return t
end

function WidgetList:_calcScrollBarPageSize()
	return math.floor(((self:height() - HEADER_HEIGHT) / self._rowHeight) + 0.5)
end

function WidgetList:_onSetPos()
	self._scrollBar:setPos(self:width() - SCROLL_BAR_WIDTH, HEADER_HEIGHT)
end

function WidgetList:_onSetDim()
	self._header:setDim(self:width(), HEADER_HEIGHT)
	self._scrollBar:setDim(SCROLL_BAR_WIDTH, self:height() - HEADER_HEIGHT)
	self._scrollBar:setPos(self:width() - SCROLL_BAR_WIDTH, HEADER_HEIGHT)
	self._scrollBar:setPageSize(self:_calcScrollBarPageSize())
end

function WidgetList:_addSelection(idx)
	if (#self._selections + 1 > self._maxSelections) then
		self:removeSelection(self._selections[#self._selections])
	end

	self._selections[#self._selections + 1] = idx

	local e = self:_createWidgetListSelectEvent(idx)

	self:_handleEvent(self.EVENT_WIDGET_LIST_SELECT, e)
end

function WidgetList:_removeSelection(idx)
	local i = array.find(self._selections, idx)
	if (nil == i) then
		return 
	end

	table.remove(self._selections, i)

	local e = self:_createWidgetListUnselectEvent(idx)

	self:_handleEvent(self.EVENT_WIDGET_LIST_UNSELECT, e)
end

function WidgetList:_displayRows()
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

function WidgetList:_handleScrollPosChange(event)
	self:_displayRows()
end

function WidgetList:addColumn(text, width)
	self._header:addColumn(text, width)
end

function WidgetList:insertRow(before, data)
	local widths = self._header:getColumnWidths()
	local row = WidgetListRow(self._gui, #self._rows + 1, widths, self:width() - SCROLL_BAR_WIDTH, self._rowHeight)
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
			row:setSelectionImage(self._selectionImage)
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

function WidgetList:addRow(data)
	return self:insertRow(#self._rows + 1, data)
end

function WidgetList:removeRow(idx)
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

function WidgetList:getRow(idx)
	return self._rows[idx]
end

function WidgetList:numRows()
	return #self._rows
end

function WidgetList:setRowHeight(height)
	self._rowHeight = height
	self._scrollBar:setPageSize(self:_calcScrollBarPageSize())
end

function WidgetList:getRowHeight()
	return self._rowHeight
end

function WidgetList:setColumnWidget(col, obj, func, data)
	local widget = ColumnWidget(obj, func, data)
	self._colWidgets[col] = widget
end

function WidgetList:setBackground(image, r, g, b, a)
	if (nil == r) then r = 1 end
	if (nil == g) then g = 1 end
	if (nil == b) then b = 1 end
	if (nil == a) then a = 1 end

	self._quads[self._BASE_OBJECTS_INDEX][1]:setTexture(image)
	self._props[self._BASE_OBJECTS_INDEX][1]:setColor(r, g, b, a)
	self._image = image
	self._color = {r, g, b, a}
end

function WidgetList:getBackground()
	return self._image
end

function WidgetList:getBackgroundColor()
	return self._color
end

function WidgetList:setSelectionImage(image)
	self._selectionImage = image
end

function WidgetList:getSelectionImage()
	return self._selectionImage
end

function WidgetList:setMaxSelections(num)
	self._maxSelections = num

	while (self._maxSelections < #self._selections) do
		self:removeSelection(self._selections[#self._selections])
	end
end

function WidgetList:getMaxSelections()
	return self._maxSelections
end

function WidgetList:setSelection(idx)
	if (idx < 1 or idx > #self._rows) then
		return
	end

	self._rows[idx]:setSelected(true)
end

function WidgetList:removeSelection(idx)
	self:_removeSelection(idx)

	if (idx < 1 and idx > #self._rows) then
		return
	end

	self._rows[idx]:setSelected(false)
end

function WidgetList:setSelections(selections)
	self:clearSelections()

	while (#selections > self._maxSelections) do
		table.remove(selections, #selections)
	end

	for i, v in ipairs(selections) do
		self:setSelection(v)
	end
end

function WidgetList:getSelections()
	return self._selections
end

function WidgetList:clearSelections()
	for i, v in ipairs(self._selections) do
		self:removeSelection(v)
	end

	self._selections = {}
end

function WidgetList:clearList()
	self:clearSelections()

	while (#self._rows > 0) do
		self:removeRow(1)
	end
end

function WidgetList:_WidgetListEvents()
	self.EVENT_WIDGET_LIST_SELECT = "EventWidgetListSelect"
	self.EVENT_WIDGET_LIST_UNSELECT = "EventWidgetListUnselect"
	self.EVENT_WIDGET_LIST_ADD_ROW = "EventWidgetListAddRow"
	self.EVENT_WIDGET_LIST_REMOVE_ROW = "EventWidgetListRemoveRow"
end

function WidgetList:init(gui)
	awindow.AWindow.init(self, gui)

	self._type = "WidgetList"

	self:_WidgetListEvents()

	self._image = nil
	self._rowHeight = 0
	self._colWidgets = {}
	self._rows = {}
	self._selections = {}
	self._selectionImage = nil
	self._maxSelections = 1

	self._scrollBar = gui:createVertScrollBar()
	self:_addWidgetChild(self._scrollBar)
	self._scrollBar:registerEventHandler(self._scrollBar.EVENT_SCROLL_BAR_POS_CHANGED, self, "_handleScrollPosChange")

	self._header = WidgetListHeader(gui, HEADER_HEIGHT)
	self:_addWidgetChild(self._header)

	self:setRowHeight(ROW_HEIGHT)
end
