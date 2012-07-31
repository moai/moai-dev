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
	DESCRIPTION: The managing class for the gui system
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	UPDATED: 5-5-12
	VERSION: 0.2
	MOAI VERSION: v1.0 r3

	Changes
	- Changed function _calcInputOver to use MOAIPartition:propListForPoint instead of MOAIPartition:propForPoint.
	  Now, windows that are set to be hidden will not accept input.
]]

local _M = {}

require "gui\\support\\class"

local resources = require "gui\\support\\resources"
local filesystem = require "gui\\support\\filesystem"

local array = require "gui\\support\\array"
local aevent = require "gui\\aevent"
local factory = require "gui\\factory"
local layoutparser = require "gui\\layoutparser"
local theme = require "gui\\theme"
local fonts = require "gui\\fonts"
local textstyles = require "gui\\textstyles"
local textures = require "gui\\textures"

local constants = require "gui\\guiconstants"

local eventtypes = require "gui\\support\\eventtypes"

_M.GUI = class()

local GUI_PRIORITY = 0
local BASE_PRIORITY = 0
local RESOURCE_GROUP = nil

function _M.GUI:_calcAbsValue(x, y)
	x = (x / 100.0) * self._width
	y = (y / 100.0) * self._height

	return x, y
end

function _M.GUI:_calcRelValue(x, y)
	x = (x * 100.0) / self._width
	y = (y * 100.0) / self._height

	return x, y
end

function _M.GUI:_createGUILayer(width, height)
	local layer = MOAILayer.new()

	local viewport = MOAIViewport.new()
	viewport:setSize(width, height)
	viewport:setScale(width, height)
	viewport:setOffset(-1, 1)
	layer:setViewport(viewport)

	local partition = MOAIPartition.new()
	layer:setPartition(partition)
	
	self._partition = partition
	self._layer = layer
	self._viewport = viewport
end

function _M.GUI:_registerHitObject(widget, prop)
	self._propToWindow[prop] = widget
end

function _M.GUI:_unregisterHitObject(prop)
	self._propToWindow[prop] = nil
end

function _M.GUI:_createProp(priority)
	priority = (priority or BASE_PRIORITY)

	local prop = MOAIProp.new()

	prop:setPriority(priority)
	prop:setLoc(0, 0)
	prop:setScl(1, 1)

	self:partition():insertProp(prop)

	return prop
end

function _M.GUI:_createQuad()
	local quad = MOAIGfxQuad2D.new()

	return quad
end

function _M.GUI:_destroyProp(prop)
	prop:setDeck(nil)
	self:partition():removeProp(prop)
end

function _M.GUI:_createMouseButtonEvent(eventType, x, y, button, down)
	local event = aevent.AEvent(eventType)

	event.x = x
	event.y = y
	event.button = button
	event.down = down

	return event
end

function _M.GUI:_createMouseMoveEvent(currX, currY, prevX, prevY)
	local event = aevent.AEvent(constants.GUI_MOUSE_MOVE)

	event.x = currX
	event.y = currY
	event.prevX = prevX
	event.prevY = prevY
	
	return event
end

function _M.GUI:_createMouseClickEvent(x, y, button)
	local event = aevent.AEvent(constants.GUI_MOUSE_CLICK)

	event.x = x
	event.y = y
	event.button = button

	return event
end

function _M.GUI:_createMouseEntersEvent()
	local event = aevent.AEvent(constants.GUI_MOUSE_ENTERS)
	
	return event
end

function _M.GUI:_createMouseLeavesEvent()
	local event = aevent.AEvent(constants.GUI_MOUSE_LEAVES)
	
	return event
end

function _M.GUI:_createKeyUpEvent(key)
	local event = aevent.AEvent(constants.GUI_KEY_UP)

	event.key = key

	return event
end

function _M.GUI:_createKeyDownEvent(key)
	local event = aevent.AEvent(constants.GUI_KEY_DOWN)

	event.key = key

	return event
end

function _M.GUI:_createTouchUpEvent()
	local event = aevent.AEvent(constants.GUI_TOUCH_UP)

	return event
end

function _M.GUI:_createTouchDownEvent()
	local event = aevent.AEvent(constants.GUI_TOUCH_DOWN)

	return event
end

function _M.GUI:_createTouchMoveEvent()
	local event = aevent.AEvent(constants.GUI_TOUCH_MOVE)

	return event
end

function _M.GUI:_createTouchTapEvent()
	local event = aevent.AEvent(constants.GUI_TOUCH_TAP)

	return event
end

function _M.GUI:_createTouchEntersEvent()
	local event = aevent.AEvent(constants.GUI_TOUCH_ENTERS)

	return event
end

function _M.GUI:_createTouchLeavesEvent()
	local event = aevent.AEvent(constants.GUI_TOUCH_LEAVES)

	return event
end

function _M.GUI:_createButtonEvent()
	local event = aevent.AEvent(constants.GUI_BUTTON)

	return event
end

function _M.GUI:_createMotionEvent()
	local event = aevent.AEvent(constants.GUI_MOTION)

	return event
end

