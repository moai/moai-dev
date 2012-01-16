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

	FILE: awindow.lua
	DESCRIPTION: Base class for all widgets
	AUTHOR: Derick Dong
	VERSION: 0.1
	MOAI VERSION: 0.7
	CREATED: 9-9-11

	TODO
	- Add support for non-PC based input (eg Touch and Motion)
	- Events "DragItemEnters", "DragItemLeaves", and "DragItemDropped" are not handled.

	BUGS
	- Change show() and hide() so they work as they should. Currently, the functions
	  simply move the widget into and out of the view area, respectively. The code for
	  handling the functionality is commented out. However, sortedPropListForPoint()
	  seems to be giving me wonky results, so when trying to determine which widget the
	  input is over, an incorrect value is returned.
]]

module(..., package.seeall)

require "gui\\support\\class"

local text = require "gui\\text"

EventHandler = class()
AWindow = class()

function EventHandler:init(obj, func, data)
	self.obj = obj
	self.func = func
	self.data = data
end

function AWindow:_handleEvent(eventType, event)
	local handler = self._eventHandlers[eventType]
	if (nil == handler or nil == handler.func) then
		return false
	end

	if (nil == handler.obj) then
		return handler.func(event, handler.data)
	else
		return handler.obj[handler.func](handler.obj, event, handler.data)
	end
end

-- function AWindow:_onSetPriority()

-- end

