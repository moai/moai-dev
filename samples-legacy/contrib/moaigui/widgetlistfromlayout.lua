
--[[
	Demo for a basic GUI system

	This demonstrates loading a widget list from a GUI layout file.
]]

local gui = require "gui\\gui"
local resources = require "gui\\support\\resources"
local filesystem = require "gui\\support\\filesystem"
local inputconstants = require "gui\\support\\inputconstants"
local layermgr = require "layermgr"

-- Turn off the texture loading logging
MOAILogMgr.setLogLevel(MOAILogMgr.LOG_NONE)

-- Setup your basic window
local width = 320
local height = 480

MOAISim.openWindow("Widget List From Layout", width, height)

-- Create the GUI, passing in the dimensions of the screen
local g = gui.GUI(width, height)

-- Search through these for specified resources
g:addToResourcePath(filesystem.pathJoin("resources", "fonts"))
g:addToResourcePath(filesystem.pathJoin("resources", "gui"))
g:addToResourcePath(filesystem.pathJoin("resources", "media"))
g:addToResourcePath(filesystem.pathJoin("resources", "themes"))
g:addToResourcePath(filesystem.pathJoin("resources", "layouts"))

-- Add the gui layer to the rendering stack, making sure its always high up on the
-- layer stack
layermgr.addLayer("gui", 99999, g:layer())

-- This sets up the theme to be used for widgets. If images aren't set
-- manually for a widget, the system looks to the theme for the images
-- to use.
g:setTheme("basetheme.lua")

-- The font used for text
g:setCurrTextStyle("default")

-- Load the widgets from the specified file
-- Parameter 1: filename of the layout
-- Parameter 2: (optional) a prefix added to the name (from the layout file) of each widget; this
--		is to help avoid name collisions
-- Returns three values, but the second is the only one we're really interested in
local roots, widgets, groups = g:loadLayout(resources.getPath("widgetlistlayout.lua"))

-- Check if the widget list is a part of the layout file
if (nil ~= widgets.widgetlist1) then
	local widgetList = widgets.widgetlist1.window

	-- Add some rows, and fill it with some junk data
	for i = 1, 30 do
		local row = widgetList:addRow()

		-- The return from getCell is the widget created by setColumnWidget, so the normal
		-- functionality for the widget is available.
		row:getCell(1):setText("Name " .. i)
		row:getCell(2):setText(tostring(i))
	end
end


-- Callback functions for input
-- When the user provides input, it must be sent into the GUI system so that it can
-- be handled properly.
function onKeyboardEvent(key, down)
	if (down == true) then
		g:injectKeyDown(key)
	else
		g:injectKeyUp(key)

		-- Get the current selections made by the user, and print them out
		local selections = widgetList:getSelections()
		if (nil == selections) then return end

		print("---------------------")
		for i = 1, #selections do
			print(selections[i])
		end
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )

function onPointerEvent(x, y)
	g:injectMouseMove(x, y)
end

function onMouseLeftEvent(down)
	if (down) then
		g:injectMouseButtonDown(inputconstants.LEFT_MOUSE_BUTTON)
	else
		g:injectMouseButtonUp(inputconstants.LEFT_MOUSE_BUTTON)
	end
end

function onMouseMiddleEvent(down)
	if (down) then
		g:injectMouseButtonDown(inputconstants.MIDDLE_MOUSE_BUTTON)
	else
		g:injectMouseButtonUp(inputconstants.MIDDLE_MOUSE_BUTTON)
	end
end

function onMouseRightEvent(down)
	if (down) then
		g:injectMouseButtonDown(inputconstants.RIGHT_MOUSE_BUTTON)
	else
		g:injectMouseButtonUp(inputconstants.RIGHT_MOUSE_BUTTON)
	end
end

-- Register the callbacks for input
MOAIInputMgr.device.pointer:setCallback(onPointerEvent)
MOAIInputMgr.device.mouseLeft:setCallback(onMouseLeftEvent)
MOAIInputMgr.device.mouseMiddle:setCallback(onMouseMiddleEvent)
MOAIInputMgr.device.mouseRight:setCallback(onMouseRightEvent)