function _M.GUI:_createGainFocusEvent(widget)
	local event = aevent.AEvent(constants.GUI_GAIN_FOCUS)
	event.widget = widget

	return event
end

function _M.GUI:_createLoseFocusEvent(widget)
	local event = aevent.AEvent(constants.GUI_LOSE_FOCUS)
	event.widget = widget

	return event
end

function _M.GUI:viewport()
	return self._viewport
end

function _M.GUI:partition()
	return self._partition
end

function _M.GUI:layer()
	return self._layer
end

function _M.GUI:_calcInputOver(x, y)
	local inputX, inputY = self._layer:wndToWorld(x, y)

	local props = {self._partition:propListForPoint(inputX, inputY, 0, MOAILayer.SORT_PRIORITY_DESCENDING)}
	if (nil == props or #props == 0) then return nil end

	for i, v in ipairs(props) do
		local win = self._propToWindow[v]
		if (nil ~= win) then
			if (true == win:getVisible()) then
				return win
			end
		end
	end

	return nil
end

function _M.GUI:_handleMouseOverWidget(widget)
	if (widget ~= self._over) then
		if (nil ~= self._over) then
			local event = self:_createMouseLeavesEvent()
			self._over:_handleMouseLeaves(event)
		end

		if (nil ~= widget) then
			local event = self:_createMouseEntersEvent()
			widget:_handleMouseEnters(event)
		end

		self:setOver(widget)
	end
end

function _M.GUI:_handleTouchOverWidget(widget)
	if (widget ~= self._over) then
		if (nil ~= self._over) then
			self._over:_handleTouchLeaves()
		end

		if (nil ~= widget) then
			widget:_handleTouchEnters()
		end

		self:setOver(widget)
	end
end

function _M.GUI:setFocus(win)
	if (nil == win) then
		if (nil ~= self._focus) then
			local event = self:_createLoseFocusEvent(self._focus)
			self._focus:_handleLoseFocus(event)
		end

		self._focus = nil
		return
	end

	if (self._focus ~= win) then
		local prev = self._focus
		self._focus = win

		local event = self:_createGainFocusEvent(win)
		self._focus:_handleGainFocus(event)

		if (nil ~= prev) then
			local event = self:_createLoseFocusEvent(win)
			prev:_handleLoseFocus(event)
		end
	end
end

function _M.GUI:getFocus()
	return self._focus
end

function _M.GUI:setOver(win)
	if (nil == win) then
		if (nil ~= self._over) then
			for k, v in pairs(self._mouseButtonsDown) do
				if (v == win) then
					self._mouseButtonsDown[k] = nil
					break
				end
			end
		end

		self._over = nil
		return
	end

	self._over = win
end

function _M.GUI:getOver()
	return self._over
end

function _M.GUI:moveToFront(win)
	local top = BASE_PRIORITY
	for k, v in pairs(self._windows) do
		top = math.max(top, v:getPriority())
	end

	win:_setPriority(top + 5)
end

function _M.GUI:moveToBack(win)
	for k, v in pairs(self._windows) do
		v:_setPriority(v:getPriority() + 1)
	end

	win:_setPriority(BASE_PRIORITY)
end

function _M.GUI:injectKeyDown(key)
	self._keysDown[key] = true

	local result = false
	local event = self:_createKeyDownEvent(key)
	if (nil ~= self._focus) then
		result = self._focus:_handleKeyDown(event)
	end

	return result
end

function _M.GUI:injectKeyUp(key)
	table.remove(self._keysDown, key)

	local result = false
	local event = self:_createKeyUpEvent(key)
	if (nil ~= self._focus) then
		result = self._focus:_handleKeyUp(event)
	end

	return result
end

function _M.GUI:injectMouseMove(newX, newY)
	self._prevMouseX = self._currMouseX
	self._prevMouseY = self._currMouseY
	self._currMouseX = newX
	self._currMouseY = newY
	
	local widget = self:_calcInputOver(newX, newY)
	self:_handleMouseOverWidget(widget)

	local result = false
	local event = self:_createMouseMoveEvent(self._currMouseX, self._currMouseY, self._prevMouseX, self._prevMouseY)
	if (nil ~= widget) then
		result = widget:_handleMouseMove(event)
	end

	if (widget ~= self._focus and nil ~= self._focus) then
		result = self._focus:_handleMouseMove(event)
	end

	return result
end

function _M.GUI:injectMouseButtonDown(button)
	self._mouseButtonsDown[button] = self._over

	self:setFocus(self._over)

	local result = false
	if (nil ~= self._focus) then
		local event = self:_createMouseButtonEvent(constants.GUI_MOUSE_BUTTON_DOWN, self._currMouseX, self._currMouseY, button, true)
		result = self._focus:_handleMouseDown(event)
	end

	return result
end

function _M.GUI:injectMouseButtonUp(button)
	local result = false
	if (nil ~= self._mouseButtonsDown[button]) then
		local event = self:_createMouseButtonEvent(constants.GUI_MOUSE_BUTTON_UP, self._currMouseX, self._currMouseY, button, false)
		self._mouseButtonsDown[button]:_handleMouseUp(event)
		if (self._mouseButtonsDown[button] == self._over) then
			local event = self:_createMouseClickEvent(self._currMouseX, self._currMouseY, button)
			result = self._over:_handleMouseClick(event)
		end
	end

	self._mouseButtonsDown[button] = nil

	return result
end

function _M.GUI:injectTouch(eventType, idx, x, y, tapCount)
	local newTouch = {}

	newTouch.type = eventType
	newTouch.x = x
	newTouch.y = y
	newTouch.tapCount = tapCount
	
	if (touches[idx] ~= nil) then
		newTouch.prevX = touches[idx].x
		newTouch.prevY = touches[idx].y
	end
	
	self._touches[idx] = newTouch
	
	local widget = self:_calcInputOver(x, y)
	return self:_handleTouchOverWidget(widget)
end

function _M.GUI:injectButton()

end

function _M.GUI:injectMotion()

end

function _M.GUI:_addWindow(w)
	self._windows[#self._windows + 1] = w
	self:moveToFront(w)
end

function _M.GUI:createWindow(...)
	local w = self._factory:create("window", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createLabel(...)
	local w = self._factory:create("label", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createImage(...)
	local w = self._factory:create("image", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createButton(...)
	local w = self._factory:create("button", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createCheckBox(...)
	local w = self._factory:create("check box", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createRadioButton(...)
	local w = self._factory:create("radio button", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createEditBox(...)
	local w = self._factory:create("edit box", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createVertSlider(...)
	local w = self._factory:create("vert slider", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createHorzSlider(...)
	local w = self._factory:create("horz slider", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createVertScrollBar(...)
	local w = self._factory:create("vert scroll bar", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createHorzScrollBar(...)
	local w = self._factory:create("horz scroll bar", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createProgressBar(...)
	local w = self._factory:create("progress bar", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createTextBox(...)
	local w = self._factory:create("text box", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:createWidgetList(...)
	local w = self._factory:create("widget list", ...)

	self:_addWindow(w)

	return w
end

function _M.GUI:destroyWindow(win)
	win:destroy()

	array.removeElement(self._windows, win)
end

function _M.GUI:addToResourcePath(path, priority)
	resources.addToPath(path, RESOURCE_GROUP, priority)
end

function _M.GUI:setTheme(fileName)
	fileName = resources.getPath(fileName, RESOURCE_GROUP)
	if (nil == fileName) then
		return
	end

	local t = self._themes[fileName]
	if (nil == t) then
		t = theme.Theme(fileName)
		self._themes[fileName] = t
	end

	self._currTheme = fileName
	self._factory:setTheme(t)
end

function _M.GUI:setCurrTextStyle(name)
	local style = textstyles.get(name)
	if (nil == style) then
		return
	end

	self._currTextStyle = name
	self._factory:setCurrTextStyle(style)
end

function _M.GUI:loadLayout(fileName, prefix, parent, params)
	return self._layoutParser:createFromLayout(fileName, prefix, parent, params)
end

function _M.GUI:_handleMouseMove(event, params)
	return self:injectMouseMove(event.x, event.y)
end

function _M.GUI:_handleMouseButtonDown(event, params)
	return self:injectMouseButtonDown(event.button)
end

function _M.GUI:_handleMouseButtonUp(event, params)
	return self:injectMouseButtonUp(event.button)
end

function _M.GUI:_handleKeyDown(event, params)
	return self:injectKeyDown(event.key)
end

function _M.GUI:_handleKeyUp(event, params)
	return self:injectKeyUp(event.key)
end

function _M.GUI:registerEvents(obj, func)
	obj[func](obj, eventtypes.MOUSE_MOVE, "gui", GUI_PRIORITY, self, "_handleMouseMove")
	obj[func](obj, eventtypes.MOUSE_BUTTON_DOWN, "gui", GUI_PRIORITY, self, "_handleMouseButtonDown")
	obj[func](obj, eventtypes.MOUSE_BUTTON_UP, "gui", GUI_PRIORITY, self, "_handleMouseButtonUp")
	obj[func](obj, eventtypes.KEY_DOWN, "gui", GUI_PRIORITY, self, "_handleKeyDown")
	obj[func](obj, eventtypes.KEY_UP, "gui", GUI_PRIORITY, self, "_handleKeyUp")
end

function _M.GUI:shutdown()
	textures.releaseAll()
	fonts.releaseAll()
	textstyles.releaseAll()

	for k, v in pairs(self._windows) do
		v:destroy()
	end
end

function _M.GUI:init(width, height)
	self._width = width
	self._height = height

	self._windows = {}
	self._propToWindow = {}

	self._focus = nil
	self._over = nil
	self._dragging = nil

	self._keysDown = {}
	self._currMouseX = 0
	self._currMouseY = 0
	self._prevMouseX = 0
	self._prevMouseY = 0
	self._mouseButtonsDown = {}
	self._touches = {}

	self._themes = {}
	self._currTheme = nil
	self._currFont = nil
	self._factory = factory.Factory(self)
	self._layoutParser = layoutparser.LayoutParser(self._factory)

	self:_createGUILayer(width, height)
end

return _M