function AWindow:_setPriority(priority)
	for i, v in ipairs(self._props) do
		for i2, v2 in ipairs(v) do
			v2:setPriority(priority + i - 1)
		end
	end

	self._priority = priority
	self._text:setPriority(self._priority + #self._props + 1)

	local childPriority = self._priority + #self._props + 1 + 1
	for i, v in ipairs(self._widgetChildren) do
		v:_setPriority(childPriority)
	end

	for i, v in ipairs(self._children) do
		v:_setPriority(childPriority)
	end

	-- self:_onSetPriority()
end

function AWindow:_setParent(parent)
	self._parent = parent

	self:_setPriority(self._parent:getPriority() + #self._parent._props + 1 + 1)
	self:_setTextPos()
end

function AWindow:_setTextRect()
	local width = self:screenWidth()

	self._text:setRect(width)
end

function AWindow:_setTextAlignment()
	if (self._textAlignHorz == self.TEXT_ALIGN_LEFT) then
		align = MOAITextBox.LEFT_JUSTIFY
	elseif (self._textAlignHorz == self.TEXT_ALIGN_CENTER) then
		align = MOAITextBox.CENTER_JUSTIFY
	else
		align = MOAITextBox.RIGHT_JUSTIFY
	end

	self._text:setAlignment(align)
end

function AWindow:_setTextPos()
	local x = self:screenX()
	local y

	height = self._text:height()
	if (nil == height) then
		return
	end

	if (self._textAlignVert == self.TEXT_ALIGN_TOP) then
		y = 0
	elseif (self._textAlignVert == self.TEXT_ALIGN_BOTTOM) then
		y = self:screenHeight() - height
	else
		y = (self:screenHeight() - height) * 0.5
	end

	y = -(self:screenY() + y)
	self._text:setPos(x, y)
end

function AWindow:getType()
	return self._type
end

function AWindow:setName(name)
	self._name = name
end

function AWindow:getName()
	return self._name
end

function AWindow:setUserData(data)
	self._userData = data
end

function AWindow:getUserData()
	return self._userData
end

function AWindow:setEnabled(flag)
	self._enabled = flag
	
	for i, v in ipairs(self._children) do
		v:setEnabled(flag)
	end

	for i, v in ipairs(self._widgetChildren) do
		v:setEnabled(flag)
	end
end

function AWindow:getEnabled()
	return self._enabled
end

function AWindow:setDraggable(flag)
	self._draggable = flag
end

function AWindow:getDraggable()
	return self._draggable
end

function AWindow:setInputPassThrough(flag)
	self._inputPassThrough = flag
end

function AWindow:getInputPassThrough()
	return self._inputPassThrough
end

function AWindow:_onSetPos()

end

function AWindow:_setScreenPos(screenX, screenY)
	for i, v in ipairs(self._props) do
		for i2, v2 in ipairs(v) do
			v2:setLoc(screenX, -screenY)
		end
	end

	self:_setTextPos()
end

function AWindow:_addWidgetChild(child)
	self._widgetChildren[#self._widgetChildren + 1] = child
	
	child:_setParent(self)

	local x, y = child:getPos()
	child:setPos(x, y)
end

function AWindow:_removeWidgetChild(child)
	for i, v in ipairs(self._widgetChildren) do
		if (v == child) then
			v:destroy()
			break
		end
	end
end

function AWindow:_getWidgetChild(idx)
	return self._widgetChildren[idx]
end

function AWindow:_moveChildren(x, y)
	for i, v in ipairs(self._children) do
		local childX, childY = v:getPos()
		local screenX, screenY = self._gui:_calcAbsValue(childX, childY)

		v:_setScreenPos(x + screenX, y + screenY)
		v:_moveChildren(x + screenX, y + screenY)

		v:_moveWidgetChildren(x + screenX, y + screenY)
	end
end

function AWindow:_moveWidgetChildren(x, y)
	for i, v in ipairs(self._widgetChildren) do
		local childX, childY = v:getPos()
		local screenX, screenY = self._gui:_calcAbsValue(childX, childY)

		v:_setScreenPos(x + screenX, y + screenY)
		v:_moveWidgetChildren(x + screenX, y + screenY)
	end
end

function AWindow:setPos(x, y)
	local parentX = 0
	local parentY = 0

	self._x = x
	self._y = y
	
	if (self._parent ~= nil) then
		parentX, parentY = self._parent:getScreenPos()
	end

	local screenX, screenY = self._gui:_calcAbsValue(x, y)
	local fullX = screenX + parentX
	local fullY = screenY + parentY
	self:_setScreenPos(fullX, fullY)
	self:_moveWidgetChildren(fullX, fullY)
	self:_moveChildren(fullX, fullY)
	
	-- self:_calcTextRect()
	
	self:_onSetPos()
end

function AWindow:modPos(deltaX, deltaY)
	self:setPos(self._x + deltaX, self._y + deltaY)
end

function AWindow:getPos()
	return self._x, self._y
end

function AWindow:x()
	return self._x
end

function AWindow:y()
	return self._y
end

function AWindow:getFullPos()
	local x = self._x
	local y = self._y
	local parent = self._parent
	while (nil ~= parent) do
		x = x + parent._x
		y = y + parent._y

		parent = parent._parent
	end

	return x, y
end

function AWindow:fullX()
	local x = self._x
	local parent = self._parent
	while (nil ~= parent) do
		x = x + parent._x

		parent = parent._parent
	end

	return x
end

function AWindow:fullY()
	local y = self._y
	local parent = self._parent
	while (nil ~= parent) do
		y = y + parent._y

		parent = parent._parent
	end

	return y
end

function AWindow:getScreenPos()
	local x, y = self._props[self._BASE_OBJECTS_INDEX][1]:getLoc()
	return x, -y
end

function AWindow:screenX()
	local x, y = self._props[self._BASE_OBJECTS_INDEX][1]:getLoc()
	return x
end

function AWindow:screenY()
	local x, y = self._props[self._BASE_OBJECTS_INDEX][1]:getLoc()
	return -y
end

function AWindow:_onSetDim()

end

function AWindow:setDim(width, height)
	self._width = width
	self._height = height

	self._screenWidth, self._screenHeight = self._gui:_calcAbsValue(width, height)

	for i, v in ipairs(self._quads) do
		for i2, v2 in ipairs(v) do
			v2:setRect(0, -self._screenHeight, self._screenWidth, 0)
		end
	end

	self:_setTextPos()
	self:_setTextRect()
	self:_setTextAlignment()

	self:_onSetDim()
end

function AWindow:getDim()
	return self._width, self._height
end

function AWindow:width()
	return self._width
end

function AWindow:height()
	return self._height
end

function AWindow:getScreenDim()
	return self._screenWidth, self._screenHeight
end

function AWindow:screenWidth()
	return self._screenWidth
end

function AWindow:screenHeight()
	return self._screenHeight
end

function AWindow:parent()
	return self._parent
end

function AWindow:gainFocus()

end

function AWindow:loseFocus()

end

function AWindow:addChild(child)
	self._children[#self._children + 1] = child
	
	child:_setParent(self)

	local x, y = child:getPos()
	child:setPos(x, y)
end

function AWindow:removeChild(child)
	for k, v in pairs(self._children) do
		if (v == child) then
			v._parent = nil
			return table.remove(self._children, k)
		end
	end
end

function AWindow:children()
	return self._children
end

function AWindow:getChildByName(name)
	for k, v in pairs(self._children) do
		if (v.name == name) then
			return v
		end
	end
end

function AWindow:getChild(idx)
	return self._children[idx]
end

function AWindow:show()
	if (false == self._visible) then
		self:modPos(1000, 1000)
		self._visible = true
	end


	-- for k, v in pairs(self._props) do
		-- for k2, v2 in pairs(v) do
			-- v2:setVisible(true)
		-- end
	-- end

	-- self._visible = true
	
	-- for i, v in ipairs(self._children) do
		-- v:show()
	-- end

	-- for i, v in ipairs(self._widgetChildren) do
		-- v:show()
	-- end

	-- self._text:show()
end

function AWindow:hide()
	if (true == self._visible) then
		self:modPos(-1000, -1000)
		self._visible = false
	end


	-- for k, v in pairs(self._props) do
		-- for k2, v2 in pairs(v) do
			-- v2:setVisible(false)
		-- end
	-- end

	-- self._visible = false

	-- for i, v in ipairs(self._children) do
		-- v:hide()
	-- end

	-- for i, v in ipairs(self._widgetChildren) do
		-- v:hide()
	-- end

	-- self._text:hide()
end

function AWindow:getVisible()
	return self._visible
end

function AWindow:moveToFront()
	self._gui:moveToFront(self)
end

function AWindow:moveToBack()
	self._gui:moveToBack(self)
end

function AWindow:moveInFrontOf(win)
	self:_setPriority(win:getPriority() + 1)
end

function AWindow:moveBehind(win)
	self:_setPriority(win:getPriority() - 1)
end

function AWindow:getPriority()
	return self._priority
end

function AWindow:setText(text)
	self._text:setString(text)

	-- self:_calcTextDim()
	self:_setTextRect()
	self:_setTextAlignment()
end

function AWindow:getText()
	return self._text:getString()
end

function AWindow:setTextFont(font, size)
	self._text:setFont(font, size)
	
	-- self:_calcTextDim()
	self:_setTextRect()
	self:_setTextAlignment()
end

function AWindow:getTextFont()
	return self._text:getFont()
end

function AWindow:setTextAlignment(horz, vert)
	if (nil == horz) then
		horz = self.TEXT_ALIGN_CENTER
	end
	
	if (nil == vert) then
		vert = self.TEXT_ALIGN_CENTER
	end

	self._textAlignHorz = horz
	self._textAlignVert = vert
	
	self:_setTextPos()
	self:_setTextAlignment()
end

function AWindow:getTextAlignment()
	return self._textAlignHorz, self._textAlignVert
end

function AWindow:setTextColor(r, g, b, a)
	self._text:setColor(r, g, b, a)
end

function AWindow:getTextColor()
	return self._text:getColor()
end

function AWindow:registerEventHandler(eventType, obj, func, data)
	if (nil == func) then
		self._eventHandlers[eventType] = nil
		return
	end

	local handler = EventHandler(obj, func, data)
	self._eventHandlers[eventType] = handler
end

function AWindow:unregisterEventHandler(eventType)
	self:registerEventHandler(eventType, nil)
end

function AWindow:setEventData(eventType, data)
	local handler = self._eventHandlers[eventType]
	if (handler ~= nil) then
		handler.data = data
	end
end

function AWindow:setFrame(tl, tr, bl, br, top, bottom, left, right)
	
end

function AWindow:setAlpha(alpha)
	self._alpha = alpha

	for i, v in ipairs(self._props) do
		for i2, v2 in ipairs(v) do
			v2:setColor(1, 1, 1, alpha)
		end
	end
end

function AWindow:getAlpha()
	return self._alpha
end

function AWindow:destroy()
	if (self._gui:getFocus() == self) then
		self._gui:setFocus(nil)
	end

	if (self._gui:getOver() == self) then
		self._gui:setOver(nil)
	end

	for i, v in ipairs(self._children) do
		v:destroy()
	end

	for i, v in ipairs(self._widgetChildren) do
		v:destroy()
	end

	self._text:destroy()

	for i, v in ipairs(self._quads) do
		for i2, v2 in ipairs(v) do
			v2:setTexture(nil)
		end
	end

	for i, v in ipairs(self._props) do
		for i2, v2 in ipairs(v) do
			self._gui:_unregisterHitObject(v2)
			self._gui:_destroyRenderObject(v2)
		end
	end
end





function AWindow:_baseHandleGainFocus(event)
	return self:_handleEvent(self.EVENT_GAIN_FOCUS, event)
end

function AWindow:_baseHandleLoseFocus(event)
	return self:_handleEvent(self.EVENT_LOSE_FOCUS, event)
end

function AWindow:_baseHandleDragStart(event)
	return self:_handleEvent(self.EVENT_DRAG_START, event)
end

function AWindow:_baseHandleDragEnd(event)
	return self:_handleEvent(self.EVENT_DRAG_END, event)
end

function AWindow:_baseHandleDragItemEnters(event)
	return self:_handleEvent(self.EVENT_DRAG_ITEM_ENTERS, event)
end

function AWindow:_baseHandleDragItemLeaves(event)
	return self:_handleEvent(self.EVENT_DRAG_ITEM_LEAVES, event)
end

function AWindow:_baseHandleDragItemDropped(event)
	return self:_handleEvent(self.EVENT_DRAG_ITEM_DROPPED, event)
end

function AWindow:_baseHandleMouseUp(event)
	return self:_handleEvent(self.EVENT_MOUSE_UP, event)
end

function AWindow:_baseHandleMouseDown(event)
	return self:_handleEvent(self.EVENT_MOUSE_DOWN, event)
end

function AWindow:_baseHandleMouseMove(event)
	return self:_handleEvent(self.EVENT_MOUSE_MOVE, event)
end

function AWindow:_baseHandleKeyDown(event)
	return self:_handleEvent(self.EVENT_KEY_DOWN, event)
end

function AWindow:_baseHandleKeyUp(event)
	return self:_handleEvent(self.EVENT_KEY_UP, event)
end

function AWindow:_baseHandleMouseEnters(event)
	return self:_handleEvent(self.EVENT_MOUSE_ENTERS, event)
end

function AWindow:_baseHandleMouseLeaves(event)
	return self:_handleEvent(self.EVENT_MOUSE_LEAVES, event)
end

function AWindow:_baseHandleMouseClick(event)
	return self:_handleEvent(self.EVENT_MOUSE_CLICK, event)
end

function AWindow:_baseHandleTouchEnters(event)
	return self:_handleEvent(self.EVENT_TOUCH_ENTERS, event)
end

function AWindow:_baseHandleTouchLeaves(event)
	return self:_handleEvent(self.EVENT_TOUCH_LEAVES, event)
end

function AWindow:_baseHandleTouchDown(event)
	return self:_handleEvent(self.EVENT_TOUCH_DOWN, event)
end

function AWindow:_baseHandleTouchUp(event)
	return self:_handleEvent(self.EVENT_TOUCH_UP, event)
end

function AWindow:_baseHandleTouchTap(event)
	return self:_handleEvent(self.EVENT_TOUCH_TAP, event)
end





function AWindow:_onHandleGainFocus(event)
	return self:_baseHandleGainFocus(event)
end

function AWindow:_onHandleLoseFocus(event)
	return self:_baseHandleLoseFocus(event)
end

function AWindow:_onHandleDragStart(event)
	return self:_baseHandleDragStart(event)
end

function AWindow:_onHandleDragEnd(event)
	return self:_baseHandleDragEnd(event)
end

function AWindow:_onHandleDragItemEnters(event)
	return self:_baseHandleDragItemEnters(event)
end

function AWindow:_onHandleDragItemLeaves(event)
	return self:_baseHandleDragItemLeaves(event)
end

function AWindow:_onHandleDragItemDropped(event)
	return self:_baseHandleDragItemDropped(event)
end

function AWindow:_onHandleMouseUp(event)
	return self:_baseHandleMouseUp(event)
end

function AWindow:_onHandleMouseDown(event)
	return self:_baseHandleMouseDown(event)
end

function AWindow:_onHandleMouseMove(event)
	return self:_baseHandleMouseMove(event)
end

function AWindow:_onHandleKeyDown(event)
	return self:_baseHandleKeyDown(event)
end

function AWindow:_onHandleKeyUp(event)
	return self:_baseHandleKeyUp(event)
end

function AWindow:_onHandleMouseEnters(event)
	return self:_baseHandleMouseEnters(event)
end

function AWindow:_onHandleMouseLeaves(event)
	return self:_baseHandleMouseLeaves(event)
end

function AWindow:_onHandleMouseClick(event)
	return self:_baseHandleMouseClick(event)
end

function AWindow:_onHandleTouchEnters(event)
	return self:_baseHandleTouchEnters(event)
end

function AWindow:_onHandleTouchLeaves(event)
	return self:_baseHandleTouchLeaves(event)
end

function AWindow:_onHandleTouchDown(event)
	return self:_baseHandleTouchDown(event)
end

function AWindow:_onHandleTouchUp(event)
	return self:_baseHandleTouchUp(event)
end

function AWindow:_onHandleTouchTap(event)
	return self:_baseHandleTouchTap(event)
end

function AWindow:_passThrough()
	if (true == self._inputPassThrough) then
		if (nil ~= self._parent) then
			return self._parent:_passThrough()
		end
	end

	return not self._inputPassThrough
end

function AWindow:_passUpMouseUp(event)
	local result = self:_onHandleMouseUp(event)
	if (not result) then
		if (nil ~= self._parent) then
			result = self._parent:_passUpMouseUp(event)
		end
	end

	return result
end

function AWindow:_handleMouseUp(event)
	if (true == self._dragging) then
		self:_handleDragEnd()
		self._gui._dragging = nil
		self._dragging = false
	end

	self:_passUpMouseUp(event)

	return self:_passThrough()
end

function AWindow:_passUpMouseDown(event)
	local result = false
	if (not self._dragging) then
		result = self:_onHandleMouseDown(event)
		if (not result) then
			if (nil ~= self._parent) then
				result = self._parent:_passUpMouseDown(event)
			end
		end
	end

	return result
end

function AWindow:_handleMouseDown(event)
	if (true == self._draggable) then
		self._dragging = true
		self._gui._dragging = self
		self:_handleDragStart()
	end

	self:_passUpMouseDown(event)

	return self:_passThrough()
end

function AWindow:_handleMouseMove(event)
	if (true == self._dragging) then
		local diffX, diffY = self._gui:_calcRelValue(event.x - event.prevX, event.y - event.prevY)
		self:setPos(self:x() + diffX, self:y() + diffY)
	end

	local result = self:_onHandleMouseMove(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleMouseMove(event)
		end
	end

	return result
end

function AWindow:_handleKeyDown(event)
	local result = self:_onHandleKeyDown(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleKeyDown(event)
		end
	end

	return result
end

function AWindow:_handleKeyUp(event)
	local result = self:_onHandleKeyUp(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleKeyUp(event)
		end
	end

	return result
end

function AWindow:_handleGainFocus(event)
	self._gui:moveToFront(self)
	local result = self:_onHandleGainFocus(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleGainFocus(event)
		end
	end

	return result
end

function AWindow:_handleLoseFocus(event)
	local result = self:_onHandleLoseFocus(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleLoseFocus(event)
		end
	end

	return result
end

function AWindow:_handleDragStart(event)
	local result = self:_onHandleDragStart(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleDragStart(event)
		end
	end

	return result
end

function AWindow:_handleDragEnd(event)
	local result = self:_onHandleDragEnd(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleDragEnd(event)
		end
	end

	return result
end

function AWindow:_handleDragItemEnters(event)
	local result = self:_onHandleDragItemEnters(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleDragItemEnters(event)
		end
	end

	return result
end

function AWindow:_handleDragItemLeaves(event)
	local result = self:_onHandleDragItemLeaves(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleDragItemLeaves(event)
		end
	end

	return result
end

function AWindow:_handleDragItemDropped(event)
	local result = self:_onHandleDragItemDropped(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleDragItemDropped(event)
		end
	end

	return result
end

function AWindow:_handleMouseEnters(event)
	local result = self:_onHandleMouseEnters(event)
	if (not result) then
		if (nil ~= self._parent) then
			result = self._parent:_onHandleMouseEnters(event)
		end
	end

	return result
end

function AWindow:_handleMouseLeaves(event)
	local result = self:_onHandleMouseLeaves(event)
	if (not result) then
		if (nil ~= self._parent) then
			result = self._parent:_handleMouseLeaves(event)
		end
	end

	return result
end

function AWindow:_handleMouseClick(event)
	local result = self:_onHandleMouseClick(event)
	if (not result) then
		if (nil ~= self._parent) then
			result = self._parent:_handleMouseClick(event)
		end
	end

	return result
end

function AWindow:_handleTouchEnters(event)
	local result = self:_onHandleTouchEnters(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleTouchEnters(event)
		end
	end

	return result
end

function AWindow:_handleTouchLeaves(event)
	local result = self:_onHandleTouchLeaves(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleTouchLeaves(event)
		end
	end

	return result
end

function AWindow:_handleTouchDown(event)
	if (true == self._draggable) then
		self._dragging = true
		self._gui._dragging = self
		self:_handleDragStart()
	end

	if (not self._dragging) then
		local result = self:_onHandleTouchDown(event)
		if (not result) then
			if (nil ~= self._parent) then
				self._parent:_handleTouchDown(event)
			end
		end
	end

	return self:_passThrough()
end

function AWindow:_handleTouchUp(event)
	if (true == self._dragging) then
		self:_handleDragEnd()
		self._gui._dragging = nil
		self._dragging = false
	end

	local result = self:_onHandleTouchUp(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleTouchUp(event)
		end
	end

	return result
end

function AWindow:_handleTouchTap(event)
	local result = self:_onHandleTouchTap(event)
	if (not result) then
		if (nil ~= self._parent) then
			self._parent:_handleTouchTap(event)
		end
	end

	return result
end

function AWindow:_AWindowPrivateVars()
	self._type = "AWindow"
	self._name = ""
	self._children = {}
	self._widgetChildren = {}
	self._userData = nil
	self._priority = 0
	self._enabled = true
	self._visible = true
	self._draggable = false
	self._dragging = false
	self._inputPassThrough = false
	self._x = 0
	self._y = 0
	self._width = 0
	self._height = 0
	self._screenX = 0
	self._screenY = 0
	self._screenWidth = 0
	self._screenHeight = 0
	self._text = text.Text(self)
	self._textAlignHorz = self.TEXT_ALIGN_CENTER
	self._textAlignVert = self.TEXT_ALIGN_CENTER
	self._eventHandlers = {}
	self._alpha = alpha
end

function AWindow:_AWindowEvents()
	self.EVENT_SHOW = "EventShow"
	self.EVENT_HIDE = "EventHide"
	self.EVENT_GAIN_FOCUS = "EventGainFocus"
	self.EVENT_LOSE_FOCUS = "EventLoseFocus"
	self.EVENT_ENABLE = "EventEnable"
	self.EVENT_DISABLE = "EventDisable"
	self.EVENT_DRAG_START = "EventDragStart"
	self.EVENT_DRAG_END = "EventDragEnd"
	self.EVENT_DRAG_ITEM_ENTERS = "EventDragItemEnters"
	self.EVENT_DRAG_ITEM_LEAVES = "EventDragItemLeaves"
	self.EVENT_DRAG_ITEM_DROPPED = "EventDragItemDropped"
	self.EVENT_MOVE = "EventMove"
	self.EVENT_SIZE = "EventSize"
	self.EVENT_MOUSE_UP = "EventMouseUp"
	self.EVENT_MOUSE_DOWN = "EventMouseDown"
	self.EVENT_MOUSE_MOVE = "EventMouseMove"
	self.EVENT_KEY_DOWN = "EventKeyDown"
	self.EVENT_KEY_UP = "EventKeyUp"
	self.EVENT_MOUSE_ENTERS = "EventMouseEnters"
	self.EVENT_MOUSE_LEAVES = "EventMouseLeaves"
	self.EVENT_MOUSE_MOVES = "EventMouseMoves"
	self.EVENT_MOUSE_CLICK = "EventMouseClick"
	self.EVENT_TOUCH_ENTERS = "EventTouchEnters"
	self.EVENT_TOUCH_LEAVES = "EventTouchLeaves"
	self.EVENT_TOUCH_DOWN = "EventTouchDown"
	self.EVENT_TOUCH_UP = "EventTouchUp"
	self.EVENT_TOUCH_TAP = "EventTouchTap"

end

function AWindow:init(gui)
	self._gui = gui
	self:_AWindowPrivateVars()
	self:_AWindowEvents()

	self.TEXT_ALIGN_LEFT = 0
	self.TEXT_ALIGN_TOP = 0
	self.TEXT_ALIGN_CENTER = 1
	self.TEXT_ALIGN_RIGHT = 2
	self.TEXT_ALIGN_BOTTOM = 2

	self._BASE_OBJECTS_INDEX = 1
	self._FRAME_OBJECTS_INDEX = 2
	self._WIDGET_SPECIFIC_OBJECTS_INDEX = 3

	self._quads = {}
	self._props = {}

	for i = 1, self._WIDGET_SPECIFIC_OBJECTS_INDEX do
		self._quads[#self._quads + 1] = {}
		self._props[#self._props + 1] = {}
	end

	quad, prop = self._gui:_createRenderObject()
	self._gui:_registerHitObject(self, prop)
	self._quads[self._BASE_OBJECTS_INDEX][#self._quads[self._BASE_OBJECTS_INDEX] + 1] = quad
	self._props[self._BASE_OBJECTS_INDEX][#self._props[self._BASE_OBJECTS_INDEX] + 1] = prop

	self:_setPriority(0)

	self:setTextAlignment(self._textAlignHorz, self._textAlignVert)
end
